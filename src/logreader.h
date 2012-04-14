#ifndef LOGREADER_H
#define LOGREADER_H

#include <string>
#include <fstream>
#include <exception>
#include <vector>
#include <string>
#include "mysqldatabase.h"

namespace LogReader
{

typedef std::vector<std::string> StringList;

class InBaseReader
{
    MysqlDatabase* db;
    std::ifstream file;
    std::string filePath;
    void parse(const std::string) const;
public:
    InBaseReader(const char*, MysqlDatabase*);
    void open();
    const char* filepath() { return filePath.c_str(); }
    void watch();
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
