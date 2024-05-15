// ContactUI.cpp
#include "ContactUI.hpp"
#include <iostream>
#include <thread>

namespace contact_management { // Everything in a self-made namespace

ContactUI::ContactUI() {}

void ContactUI::run() {
    unsigned char choice;
    do {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore(); // Ignore the newline character
        
        try { // Exception handling
            switch (choice) {
                case '1':
                    addContact();
                    break;
                case '2':
                    removeContact();
                    break;
                case '3':
                    displayContacts();
                    break;
                case '4':
                    findContactsByName();
                    break;
                case '5':
                    saveContactsToFile();
                    break;
                case '6':
                    loadContactsFromFile();
                    break;
                case '7':
                    std::cout << "Exiting..." << std::endl;
                    break;
                default:
                    std::cout << "Invalid choice. Please try again." << std::endl;
                    break;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
        
        std::cout << std::endl;
    } while (choice != '7');
}

void ContactUI::displayMenu() {
    std::cout << "Contact Management System" << std::endl;
    std::cout << "1. Add Contact" << std::endl;
    std::cout << "2. Remove Contact" << std::endl;
    std::cout << "3. Display Contacts" << std::endl;
    std::cout << "4. Find Contacts by Name" << std::endl;
    std::cout << "5. Save Contacts to File" << std::endl;
    std::cout << "6. Load Contacts from File" << std::endl;
    std::cout << "7. Exit" << std::endl;
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
        m_contactManager.addContact(BusinessContact(name, phone, email, company));
    } else {
        m_contactManager.addContact(Contact(name, phone, email));
    }
    
    std::cout << "Contact added successfully!" << std::endl;
}

void ContactUI::removeContact() {
    unsigned char index;
    std::cout << "Enter the index of the contact to remove: ";
    std::cin >> index;
    m_contactManager.removeContact(index);
    std::cout << "Contact removed successfully!" << std::endl;
}

void ContactUI::displayContacts() {
    m_contactManager.displayContacts();
}

void ContactUI::findContactsByName() {
    std::string name;
    std::cout << "Enter the name to search for: ";
    std::getline(std::cin, name);
    auto foundContacts = m_contactManager.findContactsByName(name);
    if (foundContacts.empty()) {
        std::cout << "No contacts found with the given name." << std::endl;
    } else {
        std::cout << "Contacts found:" << std::endl;
        displayContacts(foundContacts);
    }
}

void ContactUI::saveContactsToFile() {
    std::string filename;
    std::cout << "Enter the filename to save contacts: ";
    std::getline(std::cin, filename);
    
    // Save contacts to file asynchronously using a separate thread
    std::thread saveThread([this, filename]() { // Lambda function
        m_contactManager.saveToFile(filename);
        std::cout << "Contacts saved to file successfully!" << std::endl;
    });
    saveThread.detach(); // Detach the thread to run in the background
}

void ContactUI::loadContactsFromFile() {
    std::string filename;
    std::cout << "Enter the filename to load contacts from: ";
    std::getline(std::cin, filename);
    m_contactManager.loadFromFile(filename);
    std::cout << "Contacts loaded from file successfully!" << std::endl;
}

} // namespace contact_management