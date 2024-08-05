// ContactUI.hpp
#ifndef CONTACT_UI_H
#define CONTACT_UI_H

#include "ContactManager.hpp"
#include <functional>

namespace contact_management {

class ContactUI {
public:
    ContactUI();
    void run();

    // Friend function declaration
    friend void displayContactCount(const ContactUI& ui);

private:
    void displayMenu();
    void addContact();
    void removeContact();
    void displayContacts();
    void displayFilteredContacts(const std::vector<std::shared_ptr<Contact>>& contacts);

    void findContactsByName();
    void saveContactsToFile();
    void loadContactsFromFile();
    
    // New function with default parameter
    void filterContacts(const std::function<bool(const Contact&)>& filter = [](const Contact&) { return true; });

    // Inline function
    inline bool isValidChoice(char choice) const {
        return choice >= '1' && choice <= '7';
    }

    ContactManager m_contactManager;
    bool m_isRunning;  // New bool member
};

// Friend function declaration (inside the namespace)
void displayContactCount(const ContactUI& ui);

} // namespace contact_management

#endif // CONTACT_UI_H