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

#include "local.h"
#include "responsesetter.h"
#include "headers.h"
#include "response.h"
#include "dpages.h"

#ifndef _DIRECTIVESH_
#define _DIRECTIVESH_
#endif

#define DEFAULT_PAGE "index.html"


char * _getTestResponseHeaders(char * responseCode, char * content);
char** parseRequestLine(char * bufLine);


// int handle(int * serverSocket, int * clientSocket)

void * handle(void * clientfd)
{
    //new 
    int * clientSocket = (int*)clientfd;
    //new 

    int BUFFSIZE = 1024;
    FILE * stream; // for getting request from client
    char * responseContent;
    char * response;
    char * method;
    char * uri;
    char * version;
    char ** requestLine;
    char *buf = calloc(BUFFSIZE, sizeof(char));


    // open the child socket descriptor as a stream
    if ((stream = fdopen(*clientSocket, "r")) == NULL)
        printf("ERROR: Request not recieved");
    /*
        WARNING:
        DO NOT CLOSE THE 'stream'. IF TURNED OFF, CLIENT SOCKET IS REFUSED.
    */

    // get the HTTP request line
    fgets(buf, BUFFSIZE, stream);

    requestLine = parseRequestLine(buf);
    uri = *(requestLine+1);
    method = *(requestLine);


    /**********************************************************************************/
    if (strcmp(uri, "/") == 0)
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
        response = _getTestResponseHeaders(NOT_FOUND, PAGE_404);
    }

    else
    {
        response = _getTestResponseHeaders(OK, responseContent);
    }
    /**********************************************************************************/



    int isSend = send(*clientSocket, response, strlen(response), 0);

    if(isSend == -1) printf("DATA CAN NOT SENDED !");

    close(*clientSocket);
    free(buf);
    pthread_exit(NULL);
    // free(uri);
    // free(responseContent);
}


char * _getTestResponseHeaders(char * responseCode, char * content){
    char * headers = (char *)calloc(strlen(responseCode), sizeof(char));

    /*
        WARNING:
        Content-Length can take a value of -1 sometimes, check that.  
    */
    
    char p_contentLength[10];
    int contentLength = strlen(content);
    sprintf(p_contentLength, "%d", contentLength);
    
    char * head = createHeader(H_CONTENT_TYPE, createDirective(D_VALUE, "text/html; charset=utf-8"));

    headers = combineHeaders(headers,createHeader(responseCode,D_NO_DIRECTIVE));
    headers = combineHeaders(headers,createHeader(H_DATE, createDirective(D_VALUE, getDate())));
    headers = combineHeaders(headers,createHeader(H_CONTENT_TYPE, createDirective(D_VALUE, "text/html; charset=utf-8")));
    headers = combineHeaders(headers,createHeader(H_SERVER, createDirective(D_VALUE, "ramache")));
    headers = combineHeaders(headers,createHeader(H_CONTENT_LENGTH, createDirective(D_VALUE, p_contentLength)));

    return createResponse(headers, content);
        
}


char** parseRequestLine(char * bufLine)
{
    char* token;
    char ** arr = (char**)malloc(sizeof(char*)*3);
    int i = 0;
    while ((token = strtok_r(bufLine, " ", &bufLine)))
    {
        *(arr+i) = token;
        i++;
    }

    return arr; 
}