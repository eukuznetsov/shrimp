#include <iostream>
#include <boost/property_tree/ini_parser.hpp>
#include "inbasereader.h"

int main()
{
    boost::property_tree::ptree config;
    try
    {
        boost::property_tree::read_ini("/etc/shrimp.ini", config);
    }
    catch (boost::property_tree::ini_parser_error error)
    {
        std::cout
                << error.message() << ":"
                << error.filename() << ", line"
                << error.line() << std::endl;
        exit(1);
    }
    try
    {
        InBaseReader log("/etc/shrimp.ini");
    }
    catch(...)
    {
        //process exception
    }

    return 0;
}
