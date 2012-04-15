#include "logreader.h"
#include <iostream>
#include "inotify-cxx.h"
#include <unistd.h>
#include <vector>
#include "mysqldatabase.h"

LogReader::InBaseReader::InBaseReader(const char* pathToFile, MysqlDatabase* dbconn)
{
    filePath = pathToFile;
    db=dbconn;
}

void LogReader::InBaseReader::open()
{
    std::cout << "Open " << filepath();
    unsigned int count = 0; //number of tryings
    unsigned int counts = 10; //all tryings
    file.open(filePath.c_str());

    //Try open log file. Waiting need, because for rotation of logs need many time.
    while((!file.is_open())&&(count < counts))
    {
        std::cout << ".";
        file.open(filePath.c_str());
        count++;
        if(count == 10) std::cout << std::endl << "Exit for timeout" << std::endl;

    }

    std::cout << std::endl;
}


void LogReader::InBaseReader::parse(const std::string notParsed) const
{
    std::string sep(" "); //separator for log line
    LogReader::StringList lecs = LogReader::split(sep, notParsed, false); //lecsems - it's result of spliting stirn by separator

    //Build query to the database for adding log line
    std::string query = "INSERT INTO shrimp.squid_log (time, duration, client_address, result_codes, bytes, request_method, url, rfc931, hierarchy_code, type) VALUES (";
    for(LogReader::StringList::iterator iter=lecs.begin(); iter<lecs.end(); iter++)
    {
        query += "\"";
        query += *iter;
        query += "\"";
        if (iter!=lecs.end()-1) {
            query += ",";
        }
    }
    query += ")";

    try{
        db->query(query.c_str());
    }
    catch(DatabaseError &e) {
        std::cout << e.what() << std::endl;
    }
}


void LogReader::InBaseReader::watch()
{
    try{
        //Inotify variables
        InotifyWatch watcher(filepath(), IN_MODIFY|IN_MOVE_SELF);
        Inotify inotifyFile;
        size_t count;

        inotifyFile.Add(&watcher);
        for(;;)
        {
            std::string line;
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
                        while (!file.eof())
                        {
                            getline(file,line);
                            if(!line.empty())
                            {
                                parse(line);
                            }
                        }
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
        throw LogReader::InBaseReaderError(filePath.c_str(), (e.GetMessage()).c_str());
    }
}
