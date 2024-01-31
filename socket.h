#include <netinet/in.h>

typedef int SOCKET;

SOCKET make_client_socket(const char *ip_addr, int port);
SOCKET make_server_socket(int port);
SOCKET accept_connection(SOCKET sock, struct in_addr *client_addr);
int send_string(SOCKET sock, char *string);
int recv_message(SOCKET sock, char **buffer);
void inet_to_string(struct in_addr addr, char *buffer);
