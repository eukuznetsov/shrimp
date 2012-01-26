#include "logreader.h"
#include <iostream>
#include "inotify-cxx.h"
#include <sys/types.h>
#include <sys/stat.h>

LogReader::InBaseReader::InBaseReader(const char *pathToFile)
{
    filePath = pathToFile;
}

void LogReader::InBaseReader::open()
{
    file.open(filePath.c_str());
    if(!file.is_open())
    {
        throw LogReader::InBaseReaderError(filePath.c_str(), "Can't open");
    }
    //get time of creation file
    struct stat fileStatus;
    if (lstat(filePath.c_str(), &fileStatus)!=0)
    {
        throw LogReader::InBaseReaderError(filePath.c_str(), "Can't read information about file");
    }
    inode = fileStatus.st_ino;
}

void LogReader::InBaseReader::watch()
{
    InotifyWatch watcher(filepath(), IN_MODIFY);
    Inotify inotifyFile;
    inotifyFile.Add(&watcher);
    for(;;)
    {
        inotifyFile.WaitForEvents();
    }
}
