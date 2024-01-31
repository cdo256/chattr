#include "util.h"
#include "server.h"
#include "message.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int server_init(server_t *server) {
  memset(server, 0, sizeof(server_t));
  FD_ZERO(&server->fds);

  if (read_port("Enter a port number: ", &server->port) < 0) {
    goto error_cleanup;
  }

  printf("Making socket on port %d\n", server->port);
  server->socket = make_server_socket(server->port);
  if (server->socket < 0)
    goto error_cleanup;
  else
    printf("Socket bound and listening...\n");

  FD_SET(server->socket, &server->fds);
  server->max_fd = server->socket;
  return 0;

error_cleanup:
  return -1;
}

int server_handle_connect(server_t *server, SOCKET new_socket, struct in_addr addr) {
  int i;
  for (i = 0; i < MAX_CONNECTIONS; i++) {
    // socket == 0 means the connection slot is free.
    if (server->connections[i].socket == 0)
      break;
  }
  if (i == MAX_CONNECTIONS) {
    send_format(new_socket, "No more room on this server, sorry.");
    close(new_socket);
    return -1;
  }
  if (new_socket > server->max_fd)
    server->max_fd = new_socket;
  // We're accepting this connection. Add it to our list
  server->connections[i].socket = new_socket;
  server->connections[i].addr = addr;
  server->connections[i].user_id = NULL;
  server->connections[i].room = NULL;
  FD_SET(new_socket, &server->fds);
  return 0;
}

int server_process_message(server_t *server, SOCKET socket) {
  char *message = NULL;
  int recv_result = recv_message(socket, &message);
  if (recv_result < 0) {
    eprintf("Error %d\n, recv_result");
    return -1;
  } else if (recv_result == 0) {
    // Nothing to read
    return 0;
  }
  printf("Recieved '%s'\n", message);
  for (int i = 0; i < MAX_CONNECTIONS; i++) {
    send_string(server->connections[i].socket, message);
  }
  free(message);
  return 1;
}

void server_run() {
  static server_t server = {0};
  if (server_init(&server) < 0)
    goto abort;

  while (1) {
    fd_set read_fds = server.fds;
    int select_result = select(server.max_fd + 1, &read_fds, NULL, NULL, NULL);
    if (select_result < 0) {
      perror("Error select failed");
      goto abort;
    } else if (select_result == 0) {
      continue;
    }
    /* printf("Select returned %d\n", select_result); */
    /* printf("read_fds = %d\n", ((unsigned int*)&read_fds)[0]); */
    for (int i = 0; i <= server.max_fd; i++) {
      if (FD_ISSET(i, &read_fds)) {
        if (i == server.socket) {
          // New connection
          struct in_addr addr;
          SOCKET new_socket = accept_connection(server.socket, &addr);
          if (new_socket < 0)
            goto abort;
          if (server_handle_connect(&server, new_socket, addr) < 0) {
            goto abort;
          }
        } else {
          if (server_process_message(&server, i) < 0)
            goto abort;
        }
      }
    }
  }

  struct in_addr addr = {0};
  int connected_socket = accept_connection(server.socket, &addr);
  if (connected_socket < 0)
    goto abort;
  char addr_string[100];
  inet_to_string(addr, addr_string);
  printf("Accepting connection on %s (%x)\n", addr_string, addr.s_addr);
  char *buffer;
  int count = recv_message(connected_socket, &buffer);
  if (count < 0) {
    goto abort;
  } else if (count > 0) {
    printf("Recieved '%s'\n", buffer);
  } else {
    wprintf("Recieved empty message.\n");
  }

  return;

 abort:
  eprintf("Aborting server..\n");
}
