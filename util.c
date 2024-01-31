#define __STDC_WANT_LIB_EXT2__ 1 // for strdup
#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <string.h>
#include <threads.h>

void little_nap() {
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 100 * 1000000;  // 100 milliseconds

    thrd_sleep(&ts, NULL);
}

void eprintf(const char *string, ...) {
  va_list ap;
  va_start(ap, string);
  fprintf(stderr, "Error: ");
  vfprintf(stderr, string, ap);
  va_end(ap);
}

void wprintf(const char *string, ...) {
  va_list ap;
  va_start(ap, string);
  fprintf(stderr, "Warning: ");
  vfprintf(stderr, string, ap);
  va_end(ap);
}

void *resize(void *buffer, size_t new_length) {
  void *result = realloc(buffer, new_length);
  if (result == NULL) {
    free(buffer);
    eprintf("Unable to allocate %d bytes", new_length);
    return NULL;
  } else {
    return result;
  }
}

int validate_alphanum(char *string) {
  for (int i = 0; string[i]; i++) {
    if (!isalnum(string[i])) {
      eprintf("String '%s' contains non-alphanumeric character '%c'\n", string, string[i]);
      return -1;
    }
  }
  return 0;
}


int read_port(char *prompt, short *value) {
  char *buffer = NULL;
  size_t buffer_length = 0;
  do {
    printf("%s", prompt);
    int getline_result = getline(&buffer, &buffer_length, stdin);
    if (getline_result < 0) {
      perror("Error occurred while reading line");
      continue;
    }
    if (getline_result != 1) {
      buffer[getline_result-1] = '\0';
      int result = sscanf(buffer, "%hd", value);
      if (result <= 0) {
        eprintf("Input is not a valid port number.");
        continue;
      }
    } else {
      // default value
      *value = 10000;
    }
  } while (0);
  free(buffer);
  return 0;
}

int read_ip_addr(char *prompt, char **addr_string) {
  char *buffer = NULL;
  size_t buffer_length = 0;
  do {
    printf("%s", prompt);
    size_t getline_result = getline(&buffer, &buffer_length, stdin);
    if (getline_result <= 0) {
      perror("Error occurred while reading line");
      continue;
    }
    if (getline_result != 1) {
      buffer[getline_result-1] = '\0';
      if (inet_addr(buffer) == INADDR_NONE) {
        eprintf("Invalid IP address\n");
        continue;
      }
      *addr_string = strdup(buffer);
    } else {
      // default value
      *addr_string = strdup("127.0.0.1");
    }
  } while (0);
  free(buffer);
  return 0;
}

int read_alphanum(char *prompt, char **input) {
  char *buffer = NULL;
  size_t buffer_length = 0;
  do {
    printf("%s", prompt);
    int getline_result = getline(&buffer, &buffer_length, stdin);
    if (getline_result < 0) {
      perror("Error occurred while reading line");
      continue;
    }
    buffer[getline_result-1] = '\0';
    if (validate_alphanum(buffer) < 0) {
      // Error already printed by validate_alphanum.
      continue;
    }
    *input = strdup(buffer);
  } while (0);
  free(buffer);
  return 0;
}

