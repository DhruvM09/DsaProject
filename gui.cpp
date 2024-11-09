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
    std::string email;
    contactNode* left;
    contactNode* right;

    contactNode(const std::string &name, const std::string &phone, const std::string &email)
        : name(name), phone(phone), email(email), left(nullptr), right(nullptr) {}
};

class ConatactBinarySearchTree {
public:
    contactNode* head;

    ConatactBinarySearchTree() : head(nullptr) {}

    void insert(const std::string &name, const std::string &phone, const std::string &email) {
        contactNode* newNode = new contactNode(name, phone, email);
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
        std::string currentLine = current->name + " - " + current->phone + " - " + current->email;
        lines.push_back(currentLine);
        traverse(current->left, lines);
        traverse(current->right, lines);
    }

    // Delete a contact by name
    void deleteByName(const std::string &name) {
        head = deleteNode(head, name);
    }

    // Edit a contact's name, phone, and email
    void editContact(const std::string &oldName, const std::string &newName, const std::string &newPhone, const std::string &newEmail) {
        contactNode* contact = searchByName(oldName);
        if (contact) {
            contact->name = newName;
            contact->phone = newPhone;
            contact->email = newEmail;
        }
    }

private:
    contactNode* deleteNode(contactNode* root, const std::string &key) {
        if (!root) return root;

        if (key < root->name) {
            root->left = deleteNode(root->left, key);
        } else if (key > root->name) {
            root->right = deleteNode(root->right, key);
        } else {
            // Node to be deleted found
            if (!root->left) {
                contactNode* temp = root->right;
                delete root;
                return temp;
            } else if (!root->right) {
                contactNode* temp = root->left;
                delete root;
                return temp;
            }

            // Node with two children: get the inorder successor (smallest in the right subtree)
            contactNode* temp = minValueNode(root->right);
            root->name = temp->name;
            root->phone = temp->phone;
            root->email = temp->email;
            root->right = deleteNode(root->right, temp->name);
        }
        return root;
    }

    contactNode* minValueNode(contactNode* node) {
        contactNode* current = node;
        while (current && current->left) {
            current = current->left;
        }
        return current;
    }
};

class ContactManager {
public:
    void add_contact(const std::string &name, const std::string &phone, const std::string &email) {
        bst.insert(name, phone, email);
        update_display();
    }

    void search_contact(const std::string &name) {
        display_buffer->text(""); // Clear display
        contactNode* contact = bst.searchByName(name);
        if (contact) {
            std::string result = contact->name + " - " + contact->phone + " - " + contact->email + "\n";
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

    void delete_contact(const std::string &name) {
        bst.deleteByName(name);
        update_display();
    }

    void edit_contact(const std::string &oldName, const std::string &newName, const std::string &newPhone, const std::string &newEmail) {
        bst.editContact(oldName, newName, newPhone, newEmail);
        update_display();
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
    auto inputs = static_cast<std::tuple<Fl_Input*, Fl_Input*, Fl_Input*>*>(data);
    Fl_Input* name_input = std::get<0>(*inputs);
    Fl_Input* phone_input = std::get<1>(*inputs);
    Fl_Input* email_input = std::get<2>(*inputs);
    contact_manager.add_contact(name_input->value(), phone_input->value(), email_input->value());
    name_input->value("");
    phone_input->value("");
    email_input->value("");
}

void search_contact_callback(Fl_Widget*, void* data) {
    Fl_Input* search_input = static_cast<Fl_Input*>(data);
    contact_manager.search_contact(search_input->value());
}

void view_all_contacts_callback(Fl_Widget*, void*) {
    contact_manager.view_all_contacts();
}

void delete_contact_callback(Fl_Widget*, void* data) {
    Fl_Input* delete_input = static_cast<Fl_Input*>(data);
    contact_manager.delete_contact(delete_input->value());
    delete_input->value("");
}

void edit_contact_callback(Fl_Widget*, void* data) {
    auto inputs = static_cast<std::tuple<Fl_Input*, Fl_Input*, Fl_Input*, Fl_Input*>*>(data);
    Fl_Input* old_name_input = std::get<0>(*inputs);
    Fl_Input* new_name_input = std::get<1>(*inputs);
    Fl_Input* new_phone_input = std::get<2>(*inputs);
    Fl_Input* new_email_input = std::get<3>(*inputs);

    contact_manager.edit_contact(old_name_input->value(), new_name_input->value(), new_phone_input->value(), new_email_input->value());

    // Clear inputs after editing
    old_name_input->value("");
    new_name_input->value("");
    new_phone_input->value("");
    new_email_input->value("");
}

int main(int argc, char **argv) {
    // Get screen width and height
    int screen_width = Fl::w();
    int screen_height = Fl::h();

    // Set window size to be almost full screen (90% of the screen)
    int window_width = screen_width * 0.9;
    int window_height = screen_height * 0.9;

    Fl_Window *window = new Fl_Window(window_width, window_height, "Contact Manager");

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

    // Email Input
    Fl_Input *email_input = new Fl_Input(150, 200, 200, 30, "Email:");

    // Add Button
    Fl_Button *add_button = new Fl_Button(400, 100, 100, 30, "Add Contact");
    auto add_data = std::make_tuple(name_input, phone_input, email_input);
    add_button->callback(add_contact_callback, &add_data);

    // Search Input
    Fl_Input *search_input = new Fl_Input(150, 250, 200, 30, "Search:");

    // Search Button
    Fl_Button *search_button = new Fl_Button(400, 250, 100, 30, "Search");
    search_button->callback(search_contact_callback, search_input);

    // View All Button
    Fl_Button *view_all_button = new Fl_Button(400, 300, 100, 30, "View All");
    view_all_button->callback(view_all_contacts_callback, nullptr);

    // Delete Input
    Fl_Input *delete_input = new Fl_Input(150, 350, 200, 30, "Delete Contact:");

    // Delete Button
    Fl_Button *delete_button = new Fl_Button(400, 350, 100, 30, "Delete");
    delete_button->callback(delete_contact_callback, delete_input);

    // Edit Inputs (for editing operation)
    Fl_Input *old_name_input = new Fl_Input(150, 400, 200, 30, "Old Name:");
    Fl_Input *new_name_input = new Fl_Input(150, 450, 200, 30, "New Name:");
    Fl_Input *new_phone_input = new Fl_Input(150, 500, 200, 30, "New Phone:");
    Fl_Input *new_email_input = new Fl_Input(150, 550, 200, 30, "New Email:");

    // Edit Button
    Fl_Button *edit_button = new Fl_Button(400, 450, 100, 30, "Edit Contact");
    auto edit_data = std::make_tuple(old_name_input, new_name_input, new_phone_input, new_email_input);
    edit_button->callback(edit_contact_callback, &edit_data);

    // Display for contacts
    Fl_Text_Display *display = new Fl_Text_Display(150, 600, 500, 200);
    Fl_Text_Buffer *display_buffer = new Fl_Text_Buffer();
    display->buffer(display_buffer);

    // Set display buffer for the contact manager
    contact_manager.set_display_buffer(display_buffer);

    window->end();
    window->show(argc, argv);

    return Fl::run();
}

