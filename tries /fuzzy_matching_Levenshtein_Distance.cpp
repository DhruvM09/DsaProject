#include <iostream>
#include <map>
#include <string>
#include <vector>

struct TrieNode {
    std::map<char, TrieNode*> children;
    bool isEndOfWord;

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

    void insert(const std::string& word) {
        TrieNode* current = root;
        for (char c : word) {
            if (!current->children[c]) {
                current->children[c] = new TrieNode();
            }
            current = current->children[c];
        }
        current->isEndOfWord = true;
    }

    std::vector<std::string> searchWithTolerance(const std::string& query, int maxTolerance) {
        std::vector<std::string> results;
        fuzzySearch(root, "", query, maxTolerance, results);
        return results;
    }

private:
    TrieNode* root;

    void fuzzySearch(TrieNode* node, std::string currentWord, const std::string& query,
                     int tolerance, std::vector<std::string>& results, int index = 0) {
        // Check if we have reached the end of the query and node represents a valid word
        if (index == query.size()) {
            if (node->isEndOfWord && tolerance >= 0) {
                results.push_back(currentWord);
            }
            return;
        }

        char targetChar = query[index];

        // Attempt exact match
        if (node->children.find(targetChar) != node->children.end()) {
            fuzzySearch(node->children[targetChar], currentWord + targetChar, query, tolerance, results, index + 1);
        }

        if (tolerance > 0) {
            // Try substitution
            for (const auto& [ch, childNode] : node->children) {
                if (ch != targetChar) {
                    fuzzySearch(childNode, currentWord + ch, query, tolerance - 1, results, index + 1);
                }
            }

            // Try insertion
            fuzzySearch(node, currentWord + targetChar, query, tolerance - 1, results, index + 1);

            // Try deletion
            for (const auto& [ch, childNode] : node->children) {
                fuzzySearch(childNode, currentWord + ch, query, tolerance - 1, results, index);
            }
        }
    }

    // Helper to delete entire trie
    void deleteTrie(TrieNode* node) {
        if (!node) return;
        for (auto& [_, child] : node->children) {
            deleteTrie(child);
        }
        delete node;
    }
};

int main() {
    Trie trie;
    trie.insert("John");
    trie.insert("Johnny");
    trie.insert("Jonathan");
    trie.insert("Jane");
    trie.insert("Johan");

    std::string query = "Jhon";
    int tolerance = 1;

    std::vector<std::string> results = trie.searchWithTolerance(query, tolerance);

    std::cout << "Results for fuzzy search with tolerance " << tolerance << " for query \"" << query << "\":" << std::endl;
    for (const auto& result : results) {
        std::cout << result << std::endl;
    }

    return 0;
}
