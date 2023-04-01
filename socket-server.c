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
    char* text = NULL;

    /* First, read the length of the text message from the socket.  If
       read returns zero, the client closed the connection.  */
    int recvResult = recvfrom(serverSocket, &length, sizeof (length), 0,
                              (struct sockaddr *) &clientName,
                              &clientNameLength
                             );
    if (-1 == recvResult)
    {
      perror("recvfrom");
    }
    if (0 >= recvResult)
    {
      return 0;
    }
    /* Allocate a buffer to hold the text.  */
    text = (char*) malloc(length);
    bzero(text, length);

    /* Read the text itself, and print it.  */
    recvResult = recvfrom(serverSocket, text, length, 0,
                          (struct sockaddr *)&clientName,
                          &clientNameLength
                         );
    printf("%s\n", text);

    /* If the client sent the message "adios amigo", we're all done.  */
    if (0 == strcmp(text, "adios amigo"))
    {
      /* Free the buffer.  */
      free(text);
      return !0;
    }

    /* Free the buffer.  */
    free(text);
  }
  return 0;
}

/*! \brief Main function
 *  \param argc  Number of command line arguments
 *  \param argv  An array of command line argruments.
 *               argv[0] - the program name,
 *               argv[1] - the port number.
 *  \return Integer 0 upon exit success,
 *          or EXIT_FAILURE otherwise.
 */
int main(int argc, char* const argv[])
{
  if (argc < 2)
  {
    fprintf(stderr, "Too few parameters.\n");
    return EXIT_FAILURE;
  }

  int socketFileDescriptor = -1;
  int portNumber = atoi(argv[1]);
  
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
  name.sin_port = htons(portNumber);
  name.sin_addr.s_addr = INADDR_ANY;

  if (-1 == bind(socketFileDescriptor, (const struct sockaddr *)&name, sizeof (name)))
  {
    perror("bind ");
    close(socketFileDescriptor);
    exit(1);
  }

  /* Handle the connection.  */
  do
  {
    clientSentQuitMessage = server(socketFileDescriptor);
  } while (!clientSentQuitMessage);

  /* Close the socket.  */
  close(socketFileDescriptor);

  return 0;
}
