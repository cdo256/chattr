#include "socket.h"

int send_format(SOCKET socket, ...);
int await_message(int sfd, char **buffer);
