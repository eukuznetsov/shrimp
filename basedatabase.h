#ifndef BASEDATABASE_H
#define BASEDATABASE_H

#include "queryresult.h"

class BaseDatabase{
public:
    virtual QueryResult query(char*);
};

#endif // BASEDATABASE_H
