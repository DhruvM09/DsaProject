#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <unordered_map>

using namespace std;

struct ContactInfo {
    string phoneNumber;
    string email;
    string address;

    ContactInfo(const string& phone = "", const string& mail = "", const string& addr = "")
        : phoneNumber(phone), email(mail), address(addr) {}
};

class ContactNode {
public:
    string name;
    ContactInfo info;
    ContactNode* left;
    ContactNode* right;

    ContactNode(const string& n, const ContactInfo& i)
        : name(n), info(i), left(nullptr), right(nullptr) {}
};

class ContactTree {
public:
    ContactNode* root;

    ContactTree() : root(nullptr) {}

    void insert(const string& name, const ContactInfo& info) {
        root = insertNode(root, name, info);
    }

    ContactNode* search(const string& name) {
        return searchNode(root, name);
    }

    bool deleteContact(const string& name, const ContactInfo& info) {
        return deleteNode(root, name, info);
    }

    void printAllContacts() {
        printContacts(root);
    }

    void saveContacts(const string& filename) {
        vector<string> lines;
        saveContactsHelper(root, lines);
        saveToFile(filename, lines);
    }

    void loadContacts(const string& filename) {
        vector<string> lines;
        loadFromFile(filename, lines);
        loadContactsHelper(root, lines);
    }

private:
    ContactNode* insertNode(ContactNode* node, const string& name, const ContactInfo& info) {
        if (!node) {
            return new ContactNode(name, info);
        }

        if (name < node->name) {
            node->left = insertNode(node->left, name, info);
        } else if (name > node->name) {
            node->right = insertNode(node->right, name, info);
        } else {
            node->info = info; // If name already exists, update contact info
        }
        return node;
    }

    ContactNode* searchNode(ContactNode* node, const string& name) {
        if (!node || node->name == name) {
            return node;
        }
        if (name < node->name) {
            return searchNode(node->left, name);
        } else {
            return searchNode(node->right, name);
        }
    }

    bool deleteNode(ContactNode*& node, const string& name, const ContactInfo& info) {
        if (!node) return false;

        if (name < node->name) {
            return deleteNode(node->left, name, info);
        } else if (name > node->name) {
            return deleteNode(node->right, name, info);
        } else {
            // Name matches, delete the contact if info matches
            if (node->info.phoneNumber == info.phoneNumber &&
                node->info.email == info.email &&
                node->info.address == info.address) {
                ContactNode* temp = node;
                if (!node->left) {
                    node = node->right;
                } else if (!node->right) {
                    node = node->left;
                } else {
                    temp = findMinNode(node->right);
                    node->name = temp->name;
                    node->info = temp->info;
                    deleteNode(node->right, temp->name, temp->info);
                }
                delete temp;
                return true;
            }
            return false;
        }
    }

    ContactNode* findMinNode(ContactNode* node) {
        while (node->left) {
            node = node->left;
        }
        return node;
    }

    void printContacts(ContactNode* node) {
        if (node) {
            cout << "Name: " << node->name << endl;
            cout << "Phone: " << node->info.phoneNumber << endl;
            cout << "Email: " << node->info.email << endl;
            cout << "Address: " << node->info.address << endl;
            cout << "---" << endl;
            printContacts(node->left);
            printContacts(node->right);
        }
    }

    void saveContactsHelper(ContactNode* node, vector<string>& lines) {
        if (node) {
            string line = node->name + " " + node->info.phoneNumber + " " + node->info.email + " " + node->info.address;
            lines.push_back(line);
            saveContactsHelper(node->left, lines);
            saveContactsHelper(node->right, lines);
        }
    }

    void loadContactsHelper(ContactNode*& node, const vector<string>& lines) {
        for (const auto& line : lines) {
            istringstream iss(line);
            string name, phone, email, address;
            iss >> name >> phone;
            getline(iss, email, ' ');
            getline(iss, address);
            insert(name, ContactInfo(phone, email, address));
        }
    }

    void saveToFile(const string& filename, const vector<string>& lines) {
        ofstream file(filename);
        for (const auto& line : lines) {
            file << line << "\n";
        }
        file.close();
    }

    void loadFromFile(const string& filename, vector<string>& lines) {
        ifstream file(filename);
        if (file) {
            string line;
            while (getline(file, line)) {
                lines.push_back(line);
            }
            file.close();
        } else {
            cout << "Error opening file: " << filename << endl;
        }
    }
};

struct TrieNode {
    unordered_map<char, TrieNode*> children;
    bool isEndOfWord;
    int cntEndWith = 0;
    int cntPrefix = 0;

    bool containsKey(char ch) {
        return children.find(ch) != children.end();
    }
    
    void put(char ch, TrieNode* node) {
        children[ch] = node;
    }

    TrieNode* get(char ch) {
        return children[ch];
    }

    void increaseEnd() {
        cntEndWith++;
    }

    void increasePrefix() {
        cntPrefix++;
    }

    void deleteEnd() {
        cntEndWith--;
    }

    void reducePrefix() {
        cntPrefix--;
    }

    int getEnd() {
        return cntEndWith;
    }

    int getPrefix() {
        return cntPrefix;
    }
};

class Trie {
private:
    TrieNode* root;

public:
    Trie() {
        root = new TrieNode();
    }

    void insert(string word) {
        TrieNode* node = root;
        for (char ch : word) {
            if (!node->containsKey(ch)) {
                node->put(ch, new TrieNode());
            }
            node = node->get(ch);
            node->increasePrefix();
        }
        node->increaseEnd();
    }

    bool search(string word) {
        TrieNode* node = root;
        for (char ch : word) {
            if (!node->containsKey(ch)) {
                return false;
            }
            node = node->get(ch);
        }
        return node->getEnd() > 0;
    }

    bool startsWith(string prefix) {
        TrieNode* node = root;
        for (char ch : prefix) {
            if (!node->containsKey(ch)) {
                return false;
            }
            node = node->get(ch);
        }
        return true;
    }

    void erase(string word) {
        TrieNode* node = root;
        for (char ch : word) {
            if (node->containsKey(ch)) {
                node = node->get(ch);
                node->reducePrefix();
            } else {
                return; // Word does not exist in the Trie
            }
        }
        node->deleteEnd();
    }
};

struct BKTreeNode {
    string word;
    unordered_map<int, BKTreeNode*> children;

    BKTreeNode(const string& w) : word(w) {}
};

class BKTree {
public:
    BKTree() : root(nullptr) {}

    void insert(const string& word) {
        if (!root) {
            root = new BKTreeNode(word);
            return;
        }
        insertNode(root, word);
    }

    vector<string> search(const string& query, int tolerance) {
        vector<string> results;
        if (root) {
            searchNode(root, query, tolerance, results);
        }
        return results;
    }

private:
    BKTreeNode* root;

    int editDistance(const string& s1, const string& s2) {
        int len1 = s1.size(), len2 = s2.size();
        vector<vector<int>> dp(len1 + 1, vector<int>(len2 + 1));

        for (int i = 0; i <= len1; i++) dp[i][0] = i;
        for (int j = 0; j <= len2; j++) dp[0][j] = j;

        for (int i = 1; i <= len1; i++) {
            for (int j = 1; j <= len2; j++) {
                if (s1[i - 1] == s2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1];
                } else {
                    dp[i][j] = min({dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + 1});
                }
            }
        }
        return dp[len1][len2];
    }

    void insertNode(BKTreeNode* node, const string& word) {
        int dist = editDistance(node->word, word);
        if (node->children.find(dist) == node->children.end()) {
            node->children[dist] = new BKTreeNode(word);
        } else {
            insertNode(node->children[dist], word);
        }
    }

    void searchNode(BKTreeNode* node, const string& query, int tolerance, vector<string>& results) {
        int dist = editDistance(node->word, query);
        if (dist <= tolerance) {
            results.push_back(node->word);
        }

        for (auto& [key, child] : node->children) {
            if (dist - tolerance <= key && key <= dist + tolerance) {
                searchNode(child, query, tolerance, results);
            }
        }
    }
};

int main() {
    ContactTree contactTree;
    Trie trie;
    BKTree bkTree;

    bool running = true;

    while (running) {
        cout << "1. Add Contact\n";
        cout << "2. Search Contact\n";
        cout << "3. View All Contacts\n";
        cout << "4. Delete Contact\n";
        cout << "5. Load Contacts\n";
        cout << "6. Save Contacts\n";
        cout << "7. Exit\n";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                string name, phone, email, address;
                cout << "Enter Name: ";
                cin >> name;
                cout << "Enter Phone: ";
                cin >> phone;
                cout << "Enter Email: ";
                cin >> email;
                cout << "Enter Address: ";
                cin >> address;

                ContactInfo info(phone, email, address);
                contactTree.insert(name, info);
                trie.insert(name);
                bkTree.insert(name);
                cout << "Contact added.\n";
                break;
            }
            case 2: {
                string name;
                cout << "Enter name to search: ";
                cin >> name;

                ContactNode* contact = contactTree.search(name);
                if (contact) {
                    cout << "Name: " << contact->name << endl;
                    cout << "Phone: " << contact->info.phoneNumber << endl;
                    cout << "Email: " << contact->info.email << endl;
                    cout << "Address: " << contact->info.address << endl;
                } else {
                    cout << "Contact not found.\n";
                }
                break;
            }
            case 3:
                contactTree.printAllContacts();
                break;

            case 4: {
                string name;
                cout << "Enter name of contact to delete: ";
                cin >> name;

                ContactNode* contact = contactTree.search(name);
                if (contact) {
                    contactTree.deleteContact(name, contact->info);
                    trie.erase(name);
                    bkTree.insert(name); // Ensure proper handling here (removal logic)
                    cout << "Contact deleted successfully.\n";
                } else {
                    cout << "Contact not found.\n";
                }
                break;
            }
            case 5: {
                contactTree.loadContacts("contacts.txt");
                cout << "Contacts loaded successfully.\n";
                break;
            }
            case 6:
                contactTree.saveContacts("contacts.txt");
                cout << "Contacts saved successfully.\n";
                break;
            case 7:
                running = false;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
