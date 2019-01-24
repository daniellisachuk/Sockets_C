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

int main(int argc, char* argv[]){
  int sock;
  unsigned short port;
  struct sockaddr_in srv_name, *res_name;
  struct addrinfo *res;
  char URL[100], host_URL[50], path[50];

  char message[512], server_reply[10000];

  if(argc != 2){
    printf("[!]Usage: %s <URL>", argv[0]);
    exit(EXIT_FAILURE);
  }

  strcpy(URL, argv[1]);

  if(sscanf(URL, "http://%49[^:]:%hd/%49[^\n]", host_URL, &port, path) != 3){
    if(sscanf(URL, "http://%49[^/]/%49[^\n]", host_URL, path) != 2){
      strcpy(path, "index.html");
      if(sscanf(URL, "http://%49[^:]:%hd/", host_URL, &port) != 2){
        port = 80;
        if(sscanf(URL, "http://%49[^/]/", host_URL) != 1){
          perror("[-]Error in parsing URL");
        }
      }
    }
    else{ //if host/path
      port = 80;
    }
  }


  if (0 != getaddrinfo(host_URL, NULL, NULL, &res)) {
    fprintf(stderr, "[!]Error in resolving URL %s\n", URL);
    exit(1);
  }

  printf("[+]Client is alive and establishing socket connection.\n");


  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0){
      perror ("[!]Error opening channel");
      close(sock);
      exit(1);
  }


  bzero(&srv_name, sizeof(srv_name));
  res_name = (struct sockaddr_in *)res->ai_addr;
  srv_name.sin_port = htons(port);
  srv_name.sin_family = AF_INET;
  srv_name.sin_addr = res_name->sin_addr;


  if (connect(sock, (struct sockaddr *)&srv_name, sizeof(srv_name)) < 0){
      perror ("[!]Error establishing communications");
      close(sock);
      exit(1);
  }

  puts("[+]Connected\n");

  sprintf(message, "GET %s HTTP/1.0\nHOST: %s\n\n", path, host_URL);
  printf("%s\n", message);

  if(send(sock , message , strlen(message) , 0) < 0){
    puts("[-]Send failed");
    return 1;
  }

  puts("[+]Data Send\n");

  while(1){

    int received_len = recv(sock, server_reply , sizeof server_reply , 0);

    if(received_len < 0){
        puts("[+]Reciev failed");
        break;
    }
    else if(received_len == 0){
      write(1, "[+]Recive Complete", 1);
      break;
    }

    //puts(server_reply);
    write(1, server_reply , received_len);

  }

  puts("\n[+]Reply received\n");



  printf("[+]Exiting now.\n");

  close(sock);
  exit(0);

}
