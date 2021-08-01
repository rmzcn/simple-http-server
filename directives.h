#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define D_VALUE "<value>"

#define D_NO_DIRECTIVE "<no directive>"

#define D_CONNECTION_KEEP_ALIVE "keep-alive"
#define D_CONNECTION_CLOSE "CLOSE"

#define D_CONTENT_TYPE_CHARSET "charset"

#define D_KEEP_ALIVE_TIMEOUT "timeout"
#define D_KEEP_ALIVE_MAX "max"



// int getContentLength(FILE *file){
//     fseek(file, 0, SEEK_END); // seek to end of file
//     long fileSize = ftell(file); // get current file pointer
//     fseek(file, 0, SEEK_SET);
//     return fileSize;
// }

char * getDate(){

    char * buffer = (char*)malloc(100);
    time_t now = time(0);
    struct tm tm = *gmtime(&now);

    strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S %Z", &tm);

    return buffer;
}