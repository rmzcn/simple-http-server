#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <pthread.h>
#include <semaphore.h>

#include "local.h"
#include "responsesetter.h"
#include "headers.h"
#include "response.h"
#include "dpages.h"
#include "contenttypes.h"

#ifndef _DIRECTIVESH_
#define _DIRECTIVESH_
#endif

#define DEFAULT_PAGE "index.html"


char * _getTestResponseHeaders(char * responseCode, char * content, char * contentType);
char** parseRequestLine(char * bufLine);




// sem_t lock;
// pthread_mutex_t mutex;


int handle(int * clientSocket)
{
    int BUFFSIZE = 4096;
    FILE * stream; // for getting request from client
    char * responseContent;
    char * response;
    char * method;
    char * uri;
    char * version;
    char ** requestLine;
    char *buf = calloc(BUFFSIZE, sizeof(char));

    //open the child socket descriptor as a stream
    if ((stream = fdopen(*clientSocket, "r+")) == NULL){
        printf("ERROR: Request not recieved\n");
    }

    /*
        WARNING:
        DO NOT CLOSE THE 'stream'. IF TURNED OFF, CLIENT SOCKET IS REFUSED.
    */
    
    fgets(buf, BUFFSIZE, stream);

    requestLine = parseRequestLine(buf);
    uri = *(requestLine+1);
    method = *(requestLine);

    printf("URI : %s \n", uri);

    
    /**********************************************************************************/
    if (uri == NULL)
    {
        responseContent = NULL;
    }
    else if (strcmp(uri, "/") == 0)
    {   
        responseContent = getTextBasedContent(getFilePathFromLocal(DEFAULT_PAGE));

    }
    else
    {
        if (uri[0] == '/') uri++;
        responseContent = getTextBasedContent(getFilePathFromLocal(uri));

    }


    if (responseContent == NULL)
    {
        if (uri == NULL)
        {
            response = _getTestResponseHeaders(INTERNAL_SERVER_ERROR, PAGE_500, "text/html");
        }
        else
        {
            response = _getTestResponseHeaders(NOT_FOUND, PAGE_404, "text/html");
        }        
    }

    else
    {
        if (strcmp(get_filename_ext(uri), "css") == 0)
        {
            response = _getTestResponseHeaders(OK, responseContent, CONTENT_TYPE_CSS);
        }
        else if (strcmp(get_filename_ext(uri), "js") == 0)
        {
            response = _getTestResponseHeaders(OK, responseContent, CONTENT_TYPE_JS);
        }
        
        else if (strcmp(get_filename_ext(uri), "html") == 0 || strcmp("/", uri) == 0)
        {
            response = _getTestResponseHeaders(OK, responseContent, CONTENT_TYPE_HTML);
        }
        else
        {
            printf("URI : %s\n", uri);
            response = _getTestResponseHeaders(OK, responseContent, CONTENT_TYPE_PLAIN_TEXT);
        }
    }
    /**********************************************************************************/

    int isSend = send(*clientSocket, response, strlen(response), 0);

    if(isSend == -1) printf("DATA CAN NOT SENDED !\n");
    else  printf("Sended.\n\n");
    
    close(*clientSocket);
    free(buf);
}


char * _getTestResponseHeaders(char * responseCode, char * content, char * contentType){
    char * headers = (char *)calloc(strlen(responseCode), sizeof(char));

    /*
        WARNING:
        Content-Length can take a value of -1, check that.  
    */
    
    char p_contentLength[10];
    int contentLength = strlen(content);
    sprintf(p_contentLength, "%d", contentLength);

    headers = combineHeaders(headers,createHeader(responseCode,D_NO_DIRECTIVE));
    headers = combineHeaders(headers,createHeader(H_DATE, createDirective(D_VALUE, getDate())));
    headers = combineHeaders(headers,createHeader(H_CONTENT_TYPE, createDirective(D_VALUE, contentType)));
    headers = combineHeaders(headers,createHeader(H_SERVER, createDirective(D_VALUE, "ramache")));
    headers = combineHeaders(headers,createHeader(H_CONTENT_LENGTH, createDirective(D_VALUE, p_contentLength)));

    return createResponse(headers, content);
        
}


char** parseRequestLine(char * bufLine)
{
    char* token;
    char ** arr = (char**)calloc(3,sizeof(char*));
    int i = 0;
    while ((token = strtok_r(bufLine, " ", &bufLine)))
    {
        *(arr+i) = token;
        i++;
    }

    return arr; 
}