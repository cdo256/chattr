#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "socket.h"
#include "client.h"
#include "server.h"

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

int main() {
  int mode = query_choice("Client or server?", "cs");

  if (mode == 'c') {
    client_run();
  } else if (mode == 's') {
    server_run();
  } else {
    printf("Invalid input.\n");
  }
}

