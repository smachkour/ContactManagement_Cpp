// ContactGUI.cpp
#include "ContactGUI.hpp"
#include <FL/fl_ask.H>
#include <sstream>
#include <algorithm>

namespace contact_management {

ContactGUI::ContactGUI(ContactManager& manager) : m_manager(manager) {
    m_window = std::make_unique<Fl_Window>(500, 400, "Contact Manager");
    m_nameInput = std::make_unique<Fl_Input>(100, 10, 200, 25, "Name:");
    m_phoneInput = std::make_unique<Fl_Input>(100, 40, 200, 25, "Phone:");
    m_emailInput = std::make_unique<Fl_Input>(100, 70, 200, 25, "Email:");
    m_addButton = std::make_unique<Fl_Button>(320, 10, 150, 25, "Add Contact");
    m_removeButton = std::make_unique<Fl_Button>(320, 40, 150, 25, "Remove Contact");
    m_displayButton = std::make_unique<Fl_Button>(320, 70, 150, 25, "Display Contacts");
    m_findButton = std::make_unique<Fl_Button>(320, 100, 150, 25, "Find Contacts");
    m_saveButton = std::make_unique<Fl_Button>(320, 130, 150, 25, "Save Contacts");
    m_loadButton = std::make_unique<Fl_Button>(320, 160, 150, 25, "Load Contacts");
    m_setFavoriteButton = std::make_unique<Fl_Button>(320, 190, 150, 25, "Set Favorite");
    m_displayFavoriteButton = std::make_unique<Fl_Button>(320, 220, 150, 25, "Display Favorite");
    m_display = std::make_unique<Fl_Text_Display>(10, 250, 480, 140);
    m_buffer = std::make_unique<Fl_Text_Buffer>();

    m_display->buffer(m_buffer.get());
    m_addButton->callback(addContactCB, this);
    m_removeButton->callback(removeContactCB, this);
    m_displayButton->callback(displayContactsCB, this);
    m_findButton->callback(findContactsCB, this);
    m_saveButton->callback(saveContactsCB, this);
    m_loadButton->callback(loadContactsCB, this);
    m_setFavoriteButton->callback(setFavoriteContactCB, this);
    m_displayFavoriteButton->callback(displayFavoriteContactCB, this);

    m_window->end();
}

void ContactGUI::show() {
    m_window->show();
}

void ContactGUI::addContactCB(Fl_Widget*, void* userdata) {
    static_cast<ContactGUI*>(userdata)->addContact();
}

void ContactGUI::removeContactCB(Fl_Widget*, void* userdata) {
    static_cast<ContactGUI*>(userdata)->removeContact();
}

void ContactGUI::displayContactsCB(Fl_Widget*, void* userdata) {
    static_cast<ContactGUI*>(userdata)->displayContacts();
}

void ContactGUI::findContactsCB(Fl_Widget*, void* userdata) {
    static_cast<ContactGUI*>(userdata)->findContacts();
}

void ContactGUI::saveContactsCB(Fl_Widget*, void* userdata) {
    static_cast<ContactGUI*>(userdata)->saveContacts();
}

void ContactGUI::loadContactsCB(Fl_Widget*, void* userdata) {
    static_cast<ContactGUI*>(userdata)->loadContacts();
}

void ContactGUI::setFavoriteContactCB(Fl_Widget*, void* userdata) {
    static_cast<ContactGUI*>(userdata)->setFavoriteContact();
}

void ContactGUI::displayFavoriteContactCB(Fl_Widget*, void* userdata) {
    static_cast<ContactGUI*>(userdata)->displayFavoriteContact();
}

void ContactGUI::addContact() {
    std::string name = m_nameInput->value();
    std::string phone = m_phoneInput->value();
    std::string email = m_emailInput->value();

    if (name.empty() || phone.empty() || email.empty()) {
        fl_alert("Please fill in all fields.");
        return;
    }

    const char* company = fl_input("Enter company name (leave empty for personal contact):");
    if (company && strlen(company) > 0) {
        m_manager.addContact(BusinessContact(name, phone, email, company));
    } else {
        m_manager.addContact(Contact(name, phone, email));
    }
    fl_message("Contact added successfully!");

    m_nameInput->value("");
    m_phoneInput->value("");
    m_emailInput->value("");
}

void ContactGUI::removeContact() {
    const char* name = fl_input("Enter the name of the contact to remove:");
    if (name) {
        auto contacts = m_manager.getContacts();
        auto it = std::find_if(contacts.begin(), contacts.end(),
            [name](const std::shared_ptr<Contact>& contact) {
                return contact->getName() == name;
            });
        
        if (it != contacts.end()) {
            m_manager.removeContact(std::distance(contacts.begin(), it));
            fl_message("Contact removed successfully!");
        } else {
            fl_alert("Contact not found.");
        }
    }
}

void ContactGUI::displayContacts() {
    std::ostringstream oss;
    for (const auto& contact : m_manager.getContacts()) {
        oss << "Name: " << contact->getName() << "\n"
            << "Phone: " << contact->getPhone() << "\n"
            << "Email: " << contact->getEmail() << "\n";
        
        if (const BusinessContact* businessContact = dynamic_cast<const BusinessContact*>(contact.get())) {
            oss << "Company: " << businessContact->getCompany() << "\n";
        }
        
        oss << "\n";  // Empty line between contacts
    }
    m_buffer->text(oss.str().c_str());
}

void ContactGUI::findContacts() {
    const char* name = fl_input("Enter the name to search for:");
    if (name) {
        auto foundContacts = m_manager.findContactsByName(name);
        std::ostringstream oss;
        if (foundContacts.empty()) {
            oss << "No contacts found with the name: " << name << "\n";
        } else {
            oss << "Found contacts:\n\n";
            for (const auto& contact : foundContacts) {
                oss << "Name: " << contact->getName() << "\n"
                    << "Phone: " << contact->getPhone() << "\n"
                    << "Email: " << contact->getEmail() << "\n";
                
                if (const BusinessContact* businessContact = dynamic_cast<const BusinessContact*>(contact.get())) {
                    oss << "Company: " << businessContact->getCompany() << "\n";
                }
                
                oss << "\n";
            }
        }
        m_buffer->text(oss.str().c_str());
    }
}

void ContactGUI::saveContacts() {
    const char* filename = fl_file_chooser("Save Contacts", "*.txt", "");
    if (filename) {
        try {
            m_manager.saveToFile(filename);
            fl_message("Contacts saved successfully!");
        } catch (const std::exception& e) {
            fl_alert(e.what());
        }
    }
}

void ContactGUI::loadContacts() {
    const char* filename = fl_file_chooser("Load Contacts", "*.txt", "");
    if (filename) {
        try {
            m_manager.loadFromFile(filename);
            fl_message("Contacts loaded successfully!");
        } catch (const std::exception& e) {
            fl_alert(e.what());
        }
    }
}

void ContactGUI::setFavoriteContact() {
    const char* name = fl_input("Enter the name of the contact to set as favorite:");
    if (name) {
        auto contacts = m_manager.getContacts();
        auto it = std::find_if(contacts.begin(), contacts.end(),
            [name](const std::shared_ptr<Contact>& contact) {
                return contact->getName() == name;
            });
        
        if (it != contacts.end()) {
            m_manager.setFavoriteContact(std::distance(contacts.begin(), it));
            fl_message("Favorite contact set successfully!");
        } else {
            fl_alert("Contact not found.");
        }
    }
}

void ContactGUI::displayFavoriteContact() {
    const Contact* favoriteContact = m_manager.getFavoriteContact();
    if (favoriteContact == nullptr) {
        m_buffer->text("No favorite contact set.");
    } else {
        std::ostringstream oss;
        oss << "Favorite Contact:\n\n"
            << "Name: " << favoriteContact->getName() << "\n"
            << "Phone: " << favoriteContact->getPhone() << "\n"
            << "Email: " << favoriteContact->getEmail() << "\n";
        
        if (const BusinessContact* businessContact = dynamic_cast<const BusinessContact*>(favoriteContact)) {
            oss << "Company: " << businessContact->getCompany() << "\n";
        }
        
        m_buffer->text(oss.str().c_str());
    }
}

} // namespace contact_management