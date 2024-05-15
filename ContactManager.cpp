// ContactManager.cpp
#include "ContactManager.hpp"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <thread>

namespace contact_management { // Everything in a self-made namespace

ContactManager::ContactManager() {}

ContactManager::ContactManager(const std::string& filename) { // Const reference for function parameter
    loadFromFile(filename);
}

ContactManager::~ContactManager() {}

void ContactManager::addContact(const Contact& contact) { // Const reference for function parameter
    m_contacts.push_back(std::make_shared<Contact>(contact)); // Dynamic memory allocation
}

void ContactManager::removeContact(unsigned char index) { // Unsigned char for better memory efficiency
    if (index >= m_contacts.size()) {
        throw std::out_of_range("Invalid contact index"); // Exception handling
    }
    m_contacts.erase(m_contacts.begin() + index);
}

void ContactManager::displayContacts() const { // Const member function
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

} // namespace contact_management