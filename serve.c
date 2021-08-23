#include "handle.c"


int setupServiceSocket(char* ip, char* port);
int startListening(int* serverSocket, int backlog);
int run(int *serverSocket);
void getAddress(struct sockaddr *address);


// main function for http server
void serve(char* ip, char* port)
{
    int serverSocket, listening, running;
    serverSocket = setupServiceSocket(ip, port);

    listening = startListening(&serverSocket, 100);
    if (listening != 0)
    {
        printf("> ERROR: Server not listening.");
    }
    else
    {
        printf("> LISTENING --> http://%s: %s\n", ip, port);
    }
    
    running = run(&serverSocket);

    if (running == -1)
    {
        printf("> ERROR: Server is not running.");
    }
    
}


int setupServiceSocket(char* ip, char* port){

    int serverSocket = socket(
        AF_INET,      
        SOCK_STREAM,  
        0
    );

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(atoi(port));
    serverAddress.sin_addr.s_addr = inet_addr(ip);

    bind(
        serverSocket,
        (struct sockaddr *) &serverAddress,
        sizeof(serverAddress)
    );
    
    return serverSocket;

}

int startListening(int* serverSocket, int backlog){
    int listening = listen(*serverSocket, backlog);
    if (listening < 0) {
        return -1;
    }
    return 0;
}


int run(int *serverSocket){

   while(1) {
        int clientSocket;
        struct sockaddr clientSocketAddr;
        socklen_t clientSocketAddrLen;

        clientSocket = accept(*serverSocket, &clientSocketAddr, &clientSocketAddrLen);
        getAddress(&clientSocketAddr); // for log
        
        handle(&clientSocket);  
    }

    return -1;
}


void getAddress(struct sockaddr *address)
{
    char hostBuffer[INET_ADDRSTRLEN+1];
    char serviceBuffer[NI_MAXSERV+1];
    socklen_t addr_len = sizeof(*address);
    int err = getnameinfo(
        address,
        addr_len,
        hostBuffer,
        sizeof(hostBuffer),
        serviceBuffer,
        sizeof(serviceBuffer),
        NI_NUMERICHOST
    );
    if (err != 0) {
        // TODO ->  Write Logger funciton here 
        printf("Address translition error!\n");
    }

    else
    {
        // TODO ->  Write Logger funciton here 
        printf(">GET from --> %s:%s\n",hostBuffer,serviceBuffer);
    }
}
