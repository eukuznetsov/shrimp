#ifndef MYSQLDATABASE_H
#define MYSQLDATABASE_H

#include <mysql.h>
#include <databaseexcp.h>
#include <vector>
#include <string>

typedef std::vector<std::vector<std::string> > QueryResult;

class MysqlDatabase
{
  MYSQL dbsession;
public:
  MysqlDatabase() {}
  void connect(const char*, const char*, const char*, const char*,
               int port=0,
               const char* unix_socket=0,
               unsigned long clientflag=0);
  std::vector<std::vector<std::string> > query(const char*);
  ~MysqlDatabase() { mysql_close(&dbsession); }
};

#endif // MYSQLDATABASE_H
