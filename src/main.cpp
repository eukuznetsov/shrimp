#include <iostream>
#include <boost/property_tree/ini_parser.hpp>
#include "logreader.h"
#include "mysqldatabase.h"
#include <exception>
#include <vector>
#include <glog/logging.h>
#include <gflags/gflags.h>

using namespace google;
using namespace std;

struct NullLogger : public base::Logger {
  virtual void Write(bool should_flush,
                     time_t timestamp,
                     const char* message,
                     int length) {
  }

virtual void Flush(){
};

virtual uint32 LogSize(){
    return 0; }
};

void SetMyLogger(int severity, base::Logger* logger)
{
    base::Logger* old_logger = base::GetLogger(severity);
    base::SetLogger(severity, logger);
    FlushLogFiles(severity);
}

int main(int argc, char* argv[])
{
    // Initialize Google's logging library.
    InitGoogleLogging(argv[0]);

    NullLogger null_logger;
    SetMyLogger(WARNING, &null_logger);
    SetMyLogger(ERROR, &null_logger);
    //SetMyLogger(FATAL, &null_logger);

    //reading configuration
    std::cout<<"Shrimp init1"<<std::endl;
    LOG(INFO) << "Shrimp init";
    std::cout<<"Shrimp init2"<<std::endl;
    LOG(WARNING) << "Shrimp init12";
    std::cout<<"Shrimp init3"<<std::endl;
    LOG(INFO) << "Shrimp init2";
    boost::property_tree::ptree config;
    try
    {
        boost::property_tree::read_ini("/etc/shrimp.ini", config);
        LOG(INFO) << "Config file was read";
    }
    catch (boost::property_tree::ini_parser_error error)
    {
        std::cout
                << error.message() << " : "
                << error.filename() << ", line "
                << error.line() << std::endl;
        LOG(ERROR) << "INI config file error: "
                   << error.message() << " : "
                   << error.filename() << ", line "
                   << error.line();
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
        LOG(INFO) << "Connect to DB is normal";
    }
    catch(DatabaseError& e)
    {
        std::cout << e.what() << std::endl;
        LOG(ERROR) << "DatabaseError: " << e.what();
    }
    //open log-file
    LogReader::ParserInotify log("/var/log/squid/access.log", &db);
    try
    {
        log.open();
        LOG(INFO) << "access.log file opened";

    }
    catch (LogReader::ParserError& e) {
        std::cout
                << e.what() << " "
                << e.filepath() << std::endl;
        LOG(ERROR) << "LogReader parse error: "
                   << e.what() << " : "
                   << e.filepath();
    }
    /*
    log.watch();
    */
    return 0;
}
