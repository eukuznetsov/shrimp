#ifndef MYSQLDATABASE_H
#define MYSQLDATABASE_H

#include <mysql.h>

class MysqlDatabase
{
  MYSQL dbsession;
public:
  MysqlDatabase(const char*, const char*, const char*, const char*,
                int port=0,
                const char* unix_socket=0,
                unsigned long clientflag=0);
  ~MysqlDatabase() { mysql_close(&dbsession); }
};

#endif // MYSQLDATABASE_H
