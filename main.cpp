#include "ContactGUI.hpp"

int main() {
    contact_management::ContactManager manager;
    contact_management::ContactGUI gui(manager);
    gui.show();
    return Fl::run();
}

