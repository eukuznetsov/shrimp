#include <iostream>
#include <boost/property_tree/ini_parser.hpp>
#include "logreader.h"
#include "mysqldatabase.h"
#include <exception>
#include <vector>
#include <glog/logging.h>

int main(int argc, char* argv[])
{
    // Initialize Google's logging library.
    google::InitGoogleLogging(argv[0]);
    //reading configuration
    LOG(INFO) << "Start";
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

    //connect to the database
    MysqlDatabase db;
    try
    {
        db.connect((config.get<std::string>("database.path")).c_str(),
                   (config.get<std::string>("database.user")).c_str(),
                   (config.get<std::string>("database.password")).c_str(),
                   (config.get<std::string>("database.dbname")).c_str());
    }
    catch(DatabaseError& e)
    {
        std::cout << e.what() << std::endl;
    }

    //open log-file
    LogReader::ParserInotify log("/var/log/squid/access.log", &db);
    try
    {
        log.open();
    }
    catch (LogReader::ParserError& e) {
        std::cout
                << e.what() << " "
                << e.filepath() << std::endl;
    }
    log.watch();

    return 0;
}
