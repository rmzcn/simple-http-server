#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getFileLength(FILE *data){
    fseek(data, 0, SEEK_END);
    int fileSize = ftell(data);
    fseek(data, 0, SEEK_SET);
    return fileSize;
}

char * getTextBasedContent(char * path)
{
    printf("GELEN PATH : %s\n", path);
    FILE *data = fopen(path, "r");

    if (data)
    {
        char line[120];
        char *responseData = (char*)calloc((size_t)getFileLength(data)+1,sizeof(char));
        while (fgets(line, 120, data) != 0) 
        {
            strcat(responseData, line);
        }

        fclose(data);
        return responseData;
    }

    else
    {
        return NULL;
    }  

}

char * getFilePathFromLocal(char * uri){
    char filePath[] = "httpdocs/";
    char * path = (char*)calloc(strlen(uri)+strlen(filePath)+2, sizeof(char));

    strcat(path, filePath);
    strcat(path, uri);

    return path;
}
