#ifndef INCLUDE_TOOLSET_PARSER_PARSER_HPP
#define INCLUDE_TOOLSET_PARSER_PARSER_HPP

#include <string>
#include <vector>

namespace mm {
namespace toolset {

class Parser {
    virtual void parseFile(std::string datafile) = 0;
};

struct log_data {
    std::vector<std::string> data;
};

struct signal_data : log_data {
    double logger_timestamp;
    double ipc_timestamp;
    std::string ipc_hostname;
};

struct param {
    std::string name;
    std::string value;
};

} // namespace toolset
} // namespace mm

#endif
