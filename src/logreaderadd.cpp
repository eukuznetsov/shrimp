#include "logreader.h"
#include <list>
#include <string>
#include <boost/filesystem.hpp>
#include <algorithm>
#include <boost/regex.hpp>

LogReader::StringList LogReader::split( const std::string sep, const std::string str,
                  bool allowEmptyEntries)
{
    StringList lst;
    int j = 0;
    int i = str.find( sep, j );
    while ( i != -1 ) {
        if ( i > j && i <= (int)str.length() )
            lst.push_back( str.substr( j, i - j ) );
        else if ( allowEmptyEntries )
            lst.push_back( "" );
        j = i + sep.length();
        i = str.find( sep, sep.length() > 0 ? j : j+1 );
    }
    int l = str.length() - 1;
    if ( str.substr( j, l - j + 1 ).length() > 0 )
        lst.push_back( str.substr( j, l - j + 1 ) );
    else if ( allowEmptyEntries )
        lst.push_back( "" );
    return lst;
}

LogReader::StringList LogReader::find_rotated(const std::string path)
{
    StringList rotated;
    std::list<boost::filesystem::path> files; //all files in log's directory
    boost::filesystem::path log_path=path;

    //create listing of directory
    std::copy(boost::filesystem::directory_iterator(log_path.remove_filename()),
              boost::filesystem::directory_iterator(),
              std::back_inserter(files));

    //search rotated logs in listing of directory by regular expression
    boost::regex path_regexp("^"+path+".[0-9]*$");
    for(std::list<boost::filesystem::path>::iterator file = files.begin();
        file!=files.end();
        file++)
    {
        if (boost::regex_match((*file).leaf(), path_regexp))
        {
            rotated.push_back((*file).leaf());
        }
    }

    return rotated;
}
