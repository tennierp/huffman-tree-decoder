#pragma once

#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include "utils.hpp"

struct TreeNode {
    std::string word;
    TreeNode* left;
    TreeNode* right;

    bool isLeaf() const {
        return left == nullptr && right == nullptr;
    }
};

class HuffmanTree {
private:
    TreeNode* root;

    void deleteTree(TreeNode* node);

public:
    HuffmanTree();
    ~HuffmanTree();

    error_type buildFromHeader(const std::vector<std::pair<std::string, std::string>>& header_pairs);
    error_type decode(std::ifstream& code_stream, std::ofstream& out_stream);
};