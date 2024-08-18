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

    void exportContactsToJson();
    void importContactsFromJson();

private:
    std::string ensureJsonExtension(const std::string& filename) const {
        if (filename.size() >= 5 && filename.substr(filename.size() - 5) == ".json") {
            return filename;
        }
        return filename + ".json";
    }
    void displayMenu();
    void addContact();
    void removeContact();
    void displayContacts();
    void displayFilteredContacts(const std::vector<std::shared_ptr<Contact>>& contacts, size_t limit = std::numeric_limits<size_t>::max());

    void findContactsByName();
    void saveContactsToFile();
    void loadContactsFromFile();
    
    // New function with default parameter
    void filterContacts();

inline bool isValidChoice(const std::string& choice) const {
    if (choice.length() == 1) {
        return choice[0] >= '1' && choice[0] <= '9';
    } else if (choice.length() == 2) {
        return choice == "10" || choice == "11" || choice == "12";
    }
    return false;
}

    void setFavoriteContact();
    void displayFavoriteContact();

    ContactManager m_contactManager;
    bool m_isRunning;  // New bool member
};

// Friend function declaration (inside the namespace)
void displayContactCount(const ContactUI& ui);

} // namespace contact_management

#endif // CONTACT_UI_H