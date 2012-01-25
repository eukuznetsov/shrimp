#include "logreader.h"

LogReader::InBaseReaderError::InBaseReaderError(char* filePath, char* messageText)
{
    path = filePath;
    message = messageText;
}
