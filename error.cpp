#include "reader.h"

Reader::Error::Error(char* filePath, char* messageText)
{
    path = filePath;
    message = messageText;
}

Reader::Error::~Error() throw()
{
}
