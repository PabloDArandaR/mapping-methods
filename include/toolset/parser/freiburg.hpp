#include "toolset/parser/parser.hpp"
#include "spdlog/spdlog.h"

struct param{
    std::string name, value;
    float logger_timestamp;
    std::string ipc_hostname;
};

struct message{
    float ipc_timestamp, logger_timestamp;
    std::string ipc_hostname;
};

struct ODOM: message{
    float x,y,theta,tv,rv,accel;
};

struct LASER: message{
    int num_readings;
    float x, y, theta,odom_x, odom_y, odom_theta;
};

class FreiburgParser: Parser{
    public:
        FreiburgParser(){};
        void parseFile(std::string datafile) override;
};