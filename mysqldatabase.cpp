#include "mysqldatabase.h"
#include "databaseexcp.h"
#include <iostream>
#include <string>
#include <mysql.h>
#include <my_global.h>

void MysqlDatabase::connect(const char* host,
                            const char* user,
                            const char* password,
                            const char* database,
                            int port,
                            const char* unix_socket,
                            unsigned long clientflag)
{
    if(mysql_init(&dbsession)==NULL)
    {
        throw DatabaseError("Connection to mysql database creating failed");
    }
    if(mysql_real_connect(&dbsession, host, user, password,
                          database, port, unix_socket, clientflag) == NULL)
    {
        throw DatabaseError(mysql_error(&dbsession));
    }
}

void MysqlDatabase::query(const char* query)
{
    if(mysql_query(&dbsession, query)!=0)
    {
        std::cout << mysql_error(&dbsession) <<std::endl;
        throw DatabaseError("");
    }
}
