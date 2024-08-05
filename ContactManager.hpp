// ContactManager.h
#ifndef CONTACT_MANAGER_H
#define CONTACT_MANAGER_H

#include "Contact.hpp"
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
#include <functional>
#include <thread>
#include <atomic>
#include <chrono>

namespace contact_management { // Everything in a self-made namespace

class ContactManager {
public:
    // Default constructor
    ContactManager();
    
    // Parameterized constructor
    ContactManager(const std::string& filename); // Const reference for function parameter
    
    // Destructor
    ~ContactManager();

    // Add a new contact
    void addContact(const Contact& contact); // Const reference for function parameter
    
    // Remove a contact by index
    void removeContact(unsigned char index); // Unsigned char for better memory efficiency
    
    // Display all contacts
    void displayAllContacts() const; // Renamed to avoid confusion
    
    // Save contacts to a file
    void saveToFile(const std::string& filename) const; // Const reference for function parameter and const member function
    
    // Load contacts from a file
    void loadFromFile(const std::string& filename); // Const reference for function parameter

    // Find contacts by name
    std::vector<std::shared_ptr<Contact>> findContactsByName(const std::string& name) const; // Const reference for function parameter and const member function

// New function to filter contacts
    std::vector<std::shared_ptr<Contact>> filterContacts(const std::function<bool(const Contact&)>& filter) const;

    // New function to get contact count
    size_t getContactCount() const { return m_contacts.size(); }

        // New methods for favorite contact
    void setFavoriteContact(unsigned char index);
    void clearFavoriteContact();
    const Contact* getFavoriteContact() const;

    


private:
    std::vector<std::shared_ptr<Contact>> m_contacts; // Member variable using a container class and dynamic memory allocation
    mutable bool m_isModified;  // New bool to track if contacts have been modified
    bool m_isLoaded;    // New bool to track if contacts have been loaded from a file
    bool m_isSorted;    // New bool to track if contacts are sorted
    std::shared_ptr<Contact> m_favoriteContact;  // New member variable

        std::thread m_autoSaveThread;
    std::atomic<bool> m_stopAutoSave;
    void autoSaveFunction();
    void startAutoSave();
    void stopAutoSave();
};

// Template function for displaying a container of contacts
template<typename T>
void displayContacts(const T& contacts) {
    for (const auto& contact : contacts) {
        contact->displayDetails();
        std::cout << std::endl;
    }
}

} // namespace contact_management

#endif // CONTACT_MANAGER_H