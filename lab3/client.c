/*
 * Darian Marvel
 * 3/2/2022
 * Client for lab 3
*/

// Headers
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

// Constants
#define PORT 8080


int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *string = "hello";
    char buffer[2048] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    // Make stdin non-blocking
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    // Make the socket non-blocking
    flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    /*
    send(sock , string , strlen(string) , 0 );
    printf("Hello message sent\n");
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
    */

    while(1) {

      errno = 0;
      int len = read(sock, buffer, 2048);

      if(errno == EAGAIN) {
        //continue...
      }
      else if(len == 0) {
        // socket is disconnected
        printf("Disconnected\n");
        close(sock);
        sock = -1;
      }
      else if (errno != 0) {
        perror("error on read");
        printf("%d", len);
        exit(EXIT_FAILURE);
      }
      else {
        buffer[len] = 0;
        printf("From Server: %s\n", buffer);
      }

      errno = 0;
      len = read(STDIN_FILENO, buffer, 2048);

      if(errno == EAGAIN) {
        //continue...
      }
      else {
        buffer[len] = 0;
        send(sock, buffer, strlen(buffer), 0);
      }

      usleep(1000);
    }

    return 0;
}
