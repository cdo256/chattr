#include "socket.h"

typedef struct connection_t {
  // TODO
} connection_t;

typedef struct server_t {
  SOCKET socket;
  short port;
  connection_t *connection_list;
} server_t;

void server_run();