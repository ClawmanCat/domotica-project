#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

const static std::string SRC = "./Arduino.cfg";
const static std::string DST = "./DomoticaProject.Arduino.cpp";

int main(int argc, char** argv) {
    // Create filestream.
    std::ifstream stream(SRC);
    if (stream.fail()) {
        std::cout << "An error occurred: " << strerror(errno) << std::endl;
        std::exit(-1);
    }

    // Read file contents.
    std::vector<std::string> res{};

    std::string tmp;
    while (std::getline(stream, tmp)) res.push_back(tmp);

    // Find startup item.
    std::regex rgx("StartupItem=(.+)\\.((cpp)|(hpp)|(c)|(h))");

    bool found = false;
    for (const std::string& str : res) {
        std::smatch m;
        std::regex_search(str, m, rgx);

        if (m.size() > 0) {
            found = true;

            std::string item = m[1].str() + "." + m[2].str();
            std::cout << "Startup item is " << item << std::endl;

            std::ifstream ItemSrc("./" + item, std::ios::binary);
            std::ofstream ItemDst(DST, std::ios::binary);

            if (ItemSrc.fail() || ItemDst.fail()) {
                std::cout << "An error occurred: " << strerror(errno) << std::endl;
                std::exit(-1);
            }

            ItemDst << ItemSrc.rdbuf();
        }
    }

    if (!found) std::cout << "WARNING: Startup item not set. No file will be copied." << std::endl;

    return 0;
}