void eprintf(const char *string, ...);
void wprintf(const char *string, ...);
int validate_alphanumeric(char *string);
int read_port(char *prompt, short *value);
int read_ip_addr(char *prompt, char **addr_string);
int read_alphanum(char *prompt, char **input);
