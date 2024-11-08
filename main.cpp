#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

class TrieNode{
    public:
    TrieNode * child[52];
    bool isWord;
    TrieNode(){
        isWord = false;
        for(auto &a : child) a = nullptr;
    }
};

//cotnactNode for BST
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
//funtion to print a single contact
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
//BST class
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
//Trie class
class Trie{
   TrieNode * root; 
   public:
    Trie(){
        root = new TrieNode();
    }
    void insert(string s){
        TrieNode * cur = root;
        for(auto &c:s){
            int i = 0;
            if(c < 'a'){
                i = 26 + (c - 'A');
            } 
            else{
                i = c - 'a';
            }
                
            if(!cur->child[i])cur->child[i]= new TrieNode();
            cur = cur->child[i];
        }
        cur->isWord = true;
    }
    void dfs(TrieNode *node , string cur , ConatactBinarySearchTree &tree){

        if(node->isWord && tree.searchByName(cur))cout << cur << '\n';
        for(int i = 0; i < 52;i++){
            auto child = node->child[i];
            if(child){
            if(i <= 25)
            cur += ('a' + i);
            else
            cur += ('A' + (i-26));

            dfs(child , cur,tree);
            }
        }
        
    }
    bool search(string key ,bool prefix, ConatactBinarySearchTree & tree){
        TrieNode *cur = root;
        for(auto &c :key){
            int i;
            if(c < 'a'){
                i = 26 + (c - 'A');
            } 
            else{
                i = c - 'a';
            }
            if(!cur->child[i]) return false;
            cur = cur->child[i];
        }
            dfs(cur,key,tree);
            return true;
        }

    bool startsWith(string prefix , ConatactBinarySearchTree &tree){
        return search(prefix , true,tree);
    }
};
//function to load contacts from file to BST at execution 
void LoadContacts(ConatactBinarySearchTree& tree,Trie &t) {
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
        
        t.insert(node->Name);
    }
    Contacts.close();
}
//function to update contacts file from bst after completion of program
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
//TrieNode class

int main() {
    ConatactBinarySearchTree ContactTree;
    Trie trieNames;
    LoadContacts(ContactTree,trieNames);
    bool login = true;
    while (!login)
    {
        cout << "1.login\n";
        cout << "2.create account\n";
        int choice;
        cin >> choice;
        switch (choice)
        {
        case 1:
            
            break;
        
        default:
            break;
        }
    }
    
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
        cout << "6. Search by prefix\n";
        cout << "7. Exit\n";
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
                trieNames.insert(node->Name);
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
                else{
                    cout << "Contact not found\n";
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
            {
                cout << "Enter Prefix:";
                string pre;
                cin >> pre;
                trieNames.startsWith(pre,ContactTree);
                break;
            }
            case 7:
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
