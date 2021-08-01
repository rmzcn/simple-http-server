#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "directives.h"
#include "headers.h"

#ifndef _DIRECTIVESH_
#define _DIRECTIVESH_
#endif


/*
    DESCRIPTION:

    The purpose of the methods included here is to generate HTTP headers and 
    generate an HTTP message.

*/


char * combineByHttpRules(char *dest, const char * src, const char * combiner);

char * createResponse(char * headers, char * content){

    char * p_response = (char*)calloc(strlen(headers)+strlen(content)+3, sizeof(char));

    strcat(p_response, headers);
    strcat(p_response, "\n\n");
    strcat(p_response, content);

    printf("RESPONSE : %s\n", p_response);

    return p_response;
}


char * createHeader(char * header_type, char * directives){
    if (strcmp(directives, D_NO_DIRECTIVE) == 0)
    {
        return header_type;
    }
    char * p_header;
    
    
    size_t msize = strlen(header_type)+strlen(directives)+2;
    p_header = (char*)calloc(msize, sizeof(char));
    
    strcat(p_header, header_type);
    strcat(p_header, ":");
    strcat(p_header, directives);

    return p_header;
}

char * createDirective(char * directive_type, char * value){
    if (strcmp(directive_type, D_VALUE) == 0)
    {
        return value;
    }

    size_t msize = strlen(directive_type)+strlen(value)+2;
    char * p_directive = (char*)calloc(msize,sizeof(char));


    strcat(p_directive, directive_type);
    strcat(p_directive, "=");
    strcat(p_directive, value);

    return p_directive;
}

char * combineDirectives(char * dest_directive, char * source_directive){

    return combineByHttpRules(dest_directive,source_directive,";");

}

char * combineHeaders(char * dest_header_s, char * source_header){

    return combineByHttpRules(dest_header_s,source_header,"\n");
    
}

char * combineByHttpRules(char *dest, const char * src, const char * combiner)
{
    const size_t szdest = strlen(dest);
    const size_t szsrc = strlen(src);
    const size_t szcombiner = strlen(combiner);
    const size_t sztotal = szdest + szsrc + szcombiner + 1;

    dest = realloc(dest, sztotal);

    memcpy(dest + szdest, combiner, szcombiner + 1);
    memcpy(dest + szdest + szcombiner, src, szsrc + 1);

    return dest;
}