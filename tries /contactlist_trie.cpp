#include <iostream>
using namespace std;

struct Node {
    Node* links[26];
    int cntEndWith = 0;
    int cntPrefix = 0;

    bool containsKey(char ch) {
        return links[ch - 'a'] != nullptr;
    }
    
    void put(char ch, Node* node) {
        links[ch - 'a'] = node;
    }

    Node* get(char ch) {
        return links[ch - 'a'];
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
    Node* root;

public:
    Trie() {
        root = new Node();
    }

    void insert(string word) {
        Node* node = root;
        for (char ch : word) {
            if (!node->containsKey(ch)) {
                node->put(ch, new Node());
            }
            node = node->get(ch);
            node->increasePrefix();
        }
        node->increaseEnd();
    }

    bool search(string word) {
        Node* node = root;
        for (char ch : word) {
            if (!node->containsKey(ch)) {
                return false;
            }
            node = node->get(ch);
        }
        return node->getEnd() > 0 ;
    }

    bool startsWith(string prefix) {
        Node* node = root;
        for (char ch : prefix) {
            if (!node->containsKey(ch)) {
                return false;
            }
            node = node->get(ch);
        }
        return true;
    }

    int countWordsEqualTo(string word) {
        Node* node = root;
        for (char ch : word) {
            if (!node->containsKey(ch)) {
                return 0;
            }
            node = node->get(ch);
        }
        return node->getEnd();
    }

    int countWordsStartWith(string prefix) {
        Node* node = root;
        for (char ch : prefix) {
            if (!node->containsKey(ch)) {
                return 0;
            }
            node = node->get(ch);
        }
        return node->getPrefix();
    }

    void erase(string word) {
        Node* node = root;
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

int main() {
    Trie trie;
    string command;
    
    cout << "Trie Operations:\n";
    cout << "1. insert <word> - Insert a word into the Trie\n";
    cout << "2. search <word> - Search for a word in the Trie\n";
    cout << "3. startsWith <prefix> - Check if a prefix exists in any word\n";
    cout << "4. countWordsEqualTo <word> - Count occurrences of a specific word\n";
    cout << "5. countWordsStartWith <prefix> - Count words starting with a specific prefix\n";
    cout << "6. erase <word> - Erase a word from the Trie\n";
    cout << "7. exit - End the program\n\n";
    
    while (true) {
        cout << "Enter command: ";
        cin >> command;
        
        if (command == "insert") {
            string word;
            cin >> word;
            trie.insert(word);
            cout << "Inserted \"" << word << "\" into the Trie.\n";
        }
        else if (command == "search") {
            string word;
            cin >> word;
            if (trie.search(word)) {
                cout << "Word \"" << word << "\" found in the Trie.\n";
            } else {
                cout << "Word \"" << word << "\" not found in the Trie.\n";
            }
        }
        else if (command == "startsWith") {
            string prefix;
            cin >> prefix;
            if (trie.startsWith(prefix)) {
                cout << "There exists a word in the Trie with the prefix \"" << prefix << "\".\n";
            } else {
                cout << "No word in the Trie starts with the prefix \"" << prefix << "\".\n";
            }
        }
        else if (command == "countWordsEqualTo") {
            string word;
            cin >> word;
            int count = trie.countWordsEqualTo(word);
            cout << "The word \"" << word << "\" appears " << count << " time(s) in the Trie.\n";
        }
        else if (command == "countWordsStartWith") {
            string prefix;
            cin >> prefix;
            int count = trie.countWordsStartWith(prefix);
            cout << "There are " << count << " words in the Trie that start with the prefix \"" << prefix << "\".\n";
        }
        else if (command == "erase") {
            string word;
            cin >> word;
            trie.erase(word);
            cout << "Erased \"" << word << "\" from the Trie (if it existed).\n";
        }
        else if (command == "exit") {
            cout << "Exiting the program.\n";
            break;
        }
        else {
            cout << "Invalid command. Please try again.\n";
        }
    }
    
    return 0;
}
