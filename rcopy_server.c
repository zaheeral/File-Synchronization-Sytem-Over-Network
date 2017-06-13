#include <stdio.h>
#include "ftree.h"

#ifndef PORT
  #define PORT 57431
#endif

int main(int argc, char ** argv){
	fcopy_server(PORT);
}