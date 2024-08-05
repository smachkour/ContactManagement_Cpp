// Contact.h
#ifndef CONTACT_H
#define CONTACT_H

#include <string>
#include <memory>

namespace contact_management { // Everything in a self-made namespace

class Contact {
public:
    // Default constructor
    Contact();
    
    // Parameterized constructor
    Contact(const std::string& name, const std::string& phone, const std::string& email);
    
    // Copy constructor
    Contact(const Contact& other);
    
    // Destructor
    virtual ~Contact();

    // Getter and setter for name
    void setName(const std::string& name); // Const reference for function parameter
    std::string getName() const; // Const member function

    // Getter and setter for phone
    void setPhone(const std::string& phone); // Const reference for function parameter
    std::string getPhone() const; // Const member function

    // Getter and setter for email
    void setEmail(const std::string& email); // Const reference for function parameter
    std::string getEmail() const; // Const member function

    // Virtual function for displaying contact details (dynamic polymorphism)
    virtual void displayDetails() const;


    // New function to create a shared_ptr of Contact
    static std::shared_ptr<Contact> create(const std::string& name, const std::string& phone, const std::string& email) {
        return std::make_shared<Contact>(name, phone, email);
    }

private:
    std::string m_name; // Member variable
    std::string m_phone; // Member variable
    std::string m_email; // Member variable
};

// BusinessContact class derived from Contact (dynamic polymorphism)
class BusinessContact : public Contact {
public:
    // Default constructor
    BusinessContact();
    
    // Parameterized constructor
    BusinessContact(const std::string& name, const std::string& phone, const std::string& email, const std::string& company);
    
    // Copy constructor
    BusinessContact(const BusinessContact& other);
    
    // Destructor
    ~BusinessContact() override;

    // Getter and setter for company
    void setCompany(const std::string& company); // Const reference for function parameter
    std::string getCompany() const; // Const member function

    // Override displayDetails function (dynamic polymorphism)
    void displayDetails() const override;

        // New function to create a shared_ptr of BusinessContact
    static std::shared_ptr<BusinessContact> create(const std::string& name, const std::string& phone, const std::string& email, const std::string& company) {
        return std::make_shared<BusinessContact>(name, phone, email, company);
    }


private:
    std::string m_company; // Member variable
};

} // namespace contact_management

#endif // CONTACT_H