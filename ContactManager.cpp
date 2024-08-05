// ContactManager.cpp
#include "ContactManager.hpp"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <thread>

namespace contact_management { // Everything in a self-made namespace

ContactManager::ContactManager() 
    : m_isModified(false), m_isLoaded(false), m_isSorted(true), m_favoriteContact(nullptr), m_stopAutoSave(false) {
    startAutoSave();
}

ContactManager::ContactManager(const std::string& filename) 
    : m_isModified(false), m_isLoaded(false), m_isSorted(false), m_favoriteContact(nullptr) {
    loadFromFile(filename);
}

ContactManager::~ContactManager() {
    stopAutoSave();
}

void ContactManager::autoSaveFunction() {
    while (!m_stopAutoSave) {
        std::this_thread::sleep_for(std::chrono::seconds(30));  // Auto-save every 30 seconds
        if (m_isModified) {
            saveToFile("auto_save.txt");
            std::cout << "Auto-saved contacts." << std::endl;
        }
    }
}

void ContactManager::startAutoSave() {
    m_stopAutoSave = false;
    m_autoSaveThread = std::thread(&ContactManager::autoSaveFunction, this);
}

void ContactManager::stopAutoSave() {
    m_stopAutoSave = true;
    if (m_autoSaveThread.joinable()) {
        m_autoSaveThread.join();
    }
}

void ContactManager::addContact(const Contact& contact) {
    m_contacts.push_back(std::make_shared<Contact>(contact));
    m_isModified = true;
    m_isSorted = false;
}

// In the removeContact method, add this check:
void ContactManager::removeContact(unsigned char index) {
    if (index >= m_contacts.size()) {
        throw std::out_of_range("Invalid contact index");
    }
    if (m_contacts[index] == m_favoriteContact) {
        m_favoriteContact = nullptr;  // Clear favorite if it's being removed
    }
    m_contacts.erase(m_contacts.begin() + index);
    m_isModified = true;
}

void ContactManager::clearFavoriteContact() {
    m_favoriteContact = nullptr;
}

const Contact* ContactManager::getFavoriteContact() const {
    return m_favoriteContact.get();
}

void ContactManager::setFavoriteContact(unsigned char index) {
    if (index >= m_contacts.size()) {
        throw std::out_of_range("Invalid contact index");
    }
    m_favoriteContact = m_contacts[index];
}

void ContactManager::displayAllContacts() const { // Renamed to avoid confusion
    displayContacts(m_contacts);
}

void ContactManager::saveToFile(const std::string& filename) const { // Const reference for function parameter and const member function
    std::ofstream file(filename); // File I/O
    if (!file) {
        throw std::runtime_error("Unable to open file for writing"); // Exception handling
    }

    for (const auto& contact : m_contacts) { // Range-based for loop
        file << contact->getName() << std::endl;
        file << contact->getPhone() << std::endl;
        file << contact->getEmail() << std::endl;
        
        // Check if the contact is a BusinessContact (dynamic polymorphism ). If so, save the company field as well.
        if (const BusinessContact* businessContact = dynamic_cast<const BusinessContact*>(contact.get())) {
            file << businessContact->getCompany() << std::endl;
        }
        file << std::endl;
    }
        m_isModified = false;
}

void ContactManager::loadFromFile(const std::string& filename) { // Const reference for function parameter
    std::ifstream file(filename); // File I/O
    if (!file) {
        throw std::runtime_error("Unable to open file for reading"); // Exception handling
    }

    std::string name;
    std::string phone;
    std::string email;
    std::string company;

    while (std::getline(file, name)) { // String handling
        std::getline(file, phone);
        std::getline(file, email);
        std::getline(file, company);
        
        if (!company.empty()) {
            m_contacts.push_back(std::make_shared<BusinessContact>(name, phone, email, company)); // Dynamic memory allocation
        } else {
            m_contacts.push_back(std::make_shared<Contact>(name, phone, email)); // Dynamic memory allocation
        }
        
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore any extra newline characters
    }

    m_isLoaded = true;
    m_isModified = false;
    m_isSorted = false;
}

std::vector<std::shared_ptr<Contact>> ContactManager::findContactsByName(const std::string& name) const { // Const reference for function parameter and const member function
    std::vector<std::shared_ptr<Contact>> foundContacts;
    for (const auto& contact : m_contacts) { // Range-based for loop
        if (contact->getName() == name) {
            foundContacts.push_back(contact);
        }
    }
    return foundContacts;
}

std::vector<std::shared_ptr<Contact>> ContactManager::filterContacts(const std::function<bool(const Contact&)>& filter) const {
    std::vector<std::shared_ptr<Contact>> filteredContacts;
    std::copy_if(m_contacts.begin(), m_contacts.end(), std::back_inserter(filteredContacts),
                 [&filter](const std::shared_ptr<Contact>& contact) {
                     return filter(*contact);
                 });
    return filteredContacts;
}

} // namespace contact_management