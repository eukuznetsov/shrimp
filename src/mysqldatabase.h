#ifndef MYSQLDATABASE_H
#define MYSQLDATABASE_H

#include <mysql.h>
#include <databaseexcp.h>
#include <vector>
#include <string>

/**
  @brief Result of query to database
  */
typedef std::vector<std::vector<std::string> > QueryResult;

class MysqlDatabase
{
  MYSQL dbsession;
public:
  MysqlDatabase() {}
  /**
    @brief Connect to database
    @param host Host for database connection
    @param user Username
    @param password Password of user
    @param database Name of database
    @param port Port for connection
    @param unix_socket UNix socket. Default to 0.
    @param clientflag Client's flag for connection. Default to 0.
    */
  //WTF? is clientflag
  void connect(const char* host,
               const char* user,
               const char* password,
               const char* database,
               int port,
               const char* unix_socket=0,
               unsigned long clientflag=0);
  std::vector<std::vector<std::string> > query(const char*);
  ~MysqlDatabase() { mysql_close(&dbsession); }
};

#endif // MYSQLDATABASE_H
