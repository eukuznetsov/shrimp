#ifndef LOGREADER_H
#define LOGREADER_H

#include <string>
#include <fstream>
#include <exception>
#include <sys/types.h>

namespace LogReader
{

class InBaseReader
{
    std::ifstream file;
    std::string filePath;
    ino_t inode;
public:
    InBaseReader(char*);
    const char* filepath() { return filePath.c_str(); }
    void watch();
};

class InBaseReaderError:public std::exception
{
    std::string path;
    std::string message;
public:
    InBaseReaderError(char*, char*);
    const char* filepath() { return path.c_str(); }
    const char* what() { return message.c_str(); }
    ~InBaseReaderError() throw();
};

};
#endif // LOGREADER_H
