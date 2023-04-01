/*! \file    socket-client-udp.c
 *  \brief   Sockets demonstration.
 *
 *  \details Interprocess communication via UDP sockets.
 *           Write messages to the socket.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include"matrix.c"

void sendMatrix(int** matrix, int size, int socketFileDescriptor, struct sockaddr_in name)
{
    int resSend = 0;
    resSend = sendto(socketFileDescriptor, &size, sizeof (size), 0,
                     (struct sockaddr *) &name, sizeof (name)
                    );
    if (0 > resSend)
    {
        perror("sendto");
    }
    for(int i = 0; size < size; i++)
    {
        resSend = sendto(socketFileDescriptor, matrix[i], size * sizeof(int), 0,
                    (struct sockaddr *) &name, sizeof (name)
                );
        if (0 > resSend)
        {
            perror("sendto");
        }
    }
    printf("Matrix succesfully sended\n");
}

/*! \brief Main function
 *  \param argc  Number of command line arguments
 *  \param argv  An array of command line argruments.
 *               argv[0] - the program name,
 *               argv[1] - a socket IP-address.
 *               argv[2] - a socket port number.
 *  \return Integer 0 upon exit success,
 *          or EXIT_FAILURE otherwise.
 */
int main(int argc, const char* argv[])
{
  if (argc < 3)
  {
    fprintf(stderr, "Socketname and port number expected.\n");
    return EXIT_FAILURE;
  }

  int socketFileDescriptor;
  int portNumber = atoi(argv[2]);
  
  struct sockaddr_in name;
  memset((char *) &name, 0, sizeof (name));

  /* Store the server's name in the socket address.  */
  name.sin_family = AF_INET;
  name.sin_addr.s_addr = inet_addr(argv[1]);

  if (INADDR_NONE == name.sin_addr.s_addr)
  {
    printf("Wrong ");
    perror("inet_addr");
    exit(1);
  }
  name.sin_port = htons(portNumber);

  /* Create the socket.  */
  socketFileDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (0 > socketFileDescriptor)
    perror("socket");

  /* Write the text to the socket.  */
  do
  {
    int size = InputSize();
    int** matrix = (int**) malloc(size * sizeof(int*));
    for(int i = 0; i < size; i++)
    {
        matrix[i] = (int*)malloc(size * sizeof(int));
    }
    int res = InputMatrix(matrix, size);
    if(res == 1){
        printf("Matrix succesfully created\n");
        sendMatrix(matrix, size, socketFileDescriptor, name);
    }
    getchar();
  } while (!0);

  /* Disconnect and remove the socket. */
  close(socketFileDescriptor);
  return 0;
}
