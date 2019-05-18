#ifndef FNET_H_
#define FNET_H_

#include <stddef.h>

#define FNET_OK 0
#define FNET_ERR -1
#define FNET_ERR_LEN 256

/**
 * Resolves hostname to IP address.
 *
 * @return int -1 on error, 0 on success.
 */
int fnet_resolve(char *err, char *hostname, char *ipbuf, size_t ipbuf_len);

#endif  /* FNET_H_ */
