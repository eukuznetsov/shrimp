#include "logreader.h"
#include <iostream>
#include "inotify-cxx.h"

void LogReader::ParserInotify::watch()
{
    try{
        //Inotify variables
        InotifyWatch watcher(filepath(), IN_MODIFY|IN_MOVE_SELF);
        Inotify inotifyFile;
        size_t count;

        inotifyFile.Add(&watcher);
        for(;;)
        {
            std::string eventMask;
            inotifyFile.WaitForEvents();
            count = inotifyFile.GetEventCount();
            while(count != 0)
            {
                InotifyEvent event;
                bool got_event = inotifyFile.GetEvent(&event);
                if(got_event)
                {
                    event.DumpTypes(eventMask);

                    //File was move (rename)
                    if(eventMask=="IN_MOVE_SELF")
                    {
                        std::cout << "Logs rotated." << std::endl;
                        file.close();
                        open();
                        inotifyFile.Remove(watcher);
                        InotifyWatch watcher(filepath(), IN_MODIFY|IN_MOVE_SELF);
                        inotifyFile.Add(&watcher);
                    }

                    //File modified
                    if(eventMask=="IN_MODIFY")
                    {
                        readFile();
                        file.clear();
                        break;
                    }

                }
                count--;
            }
        }
    }
    catch (InotifyException& e)
    {
        throw LogReader::ParserError(filePath.c_str(), (e.GetMessage()).c_str());
    }

}
