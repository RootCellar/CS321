/*
* Darian Marvel
* 3/2/2022
* Server for lab 3
*/

// Headers
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

// Constants
#define PORT 8080


int main(int argc, char const *argv[])
{
  int server_fd;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[2048] = {0};
  char *string;

  //String constants
  char *clientsFull = "Sorry, no more clients can join!\n";
  char *needOther = "Only one client is connected!\n";

  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Forcefully attaching socket to the port 8080
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
    &opt, sizeof(opt)))
    {
      perror("setsockopt");
      exit(EXIT_FAILURE);
    }

    int flags = fcntl(server_fd, F_GETFL, 0);
    fcntl(server_fd, F_SETFL, flags | O_NONBLOCK);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
    sizeof(address))<0)
    {
      perror("bind failed");
      exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
      perror("listen failed");
      exit(EXIT_FAILURE);
    }

    int newSocket = -1;
    int client1 = -1;
    int client2 = -1;

    while(1) {

      // Attempt to Accept a connection

      errno = 0;
      newSocket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
      if(newSocket < 0 && errno == EAGAIN) {
        //continue...
      }
      else if(newSocket >= 0) {
        int flags = fcntl(newSocket, F_GETFL, 0);
        fcntl(newSocket, F_SETFL, flags | O_NONBLOCK);
        if(client1 < 0) {
          client1 = newSocket;
        }
        else if(client2 < 0) {
          client2 = newSocket;
        }
        else {
          // Both clients already joined -- cannot accept another
          printf("A third client attempted to connect\n");
          send(newSocket, clientsFull, strlen(clientsFull), 0);
        }
      }
      else {
        perror("accept failed");
        exit(EXIT_FAILURE);
      }

      // Client 1 sending to Client 2

      if(client1 > 0) {

        errno = 0;
        int len = read(client1, buffer, 2048);

        //perror("Read");
        //printf("%d\n", len);
        if(errno == EAGAIN) {
          //continue...
        }
        else if(len == 0) {
          // socket is disconnected
          printf("Client 1 disconnected\n");
          close(client1);
          client1 = -1;
        }
        else if (errno != 0) {
          perror("error on read");
          printf("%d", len);
          exit(EXIT_FAILURE);
        }
        else {

          printf("Client 1: %s\n", buffer);

          if(client2 < 0) {
            printf("We don't have the other client!\n");
            send(client1, needOther, strlen(needOther), 0);
          }
          else {
            buffer[len] = 0;
            send(client2, buffer, strlen(buffer), 0);
          }

        }

      }

      // Client 2 sending to Client 1

      if(client2 > 0) {

        errno = 0;
        int len = read(client2, buffer, 2048);

        //perror("Read");
        //printf("%d\n", len);
        if(errno == EAGAIN) {
          //continue...
        }
        else if(len == 0) {
          // socket is disconnected
          printf("Client 2 disconnected\n");
          close(client2);
          client2 = -1;
        }
        else if (errno != 0) {
          perror("error on read");
          printf("%d", len);
          exit(EXIT_FAILURE);
        }
        else {

          printf("Client 2: %s\n", buffer);

          if(client2 < 0) {
            printf("We don't have the other client!\n");
            send(client2, needOther, strlen(needOther), 0);
          }
          else {
            buffer[len] = 0;
            send(client1, buffer, strlen(buffer), 0);
          }

        }

      }

      usleep(1000);
    }

    /*
    valread = read( new_socket , buffer, 1024);
    printf("%s\n",buffer );
    send(new_socket , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    */

    return 0;
  }