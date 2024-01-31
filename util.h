#include <stddef.h>

void little_nap();
void eprintf(const char *string, ...);
void wprintf(const char *string, ...);
void *resize(void *buffer, size_t new_length);
int validate_alphanumeric(char *string);
int read_port(char *prompt, short *value);
int read_ip_addr(char *prompt, char **addr_string);
int read_alphanum(char *prompt, char **input);
