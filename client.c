#include "util.h"
#include "client.h"
#include "message.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int client_init(client_t *client) {
  memset(client, 0, sizeof(client_t));

  if (read_ip_addr("Enter an IP address: ", &client->server) < 0) {
    goto error_cleanup;
  }

  if (read_port("Enter a port: ", &client->port) < 0) {
    goto error_cleanup;
  }

  printf("Connecting to %s on port %d...\n", client->server, client->port);
  client->socket = make_client_socket(client->server, client->port);
  if (client->socket < 0)
    goto error_cleanup;
  else
    printf("Successfully connected!\n");

  if (read_alphanum("Enter a username: ", &client->username) < 0)
    goto error_cleanup;

  if (send_format(client->socket, "/identify %s", client->username) < 0) {
    eprintf("Unable to send /identify\n");
    goto error_cleanup;
  }

  return 0;

error_cleanup:
  return -1;
}

int client_process_messages(client_t *client) {
  int response = 0;
  char *message = NULL;
  int count = 0;
  do {
    response = recv_message(client->socket, &message);
    if (response < 0) {
      eprintf("Could not get message from server\n");
      break;
    } else if (response > 0) {
      printf("%s\n", message);
      count++;
    }
  } while (response != 0);
  free(message);
  if (response < 0) {
    return response;
  } else {
    return count;
  }
}

void client_run() {
  static client_t client = {0};
  if (client_init(&client) < 0) {
    eprintf("Aborting client..\n");
    return;
  }
  char *buffer = NULL;
  size_t buffer_length = 0;
  while (1) {
    little_nap();
    client_process_messages(&client);
    printf("\r> ");
    int getline_result = getline(&buffer, &buffer_length, stdin);
    if (getline_result < 0) {
      perror("Error occurred while reading line");
      continue;
    } else if (getline_result <= 1) {
      // Empty message. Just pull new messages again.
      continue;
    }
    buffer[getline_result-1] = '\0';
    printf("Sending '%s'\n", buffer);
    if (send_format(client.socket, "*%s", buffer) < 0) {
      eprintf("Error sending message\n");
      continue;
    }
  }
}
