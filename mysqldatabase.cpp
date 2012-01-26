#include "mysqldatabase.h"
#include "databaseexcp.h"
#include <iostream>
#include <string>
#include <mysql.h>
#include <my_global.h>

MysqlDatabase::MysqlDatabase(const char* host,
                             const char* user,
                             const char* password,
                             const char* database,
                             int port,
                             const char* unix_socket,
                             unsigned long clientflag)
{
    if(mysql_init(&dbsession)==NULL)
    {
        mysql_close(&dbsession);
        throw DatabaseError("Connection to mysql database creating failed");
    }
    if(mysql_real_connect(&dbsession, host, user, password,
                          database, port, unix_socket, clientflag) == NULL)
    {
        std::string err(mysql_error(&dbsession));
        mysql_close(&dbsession);
        throw DatabaseError(err.c_str());
    }
}
