#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define NAME "Smithers"





int main(int argc, char const *argv[]) {

  if(argc != 2 ){
    printf("Usage: %s <X in range [0,1]>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  double input_x = atof(argv[1]);

  if(0 > input_x || input_x > 1){
     printf("X must be between 0 and 1\n");
     exit(EXIT_FAILURE);
   }

  double random;
  int socket_fd;
  struct{
    char type;
    char message[511];
  }buffer;
  socklen_t peer_address_size;
  struct sockaddr_in sock_myself, sock_peer, sock_client, sock_server;

  socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

  bzero((char *) &sock_myself, sizeof(sock_myself));

  sock_myself.sin_family = AF_INET;
  sock_myself.sin_port = htons(0x2222);
  sock_myself.sin_addr.s_addr = htonl(INADDR_ANY);

  sock_client.sin_family = AF_INET;
  sock_client.sin_port = htons(0x1111);
  sock_client.sin_addr.s_addr = htonl(INADDR_ANY);

  sock_server.sin_family = AF_INET;
  sock_server.sin_port = htons(0x3333);
  sock_server.sin_addr.s_addr = htonl(INADDR_ANY);


  fflush(stdout);

  bind(socket_fd, (struct sockaddr *) &sock_myself, sizeof(sock_myself));

  while(1){

    peer_address_size = sizeof(sock_peer);
    recvfrom(socket_fd, &buffer, sizeof(buffer), 0,(struct sockaddr *) &sock_peer, &peer_address_size);

    // printsin( &sock_peer, "recv_udp: ", "Packet from:");
    // printf("Got data : %s\n", buffer.message);

    //if came from server
    if(buffer.type == 'C'){
      buffer.type = 'B';

      sendto(socket_fd, &buffer, sizeof(buffer), 0, (struct sockaddr *) &sock_client, sizeof(sock_client));

    }
    //if came from client
    else if(buffer.type == 'A'){
      //if generated random is greater than input X
      if((random = (rand() % 10000) / 10000.0) > input_x){
        buffer.type = 'B';

        sendto(socket_fd, &buffer, sizeof(buffer), 0, (struct sockaddr *) &sock_server, sizeof(sock_server));
      }
    }
    bzero((char *) &buffer, sizeof(buffer));
    fflush(stdout);

  }
  // TODO
  /*
  the task said "message freely", find out how can sendto and recvfrom
  run at the same time (multithreading?)

  ANS:
  no multithreading. your new TODO is to carefully copy-paste the select
  code you've written in client.c

  quick note:
  the code currently written here might be useful for router. so at least
  comment it and save it to the router.c before getting rid of it completely
  */

  return 0;
}
