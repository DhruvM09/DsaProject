#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using namespace std;

// Node structure representing each letter in a contact name
struct Node {
    unordered_map<char, Node*> links;
    bool isEndOfWord = false;
    int cntEndWith = 0;
    int cntPrefix = 0;
    unordered_map<string, string> details;  // Stores details like phone, email

    bool containsKey(char ch) {
        return links.find(ch) != links.end();
    }

    void put(char ch, Node* node) {
        links[ch] = node;
    }

    Node* get(char ch) {
        return links[ch];
    }

    void setEnd() {
        isEndOfWord = true;
    }

    bool isEnd() {
        return isEndOfWord;
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

// Trie class for managing contacts
class Trie {
private:
    Node* root;

    void collectWords(Node* node, string prefix, vector<string>& results) {
        if (node->isEnd()) results.push_back(prefix);
        for (auto& link : node->links) {
            collectWords(link.second, prefix + link.first, results);
        }
    }

public:
    Trie() {
        root = new Node();
    }

    // Insert contact with details
    void insert(string name, unordered_map<string, string> details) {
        Node* node = root;
        for (char ch : name) {
            if (!node->containsKey(ch)) {
                node->put(ch, new Node());
            }
            node = node->get(ch);
            node->increasePrefix();
        }
        node->setEnd();
        node->increaseEnd();
        node->details = details;
    }

    // Search exact contact
    bool search(string name) {
        Node* node = root;
        for (char ch : name) {
            if (!node->containsKey(ch)) return false;
            node = node->get(ch);
        }
        return node->isEnd();
    }

    // Get contact details
    unordered_map<string, string> getDetails(string name) {
        Node* node = root;
        for (char ch : name) {
            if (!node->containsKey(ch)) return {};
            node = node->get(ch);
        }
        return node->isEnd() ? node->details : unordered_map<string, string>{};
    }

    // Suggest contacts based on prefix
    vector<string> suggestContacts(string prefix) {
        Node* node = root;
        vector<string> results;
        for (char ch : prefix) {
            if (!node->containsKey(ch)) return results;
            node = node->get(ch);
        }
        collectWords(node, prefix, results);
        return results;
    }

    // Count contacts equal to the given name
    int countWordsEqualTo(string& name) {
        Node* node = root;
        for (char ch : name) {
            if (!node->containsKey(ch)) return 0;
            node = node->get(ch);
        }
        return node->getEnd();
    }

    // Count contacts starting with the given prefix
    int countWordsStartWith(string& prefix) {
        Node* node = root;
        for (char ch : prefix) {
            if (!node->containsKey(ch)) return 0;
            node = node->get(ch);
        }
        return node->getPrefix();
    }

    // Remove a contact
    void erase(string name) {
        Node* node = root;
        vector<Node*> path;
        for (char ch : name) {
            if (!node->containsKey(ch)) return;
            path.push_back(node);
            node = node->get(ch);
        }
        if (node->isEnd()) {
            node->deleteEnd();
            node->details.clear();
            for (int i = path.size() - 1; i >= 0; i--) {
                path[i]->reducePrefix();
                if (path[i]->getPrefix() == 0) {
                    delete path[i]->get(name[i]);
                    path[i]->links.erase(name[i]);
                }
            }
        }
    }
};

// Helper function to format contact details
void displayDetails(const unordered_map<string, string>& details) {
    if (details.empty()) {
        cout << "No additional details available.\n";
        return;
    }
    for (const auto& pair : details) {
        cout << pair.first << ": " << pair.second << "\n";
    }
}

int main() {
    Trie contactManager;
    string command;

    cout << "Welcome to the Smart Contact List Manager!\n";
    cout << "Commands: \n"
         << "1. add <name> <phone> <email> - Add a new contact with details\n"
         << "2. find <name> - Find a contact and show details\n"
         << "3. suggest <prefix> - Get contact suggestions based on prefix\n"
         << "4. count_equal <name> - Count exact matches of a contact name\n"
         << "5. count_prefix <prefix> - Count contacts starting with a prefix\n"
         << "6. delete <name> - Remove a contact\n"
         << "7. exit - End the program\n\n";

    while (true) {
        cout << "Enter command: ";
        cin >> command;
        if (command == "add") {
            string name, phone, email;
            cin >> name >> phone >> email;
            unordered_map<string, string> details = {{"Phone", phone}, {"Email", email}};
            contactManager.insert(name, details);
            cout << "Added \"" << name << "\" to contacts.\n";
        }
        else if (command == "find") {
            string name;
            cin >> name;
            if (contactManager.search(name)) {
                cout << "Details for \"" << name << "\":\n";
                displayDetails(contactManager.getDetails(name));
            } else {
                cout << "\"" << name << "\" not found in contacts.\n";
            }
        }
        else if (command == "suggest") {
            string prefix;
            cin >> prefix;
            vector<string> suggestions = contactManager.suggestContacts(prefix);
            if (!suggestions.empty()) {
                cout << "Suggestions for prefix \"" << prefix << "\":\n";
                for (const string& suggestion : suggestions) {
                    cout << suggestion << "\n";
                }
            } else {
                cout << "No contacts found with the prefix \"" << prefix << "\".\n";
            }
        }
        else if (command == "count_equal") {
            string name;
            cin >> name;
            cout << "Count of \"" << name << "\": " << contactManager.countWordsEqualTo(name) << "\n";
        }
        else if (command == "count_prefix") {
            string prefix;
            cin >> prefix;
            cout << "Contacts with prefix \"" << prefix << "\": " << contactManager.countWordsStartWith(prefix) << "\n";
        }
        else if (command == "delete") {
            string name;
            cin >> name;
            contactManager.erase(name);
            cout << "Deleted \"" << name << "\" from contacts.\n";
        }
        else if (command == "exit") {
            cout << "Exiting the Contact List Manager.\n";
            break;
        }
        else {
            cout << "Invalid command. Please try again.\n";
        }
    }

    return 0;
}
