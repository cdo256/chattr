#include "socket.h"

typedef struct server_t {
  SOCKET socket;
  short port;
  connection_t *connection_list;
} client_t;
