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
  @brief Base class for parsers of logs
  */
class BaseParser
{
protected:
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
    BaseParser(const char* pathToFile, MysqlDatabase* dbconn);
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
      @brief Check exists line of log in base or not
      @param line Line for search
      @return True if line already exists in database, false in otherwise
      */
    bool isAlreadyInBase(const std::string line) const;
    /**
      @brief Read the file until the end
      */
    void readFile();
    /**
      @brief Destructor
      */
    ~BaseParser() { file.close(); }
};

/**
  @brief Class reading log and writing parsed log in database. It using Inotify.
  */
class ParserInotify:public BaseParser
{
public:
    /**
      @brief Constructor
      */
    ParserInotify(const char* pathToFile, MysqlDatabase* dbconn):BaseParser(pathToFile, dbconn){}
    /**
      @brief Start tracking file
      @throw LogReader::InBaseReaderError
      */
    void watch();
};

/**
  @brief Exception for class InBaseReader
  */
class ParserError:public std::exception
{
    /**
      @brief Path to the log file
      */
    std::string path;
    std::string message;
public:
    /**
      @brief Constructor
      @param filePath Path to the log file
      @param messageText Text of message
      */
    ParserError(const char* filePath, const char* messageText)
    {
      path = filePath;
      message = messageText;
    };
    /**
      @brief Recieve path to the log file
      @return Path to the log file
      */
    const char* filepath() { return path.c_str(); }
    /**
      @brief Recieve message of exception
      @return Message
      */
    const char* what() { return message.c_str(); }
    ~ParserError() throw() {}
};

/**
  @brief Split string by separator-string
  @param sep Separator
  @param str Source string
  @param allowEmptyEntries Allow or not empty string in result list
  @return Source string separated in list of strings
  */
StringList split(const std::string sep, const std::string str, bool allowEmptyEntries);

/**
  @brief Search rotated logs
  @param path Path to the main log
  @return List of filenames of rotated logs. List empty in case of error or if rotated files not found.
  */
StringList findRotated(const std::string path);
}

#endif // LOGREADER_H
