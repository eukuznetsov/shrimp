#ifndef LOG_H
#define LOG_H

#include <fstream>

using namespace std;

class Log{
public:
    Log(char *filename);
    ~Log();

    void Write(const char *logline, ...);
private :
    ofstream m_stream;
};

#endif // LOG_H
