#include "inbasereader.h"
#include <iostream>
#include <fstream>

InBaseReader::InBaseReader(char* logPath)
{
    std::ifstream file;
    file.open(logPath);
    if(!file.is_open())
    {
        std::cout << "Can't open file " << logPath << std::endl;
    }
}
