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
    Log *pLog = new Log("/var/log/shrimp/shrimp_init.log");
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
        //pLog->Write("%s :: ERROR :: INI config file error: %s : %s, line %i", asctime(localtm), error.message(), error.filename(), error.line());
        pLog->Write("%s :: ERROR :: INI config file error", asctime(localtm));
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
        pLog->Write("%s :: ERROR :: DatabaseError: %s", asctime(localtm), e.what());
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
        pLog->Write("%s :: ERROR :: LogReader parse error: %s %s", asctime(localtm), e.what(), e.filepath());
    }
    pLog->Write("%s :: INFO :: access.log file is watching", asctime(localtm));
    log.watch();
    return 0;
}
