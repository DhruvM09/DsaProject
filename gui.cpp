#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <string>
#include <iostream>
#include <vector>

class contactNode {
public:
    std::string name;
    std::string phone;
    contactNode* left;
    contactNode* right;

    contactNode(const std::string &name, const std::string &phone)
        : name(name), phone(phone), left(nullptr), right(nullptr) {}
};

class ConatactBinarySearchTree {
public:
    contactNode* head;

    ConatactBinarySearchTree() : head(nullptr) {}

    void insert(const std::string &name, const std::string &phone) {
        contactNode* newNode = new contactNode(name, phone);
        if (!head) {
            head = newNode;
            return;
        }

        contactNode* current = head;
        contactNode* parent = nullptr;

        while (current) {
            parent = current;
            if (name < current->name) {
                current = current->left;
            } else if (name > current->name) {
                current = current->right;
            } else {
                return; // Contact already exists
            }
        }

        if (name < parent->name) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }
    }

    contactNode* searchByName(const std::string &key) {
        contactNode* currentNode = head;
        while (currentNode) {
            if (key < currentNode->name) {
                currentNode = currentNode->left;
            } else if (key > currentNode->name) {
                currentNode = currentNode->right;
            } else {
                return currentNode;
            }
        }
        return nullptr;
    }

    void traverse(contactNode* current, std::vector<std::string> &lines) {
        if (!current) return;
        std::string currentLine = current->name + " - " + current->phone;
        lines.push_back(currentLine);
        traverse(current->left, lines);
        traverse(current->right, lines);
    }
};

class ContactManager {
public:
    void add_contact(const std::string &name, const std::string &phone) {
        bst.insert(name, phone);
        update_display();
    }

    void search_contact(const std::string &name) {
        display_buffer->text(""); // Clear display
        contactNode* contact = bst.searchByName(name);
        if (contact) {
            std::string result = contact->name + " - " + contact->phone + "\n";
            display_buffer->append(result.c_str());
        } else {
            display_buffer->append("Contact not found\n");
        }
    }

    void view_all_contacts() {
        display_buffer->text(""); // Clear display
        std::vector<std::string> lines;
        bst.traverse(bst.head, lines);
        for (const auto &line : lines) {
            display_buffer->append((line + "\n").c_str());
        }
    }

    void set_display_buffer(Fl_Text_Buffer *buffer) {
        display_buffer = buffer;
        update_display();
    }

private:
    ConatactBinarySearchTree bst;
    Fl_Text_Buffer *display_buffer;

    void update_display() {
        display_buffer->text(""); // Clear display
        std::vector<std::string> lines;
        bst.traverse(bst.head, lines);
        for (const auto &line : lines) {
            display_buffer->append((line + "\n").c_str());
        }
    }
};

// Global instance of ContactManager
ContactManager contact_manager;

void add_contact_callback(Fl_Widget*, void* data) {
    auto inputs = static_cast<std::pair<Fl_Input*, Fl_Input*>*>(data);
    Fl_Input* name_input = inputs->first;
    Fl_Input* phone_input = inputs->second;
    contact_manager.add_contact(name_input->value(), phone_input->value());
    name_input->value("");
    phone_input->value("");
}

void search_contact_callback(Fl_Widget*, void* data) {
    Fl_Input* search_input = static_cast<Fl_Input*>(data);
    contact_manager.search_contact(search_input->value());
}

void view_all_contacts_callback(Fl_Widget*, void*) {
    contact_manager.view_all_contacts();
}

int main(int argc, char **argv) {
    Fl_Window *window = new Fl_Window(800, 600, "Contact Manager");

    // Title Box
    Fl_Box *box = new Fl_Box(300, 20, 200, 50, "Contact Manager");
    box->box(FL_UP_BOX);
    box->labelfont(FL_BOLD + FL_ITALIC);
    box->labelsize(22);
    box->labeltype(FL_SHADOW_LABEL);

    // Name Input
    Fl_Input *name_input = new Fl_Input(150, 100, 200, 30, "Name:");
    
    // Phone Input
    Fl_Input *phone_input = new Fl_Input(150, 150, 200, 30, "Phone:");

    // Add Button
    Fl_Button *add_button = new Fl_Button(400, 100, 100, 30, "Add Contact");
    auto add_data = std::make_pair(name_input, phone_input);
    add_button->callback(add_contact_callback, &add_data);

    // Search Input
    Fl_Input *search_input = new Fl_Input(150, 200, 200, 30, "Search:");
    
    // Search Button
    Fl_Button *search_button = new Fl_Button(400, 200, 100, 30, "Search");
    search_button->callback(search_contact_callback, search_input);

    // View All Button
    Fl_Button *view_all_button = new Fl_Button(400, 250, 100, 30, "View All");
    view_all_button->callback(view_all_contacts_callback, nullptr);

    // Display for contacts
    Fl_Text_Display *display = new Fl_Text_Display(150, 300, 500, 200);
    Fl_Text_Buffer *display_buffer = new Fl_Text_Buffer();
    display->buffer(display_buffer);

    // Set display buffer for the contact manager
    contact_manager.set_display_buffer(display_buffer);

    window->end();
    window->show(argc, argv);

    return Fl::run();
}

