#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "server.h"

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
  buffer[getline_result] = '\0';
  sscanf(buffer, "%d", &server->port);

  printf("Making socket on port %d", server->port);
  server->socket = make_server_socket(server->server, server->port);
  if (server->socket < 0)
    goto error_cleanup;
  else
    printf("Socket bound successfully!\n");

  printf("Enter an username: ");
  getline_result = getline(&buffer, &buffer_length, stdin);
  if (getline_result < 0) {
    perror("Error occurred while reading line");
    goto error_cleanup;
  }
  buffer[getline_result] = '\0';
  server->username = strdup(buffer);

 error_cleanup:
  free(buffer);
  return -1;
}

void server_run() {
  static server_t server = {0};
  if (server_init(server_t *server) < 0) {
    printf("Aborting server..\n");
    return;
  }
  char *buffer;
  int count = recv_message(server.socket, &buffer);
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
