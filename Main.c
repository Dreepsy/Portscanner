#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---Main---
int main()
{
    // ---Definition---
    char host[100];
    struct sockaddr_in target;
    struct hostent *he;     //*he is a pointer to a structure type of hostent (stores info about the host)

    //Userinput
    printf("Enter the target IP or hostname: ");
    scanf("%s", host);

    he = gethostbyname(host);   //gethostbyname resolves hostname or domain into IP

    //Errorhandling
    if (he == NULL)
    {
        printf("Error: Could not resolve the hostname. \n");
        return 1;
    }

    memcpy(&target.sin_addr, he->h_addr, he->h_length);     //he->h_addr contains the first IP found, he->h_length tells how many bytes to copy
    target.sin_family = AF_INET;    //Tells the system that its an IPv4

    printf("\nScannsing ports on %s (%s): \n", host, inet_ntoa(target.sin_addr)); //inet_ntoa(target.sin_addr) converts binary into string IP
    for(int port = 1; port <= 1024; port++)     //Checks which ports are open
    {
        int sock = socket(AF_INET, SOCK_STREAM, 0); //Creating a new socket
        if(sock < 0)    //Checks if socket call failed
        {
            perror("Socket");   //Prints error
            continue;   //If socket call fails, loop jumps to next port
        }

        //Set timeout
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000;
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));   //Sending timeout to the socket
        setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));   //Sending timeout to the socket

        if(connect(sock, (struct sockaddr *)&target, sizeof(target)) == 0)      //Tries to connect to the current port
        {
            printf("Port %d is open\n", port);  //Prints the port if the connect was succsessful
        }

        close(sock);    //closes socket
    }

    printf("Scan complete\n");
    return 0;
}