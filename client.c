#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "client.h"

int client_init(client_t *client) {
  memset(client, 0, sizeof(client_t));
  char *buffer = NULL;
  size_t buffer_length = 0;

  printf("Enter an IP address: ");
  int getline_result = getline(&buffer, &buffer_length, stdin);
  if (getline_result < 0) {
    perror("Error occurred while reading line");
    return NULL;
  }
  buffer[getline_result] = '\0';
  client.server = strdup(buffer);

  printf("Enter a port: ");
  getline_result = getline(&buffer, &buffer_length, stdin);
  if (getline_result < 0) {
    perror("Error occurred while reading line");
    return NULL;
  }
  buffer[getline_result] = '\0';
  sscanf(buffer, "%d", client.port);
  client.server = strdup(buffer);


  printf("Connecting to ")
  client.socket = make_client_socket();
  if (client.socket < 0) return NULL;

 error_cleanup:
  free(buffer);
  return NULL;
}
