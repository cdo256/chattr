#include "util.h"
#include "server.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int server_init(server_t *server) {
  memset(server, 0, sizeof(server_t));

  if (read_port("Enter a port number: ", &server->port) < 0) {
    goto error_cleanup;
  }

  printf("Making socket on port %d\n", server->port);
  server->socket = make_server_socket(server->port);
  if (server->socket < 0)
    goto error_cleanup;
  else
    printf("Socket bound and listening...\n");

  return 0;

error_cleanup:
  return -1;
}

void server_run() {
  static server_t server = {0};
  if (server_init(&server) < 0)
    goto abort;
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
