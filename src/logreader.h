#ifndef LOGREADER_H
#define LOGREADER_H

#include <string>
#include <fstream>
#include <exception>
#include <vector>
#include <string>
#include "mysqldatabase.h"

/**
  namespace LogReader
  @brief Namespace of classes reading logs
  */
namespace LogReader
{

/**
  @brief List of strings type
  */
typedef std::vector<std::string> StringList;

/**
  @brief Class reading log and writing parsed log in database
  */
class InBaseReader
{
    /**
      @brief Session identificator
      */
    MysqlDatabase* db;
    /**
      @brief Log file
      */
    std::ifstream file;
    /**
      @brief Log file path
      */
    std::string filePath;
    /**
      @brief Parse log line and write it in database
      @param notParsed String for parsing
      */
    void parse(const std::string notParsed) const;
public:
    /**
      @brief Constructor
      @param pathToFile Path to the log file
      @param dbconn Connection to database
      */
    InBaseReader(const char* pathToFile, MysqlDatabase* dbconn);
    /**
      @brief Opening log file
      */
    void open();
    /**
      @brief Recieve path to the watched log file
      @return Path to the log file
      */
    const char* filepath() { return filePath.c_str(); }
    /**
      @brief Start tracking file
      */
    void watch();
    /**
      @brief Destructor
      */
    ~InBaseReader() { file.close(); }
};

class InBaseReaderError:public std::exception
{
    std::string path;
    std::string message;
public:
    InBaseReaderError(const char* filePath, const char* messageText)
    {
      path = filePath;
      message = messageText;
    };
    const char* filepath() { return path.c_str(); }
    const char* what() { return message.c_str(); }
    ~InBaseReaderError() throw() {}
};

StringList split(const std::string, const std::string, bool);

};
#endif // LOGREADER_H
