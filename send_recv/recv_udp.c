#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#define NAME "Mr. Burns"

void printsin(struct sockaddr_in *s__in, char *str1, char *str2) {
  char* ip_addr = inet_ntoa(s__in->sin_addr);
  u_long n = ntohs(s__in->sin_port);

  printf("%s\n", str1);
  printf("%s: " ,str2);
  printf("ip:%s ," , ip_addr);
  printf("port:%lu\n", n);
}


int main(int argc, char *argv[])
{
  // initialize the variables
  int socket_fd, cc, fsize;
  // initialize address of myself and peer
  struct sockaddr_in  s_in, from;

  //defining the msg
  char  msg[512];


  // assigning the socket file directory
  socket_fd = socket (AF_INET, SOCK_DGRAM, 0);

  // setting the memory in the s_in struct to '\0' for Thread safty
  bzero((char *) &s_in, sizeof(s_in));  /* They say you must do this    */

  // setting the Family to be IPv4
  s_in.sin_family = (short)AF_INET;
  // setting the IP address
  s_in.sin_addr.s_addr = htonl(INADDR_ANY);    /* WILDCARD */
  // setting the port to the network byte order version of a given u_short
  s_in.sin_port = htons((u_short)0x3333);

    // host to network short
    // converting to Big-Endian (if host is Little Endian)

  printsin( &s_in, "RECV_UDP", "Local socket is:");
  // flush the stdout stream (for Thread safty)
  fflush(stdout);

  // binding name to socket
  bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in));

  // infinite listening loop
  while(1) {
    // store length of from address
    fsize = sizeof(from);
    // read data (msg) of a certain length from socket file directory that
    // is at the address stored in "from" and its contents are the size stored
    // in "fsize"
    cc = recvfrom(socket_fd,msg,sizeof(msg),0,(struct sockaddr *)&from,&fsize);
    //print socket info
    printsin( &from, "recv_udp: ", "Packet from:");
    // display "<msg>"
    printf("Got data ::%s\n", msg);

    strcpy(msg, NAME);
    sendto(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&from,
                    sizeof(from));

    // clean the socket file directory
    fflush(stdout);
  }

  return 0;
}
