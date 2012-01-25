#ifndef BASEDATABASE_H
#define BASEDATABASE_H

class BaseDatabase{
public:
    virtual QueryResult query(char*);
};class Error:public std::exception{
    std::string message;
public:
    Error(char*);
    char* what();
};

}
#endif // BASEDATABASE_H
