// ContactUI.h
#ifndef CONTACT_UI_H
#define CONTACT_UI_H

#include "ContactManager.hpp"

namespace contact_management { // Everything in a self-made namespace

class ContactUI {
public:
    ContactUI();
    void run();

private:
    void displayMenu();
    void addContact();
    void removeContact();
    void displayContacts();
    void findContactsByName();
    void saveContactsToFile();
    void loadContactsFromFile();

    ContactManager m_contactManager; // Member variable
};

} // namespace contact_management

#endif // CONTACT_UI_H