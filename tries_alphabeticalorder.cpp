#include <iostream>
#include <map>
#include <string>

struct TrieNode {
    std::map<char, TrieNode*> children;
    bool isEndOfWord;
    int count; // Tracks number of times the word is inserted

    TrieNode() : isEndOfWord(false), count(0) {}
};

class Trie {
public:
    Trie() {
        root = new TrieNode();
    }

    // Function to insert a word into the trie
    void insert(const std::string& word) {
        TrieNode* current = root;
        for (char c : word) {
            if (!current->children[c]) {
                current->children[c] = new TrieNode();
            }
            current = current->children[c];
        }
        current->isEndOfWord = true;
        current->count++;  // Increment count each time the word is inserted
    }

    // Function to perform alphabetical traversal of the trie
    void alphabeticalTraversal() const {
        std::string currentWord;
        alphabeticalTraversalHelper(root, currentWord);
    }

private:
    TrieNode* root;

    // Helper function for recursive alphabetical traversal
    void alphabeticalTraversalHelper(TrieNode* node, std::string& currentWord) const {
        if (node->isEndOfWord) {
            // Print the word as many times as its count
            for (int i = 0; i < node->count; ++i) {
                std::cout << currentWord << std::endl;
            }
        }

        // Traverse all children in alphabetical order
        for (const auto& [character, childNode] : node->children) {
            currentWord.push_back(character);
            alphabeticalTraversalHelper(childNode, currentWord);
            currentWord.pop_back();
        }
    }
};

int main() {
    Trie trie;
    trie.insert("apple");
    trie.insert("app");
    trie.insert("banana");
    trie.insert("app");      // Duplicate insertion of "app"
    trie.insert("banana");   // Duplicate insertion of "banana"
    trie.insert("apple");    // Duplicate insertion of "apple"

    std::cout << "Words in alphabetical order with counts:" << std::endl;
    trie.alphabeticalTraversal();

    return 0;
}
