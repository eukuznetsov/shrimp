#include "reader.h"
#include <iostream>

Reader::InBase::InBase(char* pathToFile)
{
    filePath = pathToFile;
    file.open(pathToFile);
    if(!file.is_open())
    {
        throw Reader::Error(pathToFile, "Can't open");
    }
}
