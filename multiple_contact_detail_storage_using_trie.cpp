#include <iostream>
#include <map>
#include <string>
#include <vector>

struct ContactInfo {
    std::string phoneNumber;
    std::string email;
    std::string address;

    ContactInfo(const std::string& phone = "", const std::string& mail = "", const std::string& addr = "")
        : phoneNumber(phone), email(mail), address(addr) {}
};

struct TrieNode {
    std::map<char, TrieNode*> children;
    bool isEndOfWord;
    std::vector<ContactInfo> contactList; // Stores multiple contact entries for the same name

    TrieNode() : isEndOfWord(false) {}
};

class Trie {
public:
    Trie() {
        root = new TrieNode();
    }

    // Insert a name with contact info into the trie
    void insert(const std::string& name, const ContactInfo& info) {
        TrieNode* current = root;
        for (char c : name) {
            if (!current->children[c]) {
                current->children[c] = new TrieNode();
            }
            current = current->children[c];
        }
        current->isEndOfWord = true;
        current->contactList.push_back(info); // Add contact to list instead of overwriting
    }

    // Search for a name and return its list of contact info if found
    std::vector<ContactInfo>* search(const std::string& name) {
        TrieNode* current = root;
        for (char c : name) {
            if (!current->children[c]) {
                return nullptr; // Name not found
            }
            current = current->children[c];
        }
        return current->isEndOfWord ? &current->contactList : nullptr;
    }

    // Display all names in alphabetical order with all their contact details
    void displayAllContacts() const {
        std::string currentName;
        displayContactsHelper(root, currentName);
    }

private:
    TrieNode* root;

    // Recursive helper function for alphabetical traversal
    void displayContactsHelper(TrieNode* node, std::string& currentName) const {
        if (node->isEndOfWord) {
            for (const auto& contact : node->contactList) {
                std::cout << "Name: " << currentName << "\n";
                std::cout << "Phone: " << contact.phoneNumber << "\n";
                std::cout << "Email: " << contact.email << "\n";
                std::cout << "Address: " << contact.address << "\n\n";
            }
        }

        for (const auto& [character, childNode] : node->children) {
            currentName.push_back(character);
            displayContactsHelper(childNode, currentName);
            currentName.pop_back();
        }
    }
};

int main() {
    Trie trie;

    // Adding contacts with the same name
    trie.insert("Alice", ContactInfo("123-456-7890", "alice@example.com", "123 Main St"));
    trie.insert("Alice", ContactInfo("555-555-5555", "alice@work.com", "456 Oak St"));
    trie.insert("Bob", ContactInfo("987-654-3210", "bob@example.com", "456 Elm St"));
    trie.insert("Charlie", ContactInfo("555-123-4567", "charlie@example.com", "789 Oak St"));

    // Searching for a contact
    std::vector<ContactInfo>* contacts = trie.search("Alice");
    if (contacts) {
        std::cout << "Found Alice:\n";
        for (const auto& contact : *contacts) {
            std::cout << "Phone: " << contact.phoneNumber << "\n";
            std::cout << "Email: " << contact.email << "\n";
            std::cout << "Address: " << contact.address << "\n\n";
        }
    } else {
        std::cout << "Alice not found.\n";
    }

    // Display all contacts in alphabetical order
    std::cout << "All contacts in alphabetical order:\n";
    trie.displayAllContacts();

    return 0;
}
