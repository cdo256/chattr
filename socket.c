#include <errno.h>
#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int make_client_socket(const char *ip_addr, int port) {
  int sfd, return_value;
  sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sfd < 0)
    if (sfd < 0) {
      perror("Client: Couldn't make socket");
      return -1;
    }

  struct sockaddr_in addr = {0};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(ip_addr);
  addr.sin_port = htons(port);
  int connect_result = connect(sfd, (struct sockaddr *)&addr, sizeof(addr));
  if (connect_result < 0 && errno != EINTR) {
    close(sfd);
    perror("Client: Unable to connect");
    return -1;
  }

  return sfd;
}

int make_server_socket(int port) {
  int sfd;
  sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sfd < 0) {
    perror("Server: Couldn't make socket");
    return -1;
  }
  struct sockaddr_in addr = {0};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);
  int bind_result = bind(sfd, (struct sockaddr *)&addr, sizeof(addr));
  if (bind_result < 0) {
    close(sfd);
    perror("Server: Couldn't bind to port");
    return -1;
  }
  // Hardcode backlog of 20.
  int listen_result = listen(sfd, 20);
  if (listen_result < 0) {
    close(sfd);
    perror("Server: Couldn't listen to port");
    return -1;
  }
}

int accept_connection(int sfd, struct in_addr *client_addr) {
  struct sockaddr_in addr = {0};
  int addrlen = sizeof(addr);
  int accept_return = accept(sfd, (struct sockaddr *)&addr, &addrlen);
  if (listen_result < 0) {
    close(sfd);
    perror("Server: Couldn't listen to port");
    return -1;
  }
  
}
