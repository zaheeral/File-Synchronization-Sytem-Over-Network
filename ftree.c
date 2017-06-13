#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "ftree.h"


//Here we would like to open socket and connect to our server. In the beginning, we have
//implemented some error catch and throw calls. Then we are setting up the absolute pathway
//for the server side. As server itself does not know what location is supposed to be a
//'destination' location, thus we have to set it and implement and according to our recursive
//implementation so that we can go into the directory tree nodes. 

/*@StackOverFlow had some exaples of Socket error checking*/

int fcopy_client(char *source_pth, char *dest_path, char *host, int port){
	int client_soc;
	struct sockaddr_in peer;

	if ((client_soc = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		return -1;
	}

	peer.sin_family = AF_INET;
	peer.sin_port = htons(port);

	if (inet_pton(AF_INET, host, &peer.sin_addr) < 1) {
		close(client_soc);
		return -1;
	}

	if (connect(client_soc, (struct sockaddr *)&peer, sizeof(peer)) == -1) {
		return -1;
	}

	if(client_soc < 0){
		perror("Error opening client socket\n");
		return 1;
	}



	struct stat file;
	if(lstat(source_pth, &file) == -1){
		return NULL;
	}

	// If file the file is indeed a directory, we set the path as the root node. All nodes 
	//can be root nodes, with their higherarchy of respective nodes. The parent nodes supercede 
	//the child nodes interms of execution and tree higherarchy.
	if(S_ISDIR(file.st_mode)){
int i;

		char *root_path;
		int root_mode;
		char root_misormatch[255];
		int res;

		char *str5 = dest_path;
		char *str6 = source_pth;


		char * root_new_new_path = malloc(strlen(str5) + strlen(str6) + 20);
		strcpy(root_new_new_path, str5);
		strcat(root_new_new_path, "/");
		strcat(root_new_new_path, str6);
		if (root_new_new_path == NULL) {
			perror(root_new_new_path);
		}
		root_path = root_new_new_path;
		res = write(client_soc, root_path, strlen(root_path));

		sleep(0);

		root_mode = htonl(file.st_mode);
		res = write(client_soc, &root_mode, sizeof(root_mode));

		read(client_soc, root_misormatch, sizeof(root_misormatch));

		setup_for_client(source_pth, dest_path, client_soc);
	}
}




void setup_for_client(char *source_pth, char *dest_path, int client_soc){

	//Here I am initializing the variables that will be used in managing the fileinfo struct, 
	//directory, file mode, size, path and FILE Object 
	char misormatch[9];
	int int_misormatch;
	int converted_int_feed;
	FILE *source_of_file;
	struct stat lstat_of_sorc_reg;
	char *sour_dir;
	struct fileinfo *file_info_struct = malloc(sizeof(struct fileinfo));
	int ntohl_value_mode;
	char *dest_added_path;
	FILE *child_nodes;
	struct stat file_4_size;
	size_t size_to_network;
	FILE *reg_file;
	char chardat[1];
	DIR *dir_non_reg;
	dir_non_reg = opendir(source_pth);
	struct dirent *dir_non_reg_obj;

	//At this point we would like to loop through all the directory contents in the root
	//folder.
	
	while ((dir_non_reg_obj = readdir(dir_non_reg)) != NULL){
		// The loop does not want to go into hidden directories or directories with '.'
		//as indicated in the assignment.
		if(dir_non_reg_obj->d_name[0] != '.'){
			
			char *path_1 = source_pth;
			char *path_end = dir_non_reg_obj->d_name;
			char * new_path = malloc(strlen(path_1) + strlen(path_end) + 20);
			strcpy(new_path, path_1);
			strcat(new_path, "/");
			strcat(new_path, path_end);
			if (new_path == NULL) {
				perror(new_path);
			}
			sour_dir = new_path;


			//I was getting segmentation faults at this point, so I needed to bzero the
			//paths inorder to free up memory space (I pressume). As the loop was continously
			//feeding into the variables.
			bzero(file_info_struct->path, sizeof(file_info_struct->path));
			file_info_struct->mode = NULL;
			bzero(file_info_struct->path, sizeof(file_info_struct->path));
			file_info_struct->size = NULL;
			lstat(sour_dir, &lstat_of_sorc_reg);

			//We will now be concatinating the dest absolute path with the, absolute path
			//for the src subsection to be copied into src.
			char *str3 = dest_path;
			char *str4 = sour_dir;
			char * new_dest_added_path = malloc(strlen(str3) + strlen(str4) + 20);
			strcpy(new_dest_added_path, str3);
			strcat(new_dest_added_path, "/");
			strcat(new_dest_added_path, str4);
			if (new_dest_added_path == NULL) {
				perror(new_dest_added_path);
			}
			dest_added_path = new_dest_added_path;
			//
			strcpy(file_info_struct->path, dest_added_path);
			file_info_struct->mode = lstat_of_sorc_reg.st_mode;

		
			write(client_soc, file_info_struct->path, sizeof(file_info_struct->path));

			ntohl_value_mode = htonl((file_info_struct->mode));
			write(client_soc, &ntohl_value_mode, sizeof(ntohl_value_mode));






























			//Now we are reading the updates sent by the server. If Server gives us a MATCH,
			//then the function should simply continue. 

			read(client_soc, &int_misormatch, sizeof(int_misormatch));



			//As the misormatch interms of MATCH and MISMATCH is an integer that needs to be
			//converted when it is being sent over a socket, I have created a new variable
			//inorder to store the ntohl value into it.
			converted_int_feed = ntohl(int_misormatch);



			//If there is a match, then the program does not need to do anything else.
			if (converted_int_feed == MATCH)
			{
				printf("dir_non_reg Matched Correctly\n");

				//If the program is a MISMATCh, then it most likely is a Regular File.
				//As only Regular File can return a MISMATCH, because if ever a directory
				//is non-existant, the program will create it on the server side and 
				//return MATCH. Thus, directories always return MATCH. Whereas, Regular
				//Files MAY return a MISMATCH.
			} else if (converted_int_feed == MISMATCH)
			{
				printf("Needs the file size\n");
				printf("File Path is: %s\n", sour_dir);
				printf("The size of the File is: %zd\n",lstat_of_sorc_reg.st_size);

				//I am now turning the size integer representation of the Regular File 
				//in Source folder to a 'host to network' value,inorder for it to be ready
				//to be copied over to the server.
				size_to_network = htonl(lstat_of_sorc_reg.st_size);

				//We are not writing into the socket to send to server side, the size
				//of the Regular File in source folder. By sending the size, we can use it
				//to compare with the size of the Regular File on server to see if they
				//fulfil one of the requirements of 'sameness', the other requirement having
				//same Hash.
				write(client_soc, &size_to_network, sizeof(size_to_network));
				printf("Reading Size misormatch now \n");

				//The server returns data, this info will either tell us that there was a 
				//MATCH or MISMATCH.
				read(client_soc, &int_misormatch, sizeof(int_misormatch));


				//In the case that there was a MISMATCH, we would like to open the 
				//source REG file and read it byte by byte, in a loop while the file 
				//is not at the end of it or at 0. Each byte is sent to the socket using
				//write function. At the server end in a for loop from 0 to size(source file)
				//we would then take from socket and write into Server Regular File.

				//Unfortunately even though the implementation of the code seems correct, I
				//had to block it out because it kept giving me a Segmentation Fault Error. 
				if ((ntohl(int_misormatch)) == MISMATCH)
				{
					printf("\n");
					printf("\n");
					printf("\n");
					printf("Fflush File Steam:  %s\n");
					printf("\n");
					printf("\n");


					/*
					source_of_file = fopen(source_pth, "rb");
					while(fread(chardat, 1, 1, source_of_file) != 0){
						write(client_soc, &chardat, strlen(chardat));
					}*/


				}

			}



























			






			printf("				misormatch: %s\n", misormatch);

			//Inorder to recurse into a sub-directory we need to call the function again
			//with a new path. This has to be at the end of the function due to the fact,
			//that by doing this call we can redo all the steps we had gone through before.
			if(S_ISDIR(file_info_struct->mode)){
				setup_for_client(sour_dir, dest_path, client_soc);
			}
		}
	}
}




//Here the goal is to setup a server side socket, through which communications can
//be done with the server. A HELPER FUNCTION IS REQUIRED in this case. As without 
//it I was getting a Segmentation Fault Error.

/*Help from @StackOverFlow; Title: How to open a Socket Stream*/

int server_setup(int port) {
  int on = 1;
  struct sockaddr_in self;
  int server_soc;
  if ((server_soc = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket steam error");
    return -1;
  }
  if (setsockopt(server_soc, SOL_SOCKET, SO_REUSEADDR,
                 &on, sizeof(on)) == -1) {
    perror("Cannot reuse socket");
  }

  memset(&self, '\0', sizeof(self));
  self.sin_family = AF_INET;
  self.sin_addr.s_addr = INADDR_ANY;
  self.sin_port = htons(port);

  if (bind(server_soc, (struct sockaddr *)&self, sizeof(self)) == -1) {
    perror("The port is already in use"); // probably means port is in use
    return -1;
  }

  if (listen(server_soc, 5) == -1) {
    perror("Cannot listen into the port");
    return -1;
  }
  return server_soc;
}







void fcopy_server(int port){
	int ntohl_value_mode;
	int net_match;
	int net_mismatch;
	socklen_t socklen;
	struct fileinfo *server_aquired_file;
	int len = 1;
	struct stat dets_file;
	char buf;
	char path[255];
	size_t net_fil_size;
	FILE *f;
	int ll;
	char datchar[1];
	int get_inf_socket;
	int server_soc;
	struct sockaddr_in peer;

	if((get_inf_socket = server_setup(port)) < 0){
		perror("Server socket unable to open\n");
		return 1;
	}

	// Initiate infinite loop
	while(1) {
	    socklen = sizeof(peer);
	    // Will be accepting the socket requests, from internal and external.
	    if ((server_soc = accept(get_inf_socket, (struct sockaddr *)&peer, &socklen)) < 0) {
	    perror("The socket did not accept the request to be added");
	    } else {

			bzero(path, sizeof(path));
			while((read(server_soc, path, sizeof(server_aquired_file->path))) > 0){

				//We must read the incoming mode of the file(Dir or Reg). The mode
				//will allow us to determine the type of file it si Dir or Reg.
				read(server_soc, &ntohl_value_mode, sizeof(ntohl_value_mode));

				strcpy(server_aquired_file->path, path);
				server_aquired_file->mode = ntohl(ntohl_value_mode);
				
				//Creating host to network values for MATCH AND MISMATCH, inorder
				//for them to be sent out in the Socket as soon as possible.
				net_match = htonl(MATCH);
				net_mismatch = htonl(MISMATCH);


				//Once getting the mode from the socket, we can determine if file is
				//Dir or Reg. If the file is a Dir, we simply call the function MKDIR
				//with the 'mode'/'permissions' of 0777 (global permissions I belive). 
				//If the directory does not exist it will create it, if it already exists
				//it will not do anything, leaving the directory and its contents.

				//If the files are Regular FIles, then the next obvious check would
				//be to see if the server Regular File and Client Regular File are the same
				// or not. If file does not already exist, then simply fopen with write 
				//permissions and it will create an empty 0 byte file. If the serv and client
				//files do not match then it will write message back to client saying MISMATCH.
				//Then as mentioned above we go through the loop from 0 to the size of the source
				//file and write byte per byte on the serv Reg File.

				//Unfortunately we had to comment out our piece of code, becuase we were
				//continously gettling a 'Segmentation Fault' error. It is my understanding
				//that error comes from memory handeling.

				
				if (S_ISDIR(server_aquired_file->mode))
				{
					
					mkdir(server_aquired_file->path, server_aquired_file->mode & 0777);
					printf("Match for Directory\n");
					write(server_soc, &net_match, sizeof(net_match));

				}else if (S_ISREG(server_aquired_file->mode))
				{
					printf("Mismatch for File\n");
					write(server_soc, &net_mismatch, sizeof(net_mismatch));
					read(server_soc, &net_fil_size, sizeof(net_fil_size));

					printf("Size of Transfered File: %d\n", ntohl(net_fil_size));
					printf("TEXT File PAth: %s\n", server_aquired_file->path);

					ll = fopen(server_aquired_file->path, "w");

					printf("\n");
					printf("\n");
					
					printf("File was created: %d\n", ll);

					printf("\n");
					printf("\n");

					lstat(server_aquired_file->path, &dets_file);

					printf("Size of CREATED File: %d\n", dets_file.st_size);

					printf("\n");
					printf("\n");

					if (dets_file.st_size != ntohl(net_fil_size))
					{
						printf("YAY FILES are not the Same!!\n");
						printf("\n");
						printf("\n");

						write(server_soc, &net_mismatch, sizeof(net_mismatch));
						sleep(0);

						fflush(ll);
						rewind(ll);

						/*for (int i = 0; i < net_fil_size; ++i)
						{
							read(server_soc, &datchar, sizeof(datchar));

							fwrite(&datchar, 1, strlen(datchar), ll);

							memset(datchar, 0, sizeof (datchar));
						}*/

						//memset(datchar, 0, sizeof (datchar));

					}

				}

				bzero(path, sizeof(path));
			}
		}
	}
}














































