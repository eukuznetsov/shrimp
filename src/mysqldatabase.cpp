#include "mysqldatabase.h"
#include "databaseexcp.h"
#include <iostream>
#include <string>
#include <mysql.h>
#include <vector>
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

std::vector<std::vector<std::string> > MysqlDatabase::query(const char* query)
{
    QueryResult all_res; //all rows
    std::vector<std::string> result; // one row
    if(mysql_query(&dbsession, query)!=0)
    {
        throw DatabaseError(mysql_error(&dbsession));
    }
    if(mysql_field_count(&dbsession) > 0) {
        MYSQL_RES *res;
        MYSQL_ROW row;
        unsigned int i;
        unsigned int num_fields;
        res = mysql_store_result(&dbsession);
        if (mysql_errno(&dbsession) > 0) {
            throw DatabaseError("Error: Can't fetch result");
        }
        else {
            num_fields = mysql_num_fields(res);
            while((row = mysql_fetch_row(res)) != NULL)
            {
                //get row
                for (i = 0; i<num_fields; i++)
                {
                    result.push_back(row[i]);
                }
                all_res.push_back(result);
                result.clear();
            }
        }
    }
    return all_res;
}
