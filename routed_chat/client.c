#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#define NAME "Hound"

//function prints connection info
void printsin(struct sockaddr_in *s__in, char *str1, char *str2) {
  char* ip_addr = inet_ntoa(s__in->sin_addr);
  u_long port = ntohs(s__in->sin_port);

  printf("%s\n", str1);
  printf("%s: " ,str2);
  printf("ip:%s ," , ip_addr);
  printf("port:%lu\n", port);
}



int main(int argc, char const *argv[]) {

  if(argc != 2) {
    printf("Usage: %s <HOST>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int socket_fd, select_ret, n_read;
  fd_set read_fds;
  socklen_t peer_address_size;
  struct sockaddr_in sock_myself, sock_peer;
  struct hostent *hostptr;
  char message_out [511];
  struct{
    char type;
    char message[511];
  }buffer;


  socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

  hostptr = gethostbyname(argv[1]);

  sock_myself.sin_addr.s_addr = htonl(INADDR_ANY);
  sock_myself.sin_family = AF_INET;
  sock_myself.sin_port = htons(0x1111);

  bcopy(hostptr->h_addr, (char *)&sock_peer.sin_addr,hostptr->h_length);
  sock_peer.sin_family = AF_INET;
  sock_peer.sin_port = htons(0x2222);

  fflush(stdout);

  bind(socket_fd, (struct sockaddr *) &sock_myself, sizeof(sock_myself));




  while(1) {

    FD_ZERO(&read_fds);
    //add stdin to fd_set
    FD_SET(0, &read_fds);
    //add socket to fd_set
    FD_SET(socket_fd, &read_fds);

    select_ret = select(socket_fd + 1, &read_fds, NULL, NULL, NULL);

    //if problem with select
    if(select_ret == -1){
      perror("SELECT");
      exit(EXIT_FAILURE);
    }
    else{
      //if user input
      if(FD_ISSET(0, &read_fds)){
        bzero(&message_out, sizeof(message_out));
        n_read = read(0, message_out, 511);
        if(n_read <=0) break;                    /* if unsuccessful */
        else{                                    /* if successful */
          //if typed "exit"
          if(strcasecmp(message_out, "exit\n") == 0){
            printf("Exiting...\n");
            exit(EXIT_SUCCESS);
          }
          else{ //if typed message
          buffer.type = 'A';

          strcpy(buffer.message, message_out);
          sendto(socket_fd, &buffer, sizeof(buffer), 0, (struct sockaddr *) &sock_peer, sizeof(sock_peer));

          fflush(stdin);
          }
        }
      }
      //if new message
      else if(FD_ISSET(socket_fd, &read_fds)){
        peer_address_size = sizeof(sock_peer);
        recvfrom(socket_fd, &buffer, sizeof(buffer), 0, (struct sockaddr *) &sock_peer,
                                                &peer_address_size);
        printsin( &sock_peer, "recv_udp: ", "Packet from:");
        printf("Got data : %s\n", buffer.message);

        fflush(stdout);

      }
    }
    bzero((char *) &buffer, sizeof(buffer));
    fflush(stdin);
    fflush(stdout);
  }
  return 0;
}
