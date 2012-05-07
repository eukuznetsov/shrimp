#include <iostream>
#include <boost/property_tree/ini_parser.hpp>
#include "logreader.h"
#include "mysqldatabase.h"
#include <exception>
#include <vector>
#include <log.h>
#include <cstdlib>
#include <ctime>
#include <stdarg.h>

int main()
{
    // init time for logging func
    // Current date/time based on current system
    time_t now = time(0);
    tm* localtm = localtime(&now);

    //init logging (INFO/WARNING/ERROR/FATAL)
    //Log *pLog = new Log("/var/log/shrimp/access.log");
    Log *pLog = new Log("shrimp_init.log");
    pLog->Write("%s :: INFO :: Shrimp init", asctime(localtm));

    //reading configuration
    boost::property_tree::ptree config;
    try
    {
        boost::property_tree::read_ini("/etc/shrimp.ini", config);
        pLog->Write("%s :: INFO :: Config file was read", asctime(localtm));
    }
    catch (boost::property_tree::ini_parser_error error)
    {
        std::cout
                << error.message() << " : "
                << error.filename() << ", line "
                << error.line() << std::endl;
        //LOG(ERROR) << "INI config file error: "
        //           << error.message() << " : "
        //           << error.filename() << ", line "
        //           << error.line();
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
        pLog->Write("%s :: INFO :: Connect to DB is normal", asctime(localtm));
    }
    catch(DatabaseError& e)
    {
        std::cout << e.what() << std::endl;
        //LOG(ERROR) << "DatabaseError: " << e.what();
    }
    //open log-file
    LogReader::ParserInotify log("/var/log/squid/access.log", &db);
    try
    {
        log.open();
        pLog->Write("%s :: INFO :: access.log file opened", asctime(localtm));

    }
    catch (LogReader::ParserError& e) {
        std::cout
                << e.what() << " "
                << e.filepath() << std::endl;
        //LOG(ERROR) << "LogReader parse error: "
        //           << e.what() << " : "
        //           << e.filepath();
    }
    try{
        log.watch();
        pLog->Write("%s :: INFO :: access.log file watching", asctime(localtm));
    }
    catch (LogReader::ParserError &e){
        std::cout
            << e.what() << " "
            << e.filepath() << std::endl;
        //LOG(ERROR) << "LogReader parse error on watching: "
        //        << e.what() << " : "
        //        << e.filepath();
    }
    return 0;
}
