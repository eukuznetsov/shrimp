#include "logreader.h"
#include <iostream>
#include "inotify-cxx.h"
#include <unistd.h>
#include <vector>
#include "mysqldatabase.h"
#include <boost/format.hpp>
#include <fstream>

LogReader::ParserInotify::ParserInotify(const char* pathToFile, MysqlDatabase* dbconn)
{
    filePath = pathToFile;
    db=dbconn;
}

void LogReader::ParserInotify::open()
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

    //search last line from log writed in database
    std::string logline;
    do {
        getline(file, logline);
    }while(isAlreadyInBase(logline));
    file.seekg(-(logline.size()+1), std::ios::cur);
    if(file.eof()) file.clear();
}


void LogReader::ParserInotify::parse(const std::string notParsed) const
{
    std::string sep(" "); //separator for log line
    LogReader::StringList lecs = LogReader::split(sep, notParsed, false); //lecsems - it's result of spliting stirn by separator

    //Build query to the database for adding log line
    std::string query = "INSERT INTO shrimp.squid_access_log (time, duration, client_address, result_codes, bytes, request_method, url, rfc931, hierarchy_code, type) VALUES (";
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
        throw LogReader::ParserError(filePath.c_str(), (e.GetMessage()).c_str());
    }
}

bool LogReader::ParserInotify::isAlreadyInBase(const std::string line) const
{
    QueryResult res;
    LogReader::StringList lecs = LogReader::split(" ", line, false); //lecsems - it's result of spliting string by separator
    std::string query = boost::str(boost::format("SELECT id FROM squid_access_log WHERE time=%d AND duration=%d AND client_address='%s' AND result_codes='%s' AND bytes=%d AND request_method='%s' AND url='%s' AND rfc931='%s' AND hierarchy_code='%s' AND type='%s'") % lecs.at(0) % lecs.at(1) % lecs.at(2) % lecs.at(3) % lecs.at(4) % lecs.at(5) % lecs.at(6) % lecs.at(7) % lecs.at(8) % lecs.at(9));
    try{
        res = db->query(query.c_str());
    }
    catch(DatabaseError &e) {
        std::cout << e.what() << std::endl;
    }
    if(res.empty()) {
        return false;
    }else {
        return true;
    }
}
