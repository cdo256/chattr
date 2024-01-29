#include "socket.h"

typedef struct client_t {
  SOCKET socket;
  char *server;
  short port;
  char *username;
} client_t;

void client_run();