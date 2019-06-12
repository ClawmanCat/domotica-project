#define _CRT_SECURE_NO_WARNINGS

#include <filesystem>
#include <regex>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <conio.h>

using namespace std::filesystem;

std::vector<std::string> ReadFile(const std::string& path) {
    std::ifstream stream(path);
    if (stream.fail()) {
        std::cout << "Failed to open file " << path << ": " << strerror(errno) << std::endl;
        std::cout << "Entry will be skipped" << std::endl;

        return {};
    }

    std::vector<std::string> file;
    std::string tmp;

    while (std::getline(stream, tmp)) file.push_back(tmp);

    return file;
}

void WriteFile(const std::string& path, const std::vector<std::string>& file) {
    std::ofstream stream(path, std::ios::trunc);
    if (stream.fail()) {
        std::cout << "Failed to open file " << path << ": " << strerror(errno) << std::endl;
        std::cout << "Entry will be skipped" << std::endl;

        return;
    }

    for (const std::string& line : file) stream << line << '\n';
}


void iterate(std::filesystem::path dpath, int depth = 0) {
    for (auto& entry : directory_iterator(dpath)) {
        if (entry.is_directory()) iterate(entry, depth + 1);

        if (entry.path().extension() == ".hpp") {
            std::cout << "Candidate file: " << entry.path().string() << std::endl;

            std::vector<std::string> file = ReadFile(entry.path().string());

            int ln = -1;
            std::regex rgx{ R"(#\s*include\s*"Dependencies/boost/(.+)\.hpp")" };
            for (std::string& line : file) {
                ++ln;
                std::smatch match;
                if (!std::regex_search(line, match, rgx)) continue;

                std::vector<char> newline;
                newline.resize(line.length() + 3 * depth + 1);

                std::string res = "# include \"";
                for (int i = 0; i < depth; ++i) res += "../";
                res += "Dependencies/boost/";
                res += match[1];
                res += ".hpp\"";

                std::regex_replace(newline.begin(), line.begin(), line.end(), rgx, res);
                std::cout << "Replaced line " << ln << " in file " << entry.path().string() << std::endl;

                line = std::string(newline.begin(), newline.end());
            }

            WriteFile(entry.path().string(), file);
        }
    }
}

int main(int argc, char** argv) {
    iterate("./");

    std::cout << "Press any key to exit... ";
    _getch();
}