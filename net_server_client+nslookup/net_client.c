#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define SIM_LENGTH 10
#define PORT 1337

int main(int argc, char* argv[]){

  int                   sock;
  struct sockaddr_in    srv_name, *res_name;
  struct addrinfo       *res;
  char                  *hostname;
  int                   count;
  int                   value;


  if(argc != 2){
    printf("Usage: %s <HOSTNAME>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  hostname = argv[1];

  if (0 != getaddrinfo(hostname, NULL, NULL, &res)) {
    fprintf(stderr, "Error in resolving hostname %s\n", hostname);
    exit(1);
  }

  printf("Client is alive and establishing socket connection.\n");


  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0){
      perror ("Error opening channel");
      close(sock);
      exit(1);
  }


  bzero(&srv_name, sizeof(srv_name));
  res_name = (struct sockaddr_in *)res->ai_addr;
  srv_name.sin_port = htons(PORT);
  srv_name.sin_family = AF_INET;
  srv_name.sin_addr = res_name->sin_addr;
  printf("cli addr : %s\n", inet_ntoa(srv_name.sin_addr));


  if (connect(sock, (struct sockaddr *)&srv_name, sizeof(srv_name)) < 0){
      perror ("Error establishing communications");
      close(sock);
      exit(1);
  }


  for (count = 1; count <= SIM_LENGTH; count++){
      read(sock, &value, 4);
      printf("Client has received %d from socket.\n", value);
  }

  printf("Exiting now.\n");

  close(sock);
  exit(0);

}
