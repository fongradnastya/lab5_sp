/*! \file    socket-client.c
 *  \brief   Клиентская часть программы
 *
 *  \details Создаёт сокет, считывающий пользовательский ввод 
 * и посылающий запросы другим сокетам
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include"matrix.h"
#include"input.h"

#define ADDRES "127.0.0.1"
#define PORT 5005
#define BUFFSIZE 4048

/*! \brief Записывает матрицу в буффер и отправляет его на сервер
 *
 *  \param  matrix матрица для отправки
 *  \param  size размер матрицы
 *  \param  socketFileDescriptor дескриптор сокета
 *  \param  name имя и адрес сервера
 */
void sendMatrix(int** matrix, int size, int socketFileDescriptor, struct sockaddr_in name)
{
  char buffer[BUFFSIZE];
  sprintf(buffer, "%d", size);
  for(int i = 0; i < size; i++)
  {
    for(int j = 0; j < size; j++)
    {
      sprintf(buffer, "%s %d", buffer, matrix[i][j]);
    }
  }
  int resSend = 0;
  resSend = sendto(socketFileDescriptor, &buffer, 
    strlen(buffer), 0, (struct sockaddr *) &name, sizeof (name));
  if (0 > resSend)
  {
    perror("sendto");
  }
}

/*! \brief Запускает работу клиентского приложения
 *  \return код завершения выполнения
*/
int main()
{
  int socketFileDescriptor;
  
  struct sockaddr_in name;
  memset((char *) &name, 0, sizeof (name));

  /* Store the server's name in the socket address.  */
  name.sin_family = AF_INET;
  name.sin_addr.s_addr = inet_addr(ADDRES);

  if (INADDR_NONE == name.sin_addr.s_addr)
  {
    perror("inet_addr");
    exit(1);
  }
  name.sin_port = htons(PORT);

  /* Create the socket.  */
  socketFileDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (0 > socketFileDescriptor)
    perror("socket");

  printf("---------------------------------------\n");
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
        printf("The matrix was sended to the server\n");
    }
    getchar();
    for(int i = 0; i < size; i++)
    {
      free(matrix[i]);
    }
    free(matrix);
    printf("---------------------------------------\n");
  } while (!0);

  /* Disconnect and remove the socket. */
  close(socketFileDescriptor);
  return 0;
}
