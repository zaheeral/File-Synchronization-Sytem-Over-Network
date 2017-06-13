#include <stdio.h>
#include "ftree.h"

#ifndef PORT
  #define PORT 57431
#endif

int main(int argc, char ** argv){

	if(argc < 3){
		printf("Format: ./rcopy_client client_source_path server_dest_path ip_address\n");
		return 1;
	}

	return fcopy_client(argv[1], argv[2], argv[3], PORT);
}