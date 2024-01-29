#include <netinet/in.h>

typedef int SOCKET;

int make_client_socket(const char *ip_addr, int port);
int make_server_socket(int port);
int accept_connection(int sfd, struct in_addr *client_addr);
int send_string(int sfd, char *string);
int recv_message(int sfd, char **buffer);
