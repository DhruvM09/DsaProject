#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>

struct BKTreeNode {
    std::string word;
    std::unordered_map<int, BKTreeNode*> children;

    BKTreeNode(const std::string& w) : word(w) {}
};

class BKTree {
public:
    BKTree() : root(nullptr) {}

    void insert(const std::string& word) {
        if (!root) {
            root = new BKTreeNode(word);
            return;
        }
        insertNode(root, word);
    }

    std::vector<std::string> search(const std::string& query, int tolerance) {
        std::vector<std::string> results;
        if (root) {
            searchNode(root, query, tolerance, results);
        }
        return results;
    }

private:
    BKTreeNode* root;

    int editDistance(const std::string& s1, const std::string& s2) {
        int len1 = s1.size(), len2 = s2.size();
        std::vector<std::vector<int>> dp(len1 + 1, std::vector<int>(len2 + 1));

        for (int i = 0; i <= len1; i++) dp[i][0] = i;
        for (int j = 0; j <= len2; j++) dp[0][j] = j;

        for (int i = 1; i <= len1; i++) {
            for (int j = 1; j <= len2; j++) {
                if (s1[i - 1] == s2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1];
                } else {
                    dp[i][j] = 1 + std::min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
                }
            }
        }
        return dp[len1][len2];
    }

    void insertNode(BKTreeNode* node, const std::string& word) {
        int distance = editDistance(node->word, word);
        if (node->children.find(distance) == node->children.end()) {
            node->children[distance] = new BKTreeNode(word);
        } else {
            insertNode(node->children[distance], word);
        }
    }

    void searchNode(BKTreeNode* node, const std::string& query, int tolerance, std::vector<std::string>& results) {
        int dist = editDistance(node->word, query);
        if (dist <= tolerance) {
            results.push_back(node->word);
        }

        for (int d = std::max(0, dist - tolerance); d <= dist + tolerance; d++) {
            if (node->children.find(d) != node->children.end()) {
                searchNode(node->children[d], query, tolerance, results);
            }
        }
    }
};

int main() {
    BKTree tree;
    tree.insert("John");
    tree.insert("Johnny");
    tree.insert("Jonathan");
    tree.insert("Jane");
    tree.insert("Johan");

    std::string query = "Jhon";
    int tolerance = 1;

    std::vector<std::string> results = tree.search(query, tolerance);

    std::cout << "Results for fuzzy search with tolerance " << tolerance << " for query \"" << query << "\":" << std::endl;
    for (const auto& result : results) {
        std::cout << result << std::endl;
    }

    return 0;
}
