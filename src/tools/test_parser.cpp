#include "toolset/parser/freiburg.hpp"
#include <iostream>

using namespace mm::toolset;

int main(int argc, char** argv) {
    FreiburgParser parser;
    std::string filepath{argv[1]};

    parser.parseFile(filepath);

    for (auto it{parser.getData().begin()}; it != parser.getData().end(); ++it) {
        if (it->second.empty()) {
            std::cout << " Signal empty" << std::endl;
            continue;
        }
        for (int i{0}; i < (int)it->second.size(); ++i) {
            if (it->second[i] == nullptr) {
                std::cerr << "Null pointer at index " << i << " for key " << it->first << std::endl;
                continue;
            }
            for (int j{0}; j < (int)it->second[i]->data.size(); ++j) {
                std::cout << it->second[i]->data[j] << ", ";
            }
            std::cout << std::endl;
            std::cout << "IPC Timestamp:    " << it->second[i]->ipc_timestamp << std::endl;
            std::cout << "IPC Hostname:     " << it->second[i]->ipc_hostname << std::endl;
            std::cout << "logger timestamp: " << it->second[i]->logger_timestamp << std::endl;
        }
    }
}
