#include "socket.h"

typedef struct connection_t {
  SOCKET socket;
  struct in_addr addr;
  struct connection_t *next;
} connection_t;

typedef struct server_t {
  SOCKET socket;
  short port;
  connection_t *connection_list;
} server_t;

void server_run();
