#include "logreader.h"
#include <vector>
#include <string>

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
