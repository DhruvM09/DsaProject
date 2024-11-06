#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

class contactNode {
public:
    string Name;
    string number;
    string email;
    contactNode* left;
    contactNode* right;

    contactNode(string name, string num, string mail = "") {
        Name = name;
        number = num;
        email = mail;
        left = right = nullptr;
    }
};

void printContact(contactNode* node) {
    if (node) {
        cout << "---------------------------------\n";
        cout << "Contact Info:\n";
        cout << "Name: " << node->Name << '\n';
        cout << "Contact Number: " << node->number << '\n';
        cout << "Email: " << (node->email.empty() ? "N/A" : node->email) << '\n';
        cout << "---------------------------------\n";
    }
}

class ConatactBinarySearchTree {
public:
    contactNode* head;

    ConatactBinarySearchTree() {
        head = nullptr;
    }

    void insert(contactNode*& newNode) {
        if (!head) {
            head = newNode;
            return;
        }

        contactNode* current = head;
        contactNode* parent = nullptr;

        while (current != nullptr) {
            parent = current;
            if (newNode->Name < current->Name) {
                current = current->left;
            } else if (newNode->Name > current->Name) {
                current = current->right;
            } else {
                current = current->right;
            }
        }

        if (newNode->Name < parent->Name) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }
    }

    contactNode* searchByName(const string& key) {
        contactNode* currentNode = head;
        while (currentNode) {
            if (key < currentNode->Name) {
                currentNode = currentNode->left;
            } else if (key > currentNode->Name) {
                currentNode = currentNode->right;
            } else {
                return currentNode;
            }
        }
        cout << "Contact not found.\n";
        return nullptr;
    }

    void dfs(contactNode* current) {
        if (!current) return;
        printContact(current);
        if (current->left) dfs(current->left);
        if (current->right) dfs(current->right);
    }
    void traverse(contactNode* current , vector<string> &lines) {
        if (!current) return;
        string currentLine = current->Name + " " + current->number + " " + current->email;
        lines.push_back(currentLine);
        if (current->left) traverse(current->left,lines);
        if (current->right) traverse(current->right,lines);
    }


    contactNode* getSuccessor(contactNode* curr)
{
    curr = curr->right;
    while (curr != NULL && curr->left != NULL)
        curr = curr->left;
    return curr;
}

   contactNode* delNode(contactNode* root, string x)
{

    // Base case
    if (root == NULL)
        return root;

    // If key to be searched is in a subtree
    if (root->Name> x)
        root->left = delNode(root->left, x);
    else if (root->Name < x)
        root->right = delNode(root->right, x);

    // If root matches with the given key
    else {

        // Cases when root has 0 children
        // or only right child
        if (root->left == NULL) {
            contactNode* temp = root->right;
            delete root;
            return temp;
        }

        // When root has only left child
        if (root->right == NULL) {
            contactNode* temp = root->left;
            delete root;
            return temp;
        }

        // When both children are present
        contactNode* succ = getSuccessor(root);
        root->Name = succ->Name;
        root->right = delNode(root->right, succ->Name);
    }
    return root;
} 
};

void LoadContacts(ConatactBinarySearchTree& tree) {
    ifstream Contacts("contacts.txt");
    if (!Contacts) {
        cout << "Error opening file.\n";
        return;
    }

    string currentLine;
    while (getline(Contacts, currentLine)) {
        stringstream ss(currentLine);
        string name, number, email;
        ss >> name >> number;
        getline(ss, email);
        if (!email.empty()) {
            email = email.substr(1);  // Remove leading space
        }
        contactNode* node = new contactNode(name, number, email);
        tree.insert(node);
    }
    Contacts.close();
}
void updateContacts(ConatactBinarySearchTree &tree){
    vector<string> lines;
    tree.traverse(tree.head,lines); 
    ofstream temp("temp.txt");
    for(int i = 0 ; i < lines.size();i++){
        string line = lines[i];
        temp << line<<'\n';
    }
    temp.close();
    remove("contacts.txt");
    rename("temp.txt", "contacts.txt");
}

int main() {
    ConatactBinarySearchTree ContactTree;
    LoadContacts(ContactTree);

    bool Running = true;
    while (Running) {
        cout << "\n===========================\n";
        cout << " Contact Management System \n";
        cout << "===========================\n";
        cout << "1. Create new contact\n";
        cout << "2. Edit existing contact\n";
        cout << "3. Search contact\n";
        cout << "4. Delete contact\n";
        cout << "5. See all contacts\n";
        cout << "6. Exit\n";
        cout << "===========================\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                string name, number, email;
                cout << "Enter Name: ";
                cin >> name;
                cout << "Enter Number: ";
                cin >> number;
                cout << "Enter Email (optional, press enter to skip): ";
                cin.ignore();
                getline(cin, email);
                contactNode* node = new contactNode(name, number, email);
                ContactTree.insert(node);
                break;
            }
            case 2: {
                cout << "Enter which contact to edit: ";
                string name;
                cin >> name;
                contactNode* requiredNode = ContactTree.searchByName(name);
                if (!requiredNode) break;

                int c;
                cout << "1. Change Name\n";
                cout << "2. Change Number\n";
                cout << "3. Change Email\n";
                cout << "Select what you want to change: ";
                cin >> c;

                string newName, newNumber, newEmail;
                switch (c) {
                    case 1:
                        cout << "Enter New Name: ";
                        cin >> newName;
                        requiredNode->Name = newName;
                        break;
                    case 2:
                        cout << "Enter New Number: ";
                        cin >> newNumber;
                        requiredNode->number = newNumber;
                        break;
                    case 3:
                        cout << "Enter New Email: ";
                        cin.ignore();
                        getline(cin, newEmail);
                        requiredNode->email = newEmail;
                        break;
                    default:
                        cout << "Invalid choice.\n";
                        break;
                }
                break;
            }
            case 3: {
                cout << "Enter Name: ";
                string name;
                cin >> name;
                contactNode* result = ContactTree.searchByName(name);
                if (result) {
                    printContact(result);
                }
                break;
            }
            case 4: {
                cout << "Enter which contact to delete: ";
                string name;
                cin >> name;
                contactNode* targetNode = ContactTree.searchByName(name);
                if (targetNode) {
                   ContactTree.head = ContactTree.delNode(ContactTree.head , name);
                    cout << "Contact deleted successfully.\n";
                }
                break;
            }
            case 5:
                ContactTree.dfs(ContactTree.head);
                break;
            case 6:
                Running = false;
                cout << "Exiting... Goodbye!\n";
                break;
            default:
                cout << "Invalid option. Please try again.\n";
                break;
        }
    }

    updateContacts(ContactTree);
    return 0;
}
