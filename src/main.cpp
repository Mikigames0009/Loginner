#include <iostream>
#include <string>
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>
#include <openssl/evp.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>
#include "sha256.h"

using json = nlohmann::json;
using string = std::string;

void endline() { std::cout << std::endl; }

char mode() {
    string input;
    std::cout << "Register or login (r/default l): ";
    std::getline(std::cin, input);
    if (input.empty()) {
        return 'l';
    }
    return input[0];
}

// Function to save the username and password to a JSON file
void savejson(const string& username, const string& password) {
    std::filesystem::path source_path = __FILE__;
    std::filesystem::path dir = source_path.parent_path();
    std::filesystem::path json_file = dir / "data.json";

    json j;
    std::ifstream i(json_file);
    if (i.is_open()) {
        i >> j;
        i.close();
    }

    if (!j.is_array()) {
        j = json::array();
    }

    json new_entry;
    new_entry["username"] = username;
    new_entry["password"] = password;

    j.push_back(new_entry);

    std::ofstream o(json_file);
    o << std::setw(4) << j << std::endl;
}

// Function to check login credentials
bool check_credentials(const string& username, const string& password) {
    std::filesystem::path source_path = __FILE__;
    std::filesystem::path dir = source_path.parent_path();
    std::filesystem::path json_file = dir / "data.json";

    json j;
    std::ifstream i(json_file);
    if (i.is_open()) {
        i >> j;
        i.close();
    } else {
        return false;
    }

    string hash = sha256(password);

    for (const auto& entry : j) {
        if (entry["username"] == username && entry["password"] == hash) {
            return true;
        }
    }

    return false;
}

// Function to handle user registration
void register_user() {
    string username;
    string password;
    std::cout << "Enter username: ";
    std::cin >> username;
    endline();
    std::cout << "Enter password: ";
    std::cin >> password;
    endline();
    string hash = sha256(password);
    savejson(username, hash);
    std::cout << "Successfully registered!" << std::endl;
}

// Function to handle user login
void login_user() {
    string username;
    string password;
    std::cout << "Enter username: ";
    std::cin >> username;
    endline();
    std::cout << "Enter password: ";
    std::cin >> password;
    endline();
    if (check_credentials(username, password)) {
        std::cout << "Login successful!" << std::endl;
    } else {
        std::cout << "Invalid username or password." << std::endl;
    }
}

// Main function
int main() {
    char choice = mode();

    if (choice == 'r') {
        register_user();
    } else if (choice == 'l') {
        login_user();
    }
}