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
        if (!m_stopAutoSave) {  // Check again after sleep
            try {
                if (m_isModified) {
                    saveToFile("auto_save.txt");
                    std::cout << "Auto-saved contacts." << std::endl;
                    m_isModified = false;  // Reset the flag after successful save
                }
            } catch (const std::exception& e) {
                std::cerr << "Auto-save failed: " << e.what() << std::endl;
                // Consider setting m_isModified = true here 
            }
        }
    }
}

void ContactManager::startAutoSave() {
    if (!m_autoSaveThread.joinable()) {  // Only start if not already running
        m_stopAutoSave = false;
        m_autoSaveThread = std::thread(&ContactManager::autoSaveFunction, this);
    }
}

void ContactManager::stopAutoSave() {
    m_stopAutoSave = true;
    if (m_autoSaveThread.joinable()) {
        m_autoSaveThread.join();
    }
}

// Make sure to call this whenever contacts are modified
void ContactManager::setModified() {
    m_isModified = true;
}

void ContactManager::addContact(std::shared_ptr<Contact> contact) {
    m_contacts.push_back(std::move(contact));
    setModified();
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
    setModified();
}

void ContactManager::clearFavoriteContact() {
    m_favoriteContact = nullptr;
        setModified();

}

const Contact* ContactManager::getFavoriteContact() const {
    return m_favoriteContact.get();
}

void ContactManager::setFavoriteContact(unsigned char index) {
    if (index >= m_contacts.size()) {
        throw std::out_of_range("Invalid contact index");
    }
    m_favoriteContact = m_contacts[index];
    setModified();
}

void ContactManager::displayAllContacts() const { // Renamed to avoid confusion
    displayContacts(m_contacts);
}

void ContactManager::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Unable to open file for writing");
    }

    for (const auto& contact : m_contacts) {
        file << contact->getName() << std::endl;
        file << contact->getPhone() << std::endl;
        file << contact->getEmail() << std::endl;
        
        // Check if the contact is a BusinessContact
        if (const auto* businessContact = dynamic_cast<const BusinessContact*>(contact.get())) {
            file << businessContact->getCompany() << std::endl;
        } else {
            file << "N/A" << std::endl; // Write N/A for regular contacts
        }
        file << std::endl;
    }
    m_isModified = false;
}

void ContactManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Unable to open file for reading");
    }

    m_contacts.clear(); // Clear existing contacts before loading

    std::string name, phone, email, company;
    while (std::getline(file, name) && std::getline(file, phone) && std::getline(file, email) && std::getline(file, company)) {
        if (company != "N/A") {
            m_contacts.push_back(std::make_shared<BusinessContact>(name, phone, email, company));
        } else {
            m_contacts.push_back(std::make_shared<Contact>(name, phone, email));
        }
        
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the empty line
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

void ContactManager::exportToJson(const std::string& filename) const {
    json j;
    for (const auto& contact : m_contacts) {
        json contactJson;
        contactJson["name"] = contact->getName();
        contactJson["phone"] = contact->getPhone();
        contactJson["email"] = contact->getEmail();
        
        if (const auto* businessContact = dynamic_cast<const BusinessContact*>(contact.get())) {
            contactJson["company"] = businessContact->getCompany();
        } else {
            contactJson["company"] = "N/A";
        }
        
        j["contacts"].push_back(contactJson);
    }
    
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Unable to open file for writing");
    }
    file << std::setw(4) << j << std::endl;
}

void ContactManager::importFromJson(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Unable to open file for reading");
    }
    
    json j;
    file >> j;
    
    m_contacts.clear(); // Clear existing contacts before importing

    for (const auto& contactJson : j["contacts"]) {
        std::string name = contactJson["name"];
        std::string phone = contactJson["phone"];
        std::string email = contactJson["email"];
        std::string company = contactJson["company"];
        
        if (company != "N/A") {
            m_contacts.push_back(std::make_shared<BusinessContact>(name, phone, email, company));
        } else {
            m_contacts.push_back(std::make_shared<Contact>(name, phone, email));
        }
    }

    setModified();
    m_isSorted = false;
}

const std::vector<std::shared_ptr<Contact>>& ContactManager::getAllContacts() const {
    return m_contacts;
}
} // namespace contact_management