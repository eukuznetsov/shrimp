#ifndef LOGREADER_H
#define LOGREADER_H

#include <string>
#include <fstream>
#include <exception>

namespace LogReader
{

class InBaseReader
{
    std::ifstream file;
    std::string filePath;
public:
    InBaseReader(const char*);
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

};
#endif // LOGREADER_H
