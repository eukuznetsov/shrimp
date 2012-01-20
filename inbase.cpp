#include "reader.h"
#include <iostream>
#include "inotify-cxx.h"
#include <sys/types.h>
#include <sys/stat.h>

Reader::InBase::InBase(char* pathToFile)
{
    filePath = pathToFile;
    file.open(pathToFile);
    if(!file.is_open())
    {
        throw Reader::Error(pathToFile, "Can't open");
    }
    //get time of creation file
    struct stat fileStatus;
    if (lstat(pathToFile, &fileStatus)!=0)
    {
        throw Reader::Error(pathToFile, "Can't read information about file");
    }
    inode = fileStatus.st_ino;
}

void Reader::InBase::watch()
{
    InotifyWatch watcher(filepath(), IN_MODIFY);
    Inotify inotifyFile;
    inotifyFile.Add(&watcher);
    for(;;)
    {
        inotifyFile.WaitForEvents();
    }
}
