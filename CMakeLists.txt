cmake_minimum_required(VERSION 4.0)
project(huffman_tree_decoder)

set(CMAKE_CXX_STANDARD 20)

add_executable(huffman_tree_decoder main.cpp utils.cpp
        HuffmanTree.cpp
        HuffmanTree.hpp)
