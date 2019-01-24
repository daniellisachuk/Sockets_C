#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
char* NAME = "Hound";

int main(int argc, char *argv[])
{
  //  socket file directory
  int socket_fd;
  // initialize address of destination
  struct sockaddr_in  dest;
  // initialize reader from arguments to read address given in command line
  struct hostent *hostptr;
  socklen_t dest_size;

  // defining the msg
    char msgbuf[512];

  // assigning the socket file directory
  socket_fd = socket (AF_INET, SOCK_DGRAM, 0);

  // setting the memory in the s_in struct to '\0' (for Thread safty)
  bzero((char *) &dest, sizeof(dest)); /* They say you must do this */
  // get the hostname from command line arguments
  hostptr = gethostbyname(argv[1]);
  // setting the Family to be IPv4
  dest.sin_family = (short) AF_INET;
  // copying host address from struct that extracted arguments
  // into the ADDRESS field of dest socket adress thusby setting the
  // peer IP
  bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length);

  // host to network short\long
  // converting to Big-Endian (if host is Little Endian)

  // setting the port to the network byte order version of a given u_short
  dest.sin_port = htons((u_short)0x3333);
  dest_size = sizeof(dest);
  strcpy(msgbuf, NAME);
  // send the msg to destination
  sendto(socket_fd,&msgbuf,sizeof(msgbuf),0,(struct sockaddr *)&dest,
                  sizeof(dest));
  recvfrom(socket_fd,msgbuf,sizeof(msgbuf),0,(struct sockaddr *)&dest,&dest_size);
  printf("%s\n",msgbuf);
  return 0;
}
