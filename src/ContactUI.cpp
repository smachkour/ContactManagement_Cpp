// ContactUI.cpp
#include "ContactUI.hpp"
#include "ContactManager.hpp" // Include to access the template function
#include <iostream>
#include <thread>
namespace contact_management {

ContactUI::ContactUI() : m_isRunning(true) {}

void ContactUI::run() {
    std::string choice;
    do {
        displayMenu();
        std::getline(std::cin, choice);
        
        if (!isValidChoice(choice)) {
            std::cout << "Invalid choice. Please try again." << std::endl;
            continue;
        }

        try {
            int choiceNum = std::stoi(choice);
            switch (choiceNum) {
                   case 1:
                    addContact();
                    break;
                case 2:
                    removeContact();
                    break;
                case 3:
                    displayContacts();
                    break;
                case 4:
                    findContactsByName();
                    break;
                case 5:
                    saveContactsToFile();
                    break;
                case 6:
                    loadContactsFromFile();
                    break;
                case 7:
                    setFavoriteContact();
                    break;
                case 8:
                    displayFavoriteContact();
                    break;
                case 9:
                    exportContactsToJson();
                    break;
                case 10:
                    importContactsFromJson();
                    break;
                case 11:
                    m_isRunning = false;
                    std::cout << "Exiting..." << std::endl;
                    break;
                default:
                    std::cout << "Invalid choice. Please try again." << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
        
        std::cout << std::endl;
    } while (m_isRunning);

    // Using a thread to display contact count after a delay
    std::thread countThread([this]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        displayContactCount(*this);
    });
    countThread.join();
}

void ContactUI::displayMenu() {
    std::cout << "Contact Management System" << std::endl;
    std::cout << "1. Add Contact" << std::endl;
    std::cout << "2. Remove Contact" << std::endl;
    std::cout << "3. Display Contacts" << std::endl;
    std::cout << "4. Find Contacts by Name" << std::endl;
    std::cout << "5. Save Contacts to File" << std::endl;
    std::cout << "6. Load Contacts from File" << std::endl;
    std::cout << "7. Set Favorite Contact" << std::endl;
    std::cout << "8. Display Favorite Contact" << std::endl;
    std::cout << "9. Export Contacts to JSON" << std::endl;
    std::cout << "10. Import Contacts from JSON" << std::endl;
    std::cout << "11. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

void ContactUI::addContact() {
    std::string name, phone, email, company;
    bool isBusinessContact;
    
    std::cout << "Enter name: ";
    std::getline(std::cin, name);
    std::cout << "Enter phone: ";
    std::getline(std::cin, phone);
    std::cout << "Enter email: ";
    std::getline(std::cin, email);
    std::cout << "Is this a business contact? (1 for yes, 0 for no): ";
    std::cin >> isBusinessContact;
    std::cin.ignore(); // Ignore the newline character
    
    if (isBusinessContact) {
        std::cout << "Enter company: ";
        std::getline(std::cin, company);
        m_contactManager.addContact(std::make_shared<BusinessContact>(name, phone, email, company));
    } else {
        m_contactManager.addContact(std::make_shared<Contact>(name, phone, email));
    }

    std::cout << "Contact added successfully!" << std::endl;
}

void ContactUI::removeContact() {
    unsigned char index;
    std::cout << "Enter the index of the contact to remove: ";
    std::cin >> index;
    std::cin.ignore(); // Ignore the newline character
    
    try {
        m_contactManager.removeContact(index);
        std::cout << "Contact removed successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void ContactUI::displayContacts() {
    m_contactManager.displayAllContacts(); // Call the renamed function
}

void ContactUI::findContactsByName() {
    std::string name;
    std::cout << "Enter the name to search for: ";
    std::getline(std::cin, name);
    
    auto foundContacts = m_contactManager.findContactsByName(name);
    if (foundContacts.empty()) {
        std::cout << "No contacts found with the name: " << name << std::endl;
    } else {
        std::cout << "Found contacts:" << std::endl;
        contact_management::displayContacts(foundContacts); // Call the template function with found contacts
    }
}

void ContactUI::saveContactsToFile() {
    std::string filename;
    std::cout << "Enter the filename to save to: ";
    std::getline(std::cin, filename);
    
    try {
        m_contactManager.saveToFile(filename);
        std::cout << "Contacts saved successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void ContactUI::loadContactsFromFile() {
    std::string filename;
    std::cout << "Enter the filename to load from: ";
    std::getline(std::cin, filename);
    
    try {
        m_contactManager.loadFromFile(filename);
        std::cout << "Contacts loaded successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}


void ContactUI::filterContacts(const std::function<bool(const Contact&)>& filter) {
    auto filteredContacts = m_contactManager.filterContacts(filter);
    displayFilteredContacts(filteredContacts);
}

void ContactUI::displayFilteredContacts(const std::vector<std::shared_ptr<Contact>>& contacts) {
    for (const auto& contact : contacts) {
        contact->displayDetails();
        std::cout << std::endl;
    }
}

void ContactUI::setFavoriteContact() {
    std::vector<std::shared_ptr<Contact>> contacts = m_contactManager.getAllContacts();
    
    if (contacts.empty()) {
        std::cout << "No contacts available to set as favorite." << std::endl;
        return;
    }

    std::cout << "Select a contact to set as favorite:" << std::endl;
    for (size_t i = 0; i < contacts.size(); ++i) {
        std::cout << i + 1 << ". " << contacts[i]->getName() << std::endl;
    }

    size_t choice;
    while (true) {
        std::cout << "Enter the number of the contact (1-" << contacts.size() << "): ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice > 0 && choice <= contacts.size()) {
            break;
        }
        std::cout << "Invalid choice. Please try again." << std::endl;
    }

    m_contactManager.setFavoriteContact(choice - 1);  // Subtract 1 because array is 0-indexed
    std::cout << "Favorite contact set to: " << contacts[choice - 1]->getName() << std::endl;
}

void ContactUI::displayFavoriteContact() {
    const Contact* favoriteContact = m_contactManager.getFavoriteContact();
    if (favoriteContact == nullptr) {
        std::cout << "No favorite contact set." << std::endl;
    } else {
        std::cout << "Favorite Contact:" << std::endl;
        favoriteContact->displayDetails();
    }
}


// Implement the friend function
void displayContactCount(const ContactUI& ui) {
    std::cout << "Total contacts: " << ui.m_contactManager.getContactCount() << std::endl;
}

void ContactUI::exportContactsToJson() {
    std::string filename;
    std::cout << "Enter the filename to export to (without extension): ";
    std::getline(std::cin, filename);
    
    filename = ensureJsonExtension(filename);
    
    try {
        m_contactManager.exportToJson(filename);
        std::cout << "Contacts exported to JSON successfully: " << filename << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void ContactUI::importContactsFromJson() {
    std::string filename;
    std::cout << "Enter the filename to import from (without extension): ";
    std::getline(std::cin, filename);
    
    filename = ensureJsonExtension(filename);
    
    try {
        m_contactManager.importFromJson(filename);
        std::cout << "Contacts imported from JSON successfully: " << filename << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

} // namespace contact_management