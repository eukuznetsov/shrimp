#include "logreader.h"
#include <iostream>
#include "inotify-cxx.h"
#include <sys/types.h>
#include <sys/stat.h>

LogReader::InBaseReader::InBaseReader(char* pathToFile)
{
    filePath = pathToFile;
    file.open(pathToFile);
    if(!file.is_open())
    {
        throw LogReader::InBaseReaderError(pathToFile, "Can't open");
    }
    //get time of creation file
    struct stat fileStatus;
    if (lstat(pathToFile, &fileStatus)!=0)
    {
        throw LogReader::InBaseReaderError(pathToFile, "Can't read information about file");
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
