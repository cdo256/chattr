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

int main() {
  int mode = query_choice("Client or server?", "cs");

  if (mode == 'c') {
    client_init();
  } else if (mode == 's') {
    
  } else {
    printf("Invalid input.\n");
  }
}

