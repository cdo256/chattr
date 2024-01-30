#include "socket.h"

// Maximum total connections, users, rooms that the server can track.
#define MAX_CONNECTIONS 20
#define MAX_USERS 100
#define MAX_ROOMS 10
#define MAX_USERS_PER_ROOM 10

typedef struct connection_t connection_t;
typedef struct user_t user_t;
typedef struct room_t room_t;
typedef struct server_t server_t;

struct connection_t {
  SOCKET socket;
  struct in_addr addr;
  user_t *user_id;
  room_t *room;
};

struct user_t {
  char *username;
  char *password;
  connection_t *connection;
};

struct room_t {
  char *name;
  connection_t *connected[MAX_USERS_PER_ROOM];
};

struct server_t {
  SOCKET socket;
  short port;
  fd_set fds;
  SOCKET max_fd;
  connection_t connections[MAX_CONNECTIONS];
  user_t users[MAX_USERS];
  room_t rooms[MAX_ROOMS];
};

void server_run();
