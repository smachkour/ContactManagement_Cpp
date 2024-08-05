// Contact.cpp
#include "Contact.hpp"
#include <iostream>

namespace contact_management { // Everything in a self-made namespace

// Contact class implementation

Contact::Contact() : m_name(""), m_phone(""), m_email("") {} // Default constructor with member initialization

Contact::Contact(const std::string& name, const std::string& phone, const std::string& email)
    : m_name(name), m_phone(phone), m_email(email) {} // Parameterized constructor with member initialization

Contact::Contact(const Contact& other)
    : m_name(other.m_name), m_phone(other.m_phone), m_email(other.m_email) {} // Copy constructor with member initialization

Contact::~Contact() {} // Destructor

void Contact::setName(const std::string& name) {
    this->m_name = name;
}

std::string Contact::getName() const {
    return m_name;
}

void Contact::setPhone(const std::string& phone) {
    this->m_phone = phone;
}

std::string Contact::getPhone() const {
    return m_phone;
}

void Contact::setEmail(const std::string& email) {
    this->m_email = email;
}

std::string Contact::getEmail() const {
    return m_email;
}

void Contact::displayDetails() const {
    std::cout << "Name: " << m_name << std::endl;
    std::cout << "Phone: " << m_phone << std::endl;
    std::cout << "Email: " << m_email << std::endl;
}

// BusinessContact class implementation

BusinessContact::BusinessContact() : Contact(), m_company("") {} // Default constructor with member initialization and constructor forwarding

BusinessContact::BusinessContact(const std::string& name, const std::string& phone, const std::string& email, const std::string& company)
    : Contact(name, phone, email), m_company(company) {} // Parameterized constructor with member initialization and constructor forwarding

BusinessContact::BusinessContact(const BusinessContact& other)
    : Contact(other), m_company(other.m_company) {} // Copy constructor with member initialization and constructor forwarding

BusinessContact::~BusinessContact() {} // Destructor

void BusinessContact::setCompany(const std::string& company) {
    m_company = company;
}

std::string BusinessContact::getCompany() const {
    return m_company;
}

void BusinessContact::displayDetails() const {
    Contact::displayDetails(); // Call base class function
    std::cout << "Company: " << m_company << std::endl;
}

} // namespace contact_management