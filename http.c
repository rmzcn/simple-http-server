#include <stdio.h>
#include <stdlib.h>
#include "serve.c"

int main(int argc, char** argv)
{

    if (argc > 2 && argc < 0)
    {
        printf("You have to pass 2 arguments: IP and PORT where the server will run...");
        return -1;
    }

    serve(argv[1],argv[2]);
    
    return 0;
    
}