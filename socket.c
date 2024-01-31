#include "util.h"
#include "socket.h"
#include <arpa/inet.h>
#include <errno.h>
#include <net/if.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int set_socket_nonblocking(SOCKET sfd) {
  printf("Setting socket %d as non-blocking\n", sfd);
  int flags = fcntl(sfd, F_GETFL, 0);
  if (flags < 0) {
    close(sfd);
    perror("Error: Unable to get socket properties");
    return -1;
  }
  fcntl(sfd, F_SETFL, flags | O_NONBLOCK); 
  if (flags < 0) {
    close(sfd);
    perror("Error: Unable to set socket as non-blocking");
    return -1;
  }

  return 0;
}

int make_client_socket(const char *ip_addr, int port) {
  int sfd;
  sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sfd < 0)
    if (sfd < 0) {
      perror("Error: Couldn't make socket");
      return -1;
    }

  struct sockaddr_in addr = {0};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(ip_addr);
  addr.sin_port = htons(port);
  int connect_result = connect(sfd, (struct sockaddr *)&addr, sizeof(addr));
  if (connect_result < 0 && errno != EINTR) {
    close(sfd);
    perror("Error: Unable to connect");
    return -1;
  }
  if (set_socket_nonblocking(sfd) < 0) {
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
  if (set_socket_nonblocking(sfd) < 0) {
    return -1;
  }

  return sfd;
}

int accept_connection(int sfd, struct in_addr *client_addr) {
  struct sockaddr_in addr = {0};
  socklen_t addrlen = sizeof(addr);
  int nsfd = accept(sfd, (struct sockaddr *)&addr, &addrlen);
  if (nsfd < 0) {
    close(sfd);
    perror("Server: Couldn't listen to port");
    return -1;
  }
  memcpy(client_addr, &addr.sin_addr, sizeof(struct in_addr));
  if (set_socket_nonblocking(nsfd) < 0) {
    return -1;
  }

  return nsfd;
}

int send_string(int sfd, char *string) {
  unsigned short length = strlen(string);
  char *buffer = malloc(2 + length);
  if (buffer == NULL) {
    fprintf(stderr, "Unable to allocate buffer to send string. Aborting..\n");
    close(sfd);
    return -1;
  }
  // Store the length first
  *(unsigned short *)buffer = htons(length);
  // Then the string
  memcpy(buffer + 2, string, length);
  int send_result = send(sfd, buffer, length + 2, 0);
  if (send_result < 0) {
    perror("Unable to send message");
    free(buffer);
    close(sfd);
    return -1;
  }
  free(buffer);
  return 0;
}

int recv_message(int sfd, char **buffer) {
  unsigned short length;
  int recv_result;
  recv_result = recv(sfd, &length, 2, MSG_WAITALL);

  if (recv_result < 0) {
    if (errno == EAGAIN) {
      // Not a real error code. Ignore
      return 0;
    }
    perror("Error: Unable to recv message length");
    close(sfd);
    return -1;
  } else if (recv_result == 0) {
    return 0;
  } else if (recv_result < 2) {
    eprintf("Incomplete packet. Only %d/%d bytes recieved\n", recv_result, 2);
    close(sfd);
    return -1;
  }
  length = ntohs(length);
  *buffer = resize(*buffer, length + 1);
  recv_result = recv(sfd, *buffer, length, MSG_WAITALL);
  if (recv_result < 0) {
    perror("Unable to recv message body");
    close(sfd);
    return -1;
  } else if (recv_result < length) {
    eprintf("Incomplete packet. Only %d/%d bytes recieved\n", recv_result + 2, length + 2);
    close(sfd);
    return -1;
  }
  (*buffer)[length] = '\0';
  return length;
}

void inet_to_string(struct in_addr addr, char *buffer) {
  inet_ntop(AF_INET, &addr, buffer, INET_ADDRSTRLEN);
}
