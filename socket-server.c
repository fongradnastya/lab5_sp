/*! \file    socket-server-udp.c
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
#include <unistd.h>
#include"matrix.c"

#define PORT 5005

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
    /* Recieve messages from any peers */
    int length = 0;

    /* First, read the length of the text message from the socket.  If
       read returns zero, the client closed the connection.  */
    int recvResult = recvfrom(serverSocket, &length, sizeof (length), 0,
                              (struct sockaddr *) &clientName,
                              &clientNameLength
                             );
    printf("%d\n", length);
    if (recvResult == -1)
    {
      perror("recvfrom");
    }
    if (0 >= recvResult)
    {
      return 0;
    }
    /* Allocate a buffer to hold the text.  */
    int** matrix = (int**) malloc(length * sizeof(int*));
    for(int i = 0; i < length; i++)
    {
        matrix[i] = (int*)malloc(length * sizeof(int));
    }   
    for(int i = 0; i < length; i++)
    {
        printf("%d\n", i);
        recvResult = recvfrom(serverSocket, matrix[i], length * sizeof(int), 0,
                          (struct sockaddr *)&clientName,
                          &clientNameLength
                         );
        if(recvResult < 0)
        {
            perror("recovfrom");
            return 0;
        }
    }
    printf("The matrix successfully read\n");
    PrintMatrix(matrix, length, length);
    matrixProcessing(matrix, length);
    free(matrix);
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
