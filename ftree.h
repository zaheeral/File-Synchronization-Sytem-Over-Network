#include <stdlib.h>
#include "hash.h"


#ifndef _FTREE_H_
#define _FTREE_H_

#define MAXPATH 255
#define MISMATCH 1
#define MATCH 2
#define MATCH_ERROR 3
#define TRANSMIT_OK 4
#define TRANSMIT_ERROR 5

/* Struct for storing file information.
 */
struct fileinfo {
    char path[MAXPATH];
    mode_t mode;
    char hash[HASH_SIZE];
    size_t size;
};

/* Client fcopy function. 
 * Returns 0 on a successful copy and 1 otherwise.
 */
int fcopy_client(char *src_path, char *dest_path, char *host, int port);

/* Server fcopy function.
 * Should never return, as it keeps waiting for new connections.
 */ 
void fcopy_server(int port);

#endif // _FTREE_H_
