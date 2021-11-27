#include <coap3/coap.h>

void hexdump(void *ptr, int buflen);
int resolve_address(const char *host, const char *service, coap_address_t *dst);