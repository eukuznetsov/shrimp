#ifndef DATABASEEXCP_H
#define DATABASEEXCP_H

#include <exception>
#include <string>

/**
  @brief Exception for DatabaseError
  */
class DatabaseError:public std::exception
{
    std::string message;
public:
    DatabaseError(const char* msg) { message = msg; }
    /**
      @brief Get message text
      @return Message text
      */
    const char* what() { return message.c_str(); }
    ~DatabaseError() throw() {}
};

#endif // DATABASEEXCP_H
