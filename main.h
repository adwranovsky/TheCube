#include <stddef.h>

// Sci functions
void sci_get_buf(char *buf, size_t length);
void sci_send_buf(const char *buf, size_t length);
char sci_get_char(void);
void sci_send_char(char c);
