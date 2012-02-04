#ifndef DATABASEEXCP_H
#define DATABASEEXCP_H

#include <exception>
#include <string>

class DatabaseError:public std::exception
{
    std::string message;
public:
    DatabaseError(const char* msg) { message = msg; }
    const char* what() { return message.c_str(); }
    ~DatabaseError() throw() {}
};

#endif // DATABASEEXCP_H
