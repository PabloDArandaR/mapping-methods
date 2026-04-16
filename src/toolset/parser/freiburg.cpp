#include "toolset/parser/freiburg.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

namespace {

const std::string PARAM_SIGNAL_NAME = "PARAM";

std::string jumpNoDataLines(std::ifstream& file) {
    std::string dummy;
    getline(file, dummy);
    while (dummy[0] == '#') {
        getline(file, dummy);
    }
    return dummy;
}

} // namespace

namespace mm {
namespace toolset {

void FreiburgParser::_parseLine(std::string line) {
    signal_data* new_signal_data = new signal_data();
    std::stringstream current_line_stream(line);
    std::string current_val;
    int n_signal_values{0};
    std::getline(current_line_stream, current_val, ' ');
    if (current_val == PARAM_SIGNAL_NAME) {
        param* new_param = new param();
        std::getline(current_line_stream, current_val, ' ');
        new_param->name = current_val;

        std::getline(current_line_stream, current_val, ' ');
        new_param->value = current_val;
    } else {
        int n_data_values{-3};
        std::string signal_name{current_val};
        if (this->data.find(current_val) == this->data.end()) {
            std::vector<signal_data*> new_vector(0);
            this->data[current_val] = new_vector;
        }
        while (std::getline(current_line_stream, current_val, ' ')) {
            ++n_data_values;
        }
        current_line_stream.clear();
        current_line_stream.seekg(0);

        std::getline(current_line_stream, current_val, ' ');
        for (int i{0}; i < n_data_values; ++i) {
            std::getline(current_line_stream, current_val, ' ');
            new_signal_data->data.push_back(current_val);
        }

        std::getline(current_line_stream, current_val, ' ');
        new_signal_data->ipc_timestamp = std::stof(current_val);
        std::getline(current_line_stream, current_val, ' ');
        new_signal_data->ipc_hostname = current_val;
        std::getline(current_line_stream, current_val, ' ');
        new_signal_data->logger_timestamp = std::stof(current_val);

        this->data[signal_name].push_back(new_signal_data);
    }
}

void FreiburgParser::parseFile(std::string datafile) {
    std::ifstream file(datafile);
    std::string line = jumpNoDataLines(file);
    this->_parseLine(line);
    while (getline(file, line)) {
        this->_parseLine(line);
    }

    std::cout << "Finished extracting data." << std::endl;
};

std::vector<signal_data*>& FreiburgParser::getSignalData(std::string signal_name) {
    return this->data[signal_name];
}

std::unordered_map<std::string, std::vector<signal_data*>>& FreiburgParser::getData() { return this->data; }

} // namespace toolset
} // namespace mm
