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

    ~Trie() {
        deleteTrie(root);
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
        current->contactList.push_back(info);
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

    // Delete a specific contact for a name
    bool deleteContact(const std::string& name, const ContactInfo& info) {
        return deleteContactHelper(root, name, info, 0);
    }

private:
    TrieNode* root;

    // Recursive function to delete a contact from the trie
    bool deleteContactHelper(TrieNode* node, const std::string& name, const ContactInfo& info, size_t depth) {
        if (depth == name.size()) {
            // We reached the end node of the name
            if (node->isEndOfWord) {
                // Try to find and erase the specific contact info
                auto& contacts = node->contactList;
                auto it = std::find_if(contacts.begin(), contacts.end(),
                                       [&info](const ContactInfo& c) {
                                           return c.phoneNumber == info.phoneNumber &&
                                                  c.email == info.email &&
                                                  c.address == info.address;
                                       });
                if (it != contacts.end()) {
                    contacts.erase(it); // Remove the contact

                    // If contactList is empty, mark isEndOfWord as false
                    if (contacts.empty()) {
                        node->isEndOfWord = false;
                    }
                }

                // Return true if the node should be deleted (i.e., no children and no contact info)
                return node->children.empty() && !node->isEndOfWord;
            }
            return false; // Name not found
        }

        char ch = name[depth];
        TrieNode* childNode = node->children[ch];
        if (!childNode) return false; // Name not found

        // Recur to the next character in the name
        bool shouldDeleteChild = deleteContactHelper(childNode, name, info, depth + 1);

        // If true, delete the child node as it’s no longer needed
        if (shouldDeleteChild) {
            delete childNode;
            node->children.erase(ch);

            // Return true if this node should also be deleted (no children and not end of another word)
            return node->children.empty() && !node->isEndOfWord;
        }
        return false;
    }

    // Helper function to delete the entire trie
    void deleteTrie(TrieNode* node) {
        if (!node) return;
        for (auto& [ch, childNode] : node->children) {
            deleteTrie(childNode);
        }
        delete node;
    }
};

int main() {
    Trie trie;

    // Adding contacts with the same name
    trie.insert("Alice", ContactInfo("123-456-7890", "alice@example.com", "123 Main St"));
    trie.insert("Alice", ContactInfo("555-555-5555", "alice@work.com", "456 Oak St"));
    trie.insert("Bob", ContactInfo("987-654-3210", "bob@example.com", "456 Elm St"));

    // Delete a specific contact for "Alice"
    if (trie.deleteContact("Alice", ContactInfo("555-555-5555", "alice@work.com", "456 Oak St"))) {
        std::cout << "Contact deleted successfully.\n";
    } else {
        std::cout << "Contact not found.\n";
    }

    return 0;
}