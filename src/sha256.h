#ifndef SHA256_H
#define SHA256_H

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <openssl/evp.h>

// Function to compute the SHA256 hash of a string
std::string sha256(const std::string &data) {
    unsigned char hash[EVP_MAX_MD_SIZE];  // EVP_MAX_MD_SIZE can hold the largest hash (e.g., SHA256)
    unsigned int length = 0;  // To store the length of the hash

    // Create a new context for SHA256
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (mdctx == nullptr) {
        std::cerr << "Error creating context!" << std::endl;
        return "";
    }

    // Initialize the context for SHA256
    if (EVP_DigestInit_ex(mdctx, EVP_sha256(), nullptr) != 1) {
        std::cerr << "Error initializing the context!" << std::endl;
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    // Update the context with the data
    if (EVP_DigestUpdate(mdctx, data.c_str(), data.size()) != 1) {
        std::cerr << "Error updating the context!" << std::endl;
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    // Finalize the hash computation
    if (EVP_DigestFinal_ex(mdctx, hash, &length) != 1) {
        std::cerr << "Error finalizing the hash!" << std::endl;
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    // Clean up
    EVP_MD_CTX_free(mdctx);

    // Convert the hash to a hex string
    std::stringstream ss;
    for (unsigned int i = 0; i < length; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return ss.str();
}

#endif // SHA256_H