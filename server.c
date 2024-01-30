#include "server.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int server_init(server_t *server) {
  memset(server, 0, sizeof(server_t));
  char *buffer = NULL;
  size_t buffer_length = 0;

  printf("Enter a port: ");
  int getline_result = getline(&buffer, &buffer_length, stdin);
  if (getline_result < 0) {
    perror("Error occurred while reading line");
    goto error_cleanup;
  }
  buffer[getline_result-1] = '\0';
  sscanf(buffer, "%hd", &server->port);

  printf("Making socket on port %d\n", server->port);
  server->socket = make_server_socket(server->port);
  if (server->socket < 0)
    goto error_cleanup;
  else
    printf("Socket bound successfully!\n");

  return 0;

error_cleanup:
  free(buffer);
  return -1;
}

void server_run() {
  static server_t server = {0};
  if (server_init(&server) < 0) {
    printf("Aborting server..\n");
    return;
  }
  struct in_addr addr = {0};
  int connected_socket = accept_connection(server.socket, &addr);
  if (connected_socket < 0) {
    printf("Aborting server..\n");
  }
  char addr_string[100];
  inet_to_string(addr, addr_string);
  printf("Accepting connection on %s (%x)\n", addr_string, addr.s_addr);
  char *buffer;
  int count = recv_message(connected_socket, &buffer);
  if (count < 0) {
    printf("Aborting server..\n");
    return;
  } else if (count > 0) {
    printf("Recieved '%s'\n", buffer);
  } else {
    printf("Recieved empty message..\n");
    return;
  }
}
