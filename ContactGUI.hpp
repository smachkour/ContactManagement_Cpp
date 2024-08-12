// ContactGUI.hpp
#ifndef CONTACT_GUI_H
#define CONTACT_GUI_H

#include "ContactManager.hpp"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_File_Chooser.H>
#include <memory>

namespace contact_management {

class ContactGUI {
public:
    ContactGUI(ContactManager& manager);
    void show();

private:
    static void addContactCB(Fl_Widget*, void* userdata);
    static void removeContactCB(Fl_Widget*, void* userdata);
    static void displayContactsCB(Fl_Widget*, void* userdata);
    static void findContactsCB(Fl_Widget*, void* userdata);
    static void saveContactsCB(Fl_Widget*, void* userdata);
    static void loadContactsCB(Fl_Widget*, void* userdata);
    static void setFavoriteContactCB(Fl_Widget*, void* userdata);
    static void displayFavoriteContactCB(Fl_Widget*, void* userdata);

    void addContact();
    void removeContact();
    void displayContacts();
    void findContacts();
    void saveContacts();
    void loadContacts();
    void setFavoriteContact();
    void displayFavoriteContact();

    ContactManager& m_manager;
    std::unique_ptr<Fl_Window> m_window;
    std::unique_ptr<Fl_Input> m_nameInput;
    std::unique_ptr<Fl_Input> m_phoneInput;
    std::unique_ptr<Fl_Input> m_emailInput;
    std::unique_ptr<Fl_Button> m_addButton;
    std::unique_ptr<Fl_Button> m_removeButton;
    std::unique_ptr<Fl_Button> m_displayButton;
    std::unique_ptr<Fl_Button> m_findButton;
    std::unique_ptr<Fl_Button> m_saveButton;
    std::unique_ptr<Fl_Button> m_loadButton;
    std::unique_ptr<Fl_Button> m_setFavoriteButton;
    std::unique_ptr<Fl_Button> m_displayFavoriteButton;
    std::unique_ptr<Fl_Text_Display> m_display;
    std::unique_ptr<Fl_Text_Buffer> m_buffer;
};

} // namespace contact_management

#endif // CONTACT_GUI_H