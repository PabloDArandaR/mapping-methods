#ifndef INCLUDE_TOOLSET_PARSER_FREIBURG_HPP
#define INCLUDE_TOOLSET_PARSER_FREIBURG_HPP

#include "spdlog/spdlog.h"
#include "toolset/parser/parser.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace mm {
namespace toolset {

class FreiburgParser : Parser {
  public:
    FreiburgParser(){};
    std::vector<signal_data*>& getSignalData(std::string signal_name);
    std::unordered_map<std::string, std::vector<signal_data*>>& getData();
    void parseFile(std::string datafile) override;
    void _parseLine(std::string line);

  private:
    std::unordered_map<std::string, std::vector<signal_data*>> data;
    std::unordered_map<std::string, std::vector<param*>> parameters;
};

} // namespace toolset
} // namespace mm

#endif
