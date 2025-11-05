#include "HuffmanTree.hpp"
#include <iostream>

HuffmanTree::HuffmanTree() {
    root = nullptr;
}

HuffmanTree::~HuffmanTree() {
    deleteTree(root);
}

void HuffmanTree::deleteTree(TreeNode* node) {
    if (node == nullptr) {
        return;
    }
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

error_type HuffmanTree::buildFromHeader(const std::vector<std::pair<std::string, std::string>>& header_pairs) {

    root = new TreeNode();

    for (const auto& [word, code] : header_pairs) {
        TreeNode* current = root;

        // For each bit in the code:
        // If the bit is 0, go to (or create) the left child.
        // If the bit is 1, go to (or create) the right child.

        for (char bit : code) {
            if (bit == '0') {
                // Go left
                if (current->left == nullptr) {
                    current->left = new TreeNode();
                }
                current = current->left;
            } else if (bit == '1') {
                // Go right
                if (current->right == nullptr) {
                    current->right = new TreeNode();
                }
                current = current->right;
            } else {
                // Invalid character in code
                return DECODING_ERROR;
            }
        }

        current->word = word;
    }

    return NO_ERROR;
}

error_type HuffmanTree::decode(std::ifstream& code_stream, std::ofstream& out_stream) {
    if (root == nullptr) {
        return DECODING_ERROR;
    }

    TreeNode* current = root;
    char ch;
    bool first_word = true;

    while (code_stream.get(ch)) {

        if (ch == '\n') {
            continue;
        }

        if (ch == '0') {
            if (current->left == nullptr) {
                return DECODING_ERROR;
            }
            current = current->left;
        } else if (ch == '1') {
            if (current->right == nullptr) {
                return DECODING_ERROR;
            }
            current = current->right;
        } else {
            return DECODING_ERROR;
        }

        if (current->isLeaf()) {
            if (!first_word) {
                out_stream << '\n';
            }
            out_stream << current->word;
            first_word = false;

            current = root;
        }
    }

    if (!first_word) {
        out_stream << '\n';
    }

    if (current != root) {
        return DECODING_ERROR;
    }

    return NO_ERROR;
}