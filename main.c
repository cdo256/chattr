#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "socket.h"

int query_choice(char *string, char *options) {
  do {
    printf("%s [%s]: ", string, options);
    int answer = getc();
    for (int i = 0; options[i]; i++) {
      if (tolower(options[i]) == tolower(answer)) {
        return options[i];
      }
    }
    printf("Invalid option %c\n", answer);
  } while (1);
}

static server_t server = {0};
static client_t client = {0};

int main() {
  int mode = query_choice("Client or server?", "cs");

  if (mode == 'c') {
    if (client_init(&server) == 0) {
      send_string("Hello!");
    }
  } else if (mode == 's') {
    if (server_init(&server) == 0) {
      char *buffer;
      int count = recv_message(&buffer);
      if (count > 0) {
        printf("recieved '%s'\n", buffer);
      }
    }
  } else {
    printf("Invalid input.\n");
  }
}

