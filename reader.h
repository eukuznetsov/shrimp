#ifndef READER_H
#define READER_H

#include <string>
#include <fstream>
#include <exception>
#include <sys/types.h>

namespace Reader
{

class InBase
{
    std::ifstream file;
    std::string filePath;
    ino_t inode;
public:
    InBase(char*);
    const char* filepath() { return filePath.c_str(); }
};

class Error:public std::exception
{
    std::string path;
    std::string message;
public:
    Error(char*, char*);
    const char* filepath() { return path.c_str(); }
    const char* what() { return message.c_str(); }
    ~Error() throw();
};

};
#endif // READER_H
