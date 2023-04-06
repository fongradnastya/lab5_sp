/*! \file    socket-server.c
 *  \brief   Sockets demonstration.
 *
 *  \details Interprocess communication via internet UDP sockets.
 *           Read messages and print it out until client sent "adios amigo".
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include"matrix.h"

#define PORT 5005
#define BUFFSIZE 4048

/*! \brief Read text from socket and print it.
 *
 *  \details Read text from the socket and print it out.
 *           Continue until the socket closes.
 *  \param  serverSocket     Listening socket descriptor
 *  \return non-zero, if the client sent a "adios amigo" message,
 *          zero otherwise.
 */
int server(int serverSocket)
{
  struct sockaddr_in clientName;
  socklen_t clientNameLength = sizeof (clientName);
  while (!0)
  {
    char buffer[BUFFSIZE];
    int recvResult = recvfrom(serverSocket, buffer, BUFFSIZE, 0,
                              (struct sockaddr *) &clientName,
                              &clientNameLength
                             );
    printf("Received a request from %s:%d\n", inet_ntoa(clientName.sin_addr), ntohs(clientName.sin_port));
    buffer[recvResult] = '\0';
    printf("Request content: %s\n", buffer);
    printf("Response:\n");
    if (recvResult == -1)
    {
      perror("recvfrom");
    }
    else if (recvResult == 0)
    {
      return 0;
    }
    getMatrixFromString(buffer);
  }
  return 0;
}

int main(int argc, char* const argv[])
{
  int socketFileDescriptor = -1;
  
  struct sockaddr_in name;
  int clientSentQuitMessage;

  /* Create the socket.  */
  socketFileDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  int i = 1;
  printf("Socket created\n");
  setsockopt(socketFileDescriptor, SOL_SOCKET, SO_REUSEADDR,
             (const char *)&i, sizeof (i)
            );

  bzero((char *)&name, sizeof(name));

  /* Indicate this is a server.  */
  name.sin_family = AF_INET;
  name.sin_port = htons(PORT);
  name.sin_addr.s_addr = INADDR_ANY;

  if (-1 == bind(socketFileDescriptor, (const struct sockaddr *)&name, sizeof (name)))
  {
    perror("bind ");
    close(socketFileDescriptor);
    exit(1);
  }
  printf("Started listening client's requests\n");
  /* Handle the connection.  */
  do
  {
    clientSentQuitMessage = server(socketFileDescriptor);
  } while (!clientSentQuitMessage);

  /* Close the socket.  */
  close(socketFileDescriptor);

  return 0;
}
