#include "client.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int client_init(client_t *client) {
  memset(client, 0, sizeof(client_t));
  char *buffer = NULL;
  size_t buffer_length = 0;

  printf("Enter an IP address: ");
  int getline_result = getline(&buffer, &buffer_length, stdin);
  if (getline_result < 0) {
    perror("Error occurred while reading line");
    goto error_cleanup;
  }
  buffer[getline_result-1] = '\0';
  client->server = strdup(buffer);

  printf("Enter a port: ");
  getline_result = getline(&buffer, &buffer_length, stdin);
  if (getline_result < 0) {
    perror("Error occurred while reading line");
    goto error_cleanup;
  }
  buffer[getline_result-1] = '\0';
  sscanf(buffer, "%hd", &client->port);

  printf("Connecting to %s on port %d", client->server, client->port);
  client->socket = make_client_socket(client->server, client->port);
  if (client->socket < 0)
    goto error_cleanup;
  else
    printf("Successfully connected!\n");

  printf("Enter an username: ");
  getline_result = getline(&buffer, &buffer_length, stdin);
  if (getline_result < 0) {
    perror("Error occurred while reading line");
    goto error_cleanup;
  }
  buffer[getline_result-1] = '\0';
  client->username = strdup(buffer);

error_cleanup:
  free(buffer);
  return -1;
}

void client_run() {
  static client_t client = {0};
  if (client_init(&client) < 0) {
    printf("Aborting client..\n");
    return;
  }
  send_string(client.socket, "Hello!");
}
