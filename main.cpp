#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

#include "utils.hpp"
#include "HuffmanTree.hpp"

error_type readHeader(const std::string& hdrFileName, std::vector<std::pair<std::string, std::string>>& header_pairs) {

    std::ifstream hdrFile(hdrFileName);
    if (!hdrFile.is_open()) {
        return UNABLE_TO_OPEN_FILE;
    }

    header_pairs.clear();
    std::string word, code;

    while (hdrFile >> word >> code) {
        header_pairs.emplace_back(word, code);
    }

    if (hdrFile.bad()) {
        return UNABLE_TO_OPEN_FILE;
    }

    hdrFile.close();
    return NO_ERROR;
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    namespace fs = std::filesystem;

    const fs::path dirName = "input_output";
    const fs::path inputHdrFileName = dirName / argv[1];
    const fs::path inputCodeFileName = dirName / argv[2];
    const fs::path outputFileName = dirName / (fs::path(argv[2]).stem().string() + ".tokens_decoded");

    // Check that directory exists
    if (error_type status; (status = directoryExists(dirName.string())) != NO_ERROR)
        exitOnError(status, dirName.string());

    // Check that .hdr input file exists
    if (error_type status; (status = regularFileExistsAndIsAvailable(inputHdrFileName.string())) != NO_ERROR)
        exitOnError(status, inputHdrFileName.string());

    // Check that .code input file exists
    if (error_type status; (status = regularFileExistsAndIsAvailable(inputCodeFileName.string())) != NO_ERROR)
        exitOnError(status, inputCodeFileName.string());

    // Check that output file can be created
    if (error_type status; (status = canOpenForWriting(outputFileName.string())) != NO_ERROR)
        exitOnError(status, outputFileName.string());

    // Read the header file
    std::vector<std::pair<std::string, std::string>> header_pairs;
    if (error_type status; (status = readHeader(inputHdrFileName.string(), header_pairs)) != NO_ERROR)
        exitOnError(status, inputHdrFileName.string());

    // Build the Huffman tree
    HuffmanTree decoder;

    error_type status = decoder.buildFromHeader(header_pairs);
    if (status != NO_ERROR) {
        std::cerr << "Error: Failed to build Huffman tree from header\n";
        return 1;
    }

    // Open the code file
    std::ifstream code_stream(inputCodeFileName);
    if (!code_stream.is_open()) {
        exitOnError(UNABLE_TO_OPEN_FILE, inputCodeFileName);
    }

    // Open the output file
    std::ofstream out_stream(outputFileName);
    if (!out_stream.is_open()) {
        exitOnError(UNABLE_TO_OPEN_FILE_FOR_WRITING, outputFileName);
    }

    // Decode the message
    status = decoder.decode(code_stream, out_stream);
    if (status != NO_ERROR) {
        std::cerr << "Error: Failed to decode message\n";
        code_stream.close();
        out_stream.close();
        return 1;
    }

    // Close the files
    code_stream.close();
    out_stream.close();

    return 0;
}