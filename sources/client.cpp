#include <iostream>
#include <sys/types.h>	    
#include <sys/socket.h>	  
#include <netinet/in.h>	     
#include <unistd.h>          
#include <netdb.h>	        
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
using namespace std;

int main(int argc,char **argv){
	int flag=0;
	int port, sock, i;
	FILE *fp=NULL;
    struct sockaddr_in server;
    struct sockaddr *serverptr = (struct sockaddr*)&server;
    struct hostent *rem;
    char host[9999];
    if(argc>=7){
    	for(int i=1;i<argc;i=i+2){
    		if(strcmp(argv[i],"-h")==0){
    			strcpy(host,argv[i+1]);
   				 if ((rem = gethostbyname(argv[i+1])) == NULL) {	
	  				 herror("gethostbyname"); exit(1);
    				}
    		}
    		else if(strcmp(argv[i],"-p")==0){
    			if(atoi(argv[i+1])>0){
    				port = atoi(argv[i+1]);
    			}
    		}
    		else if(strcmp(argv[i],"-i")==0){
    			fp=fopen(argv[i+1],"r");
				if(fp==NULL){
					cout << "File Not Found" << endl;
				}
    		}
    	}
    }
    else{
    	cout << "Not enough arguments" << endl;
    	exit(1);
	}
	/* Create socket */
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0){
	  	cout << "error" << endl;
		exit(1);
    }
    server.sin_family = AF_INET;       /* Internet domain */
    memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
    server.sin_port = htons(port);         /* Server port */
    /* Initiate connection */
    if (connect(sock, serverptr, sizeof(server)) < 0){
	   cout << "error" << endl;
		exit(1);
	}
    printf("Connecting to port %d\n", port);

	while(1){
	char operation[1000];
	char operation2[1000];
	if(fp!=NULL){
		if(!feof(fp)){
			fgets(operation,1000,fp);
		}
		else{
			fclose(fp);
			fp=NULL;
		}
		cout << operation << endl;
		cout << "--------------------------" << endl;
	}
	else{
		cout << "add_account <init_amount> <name> [delay]" << endl;//done
		cout << "add_transfer <amount> <src_name> <dst_name> [delay]" << endl;//done
		cout << "add_multi_transfer <amount> <src_name> <dst_name1> ... [delay]" << endl;
		cout << "print_balance <name>" << endl;//done
		cout << "print_multi_balance <name1> ...." << endl;//done
		cout << "sleep <time>" << endl;//done
		cout << "exit" << endl;//done
		fgets(operation, sizeof(operation), stdin);	/* Read from stdin*/
		cout << "--------------------------" << endl;
	}
	strcpy(operation2,operation);
	write(sock, operation, strlen(operation));
	char *temp=strtok(operation2," \n");
	if(strcmp(temp,"exit")==0){
		cout << "Client Close" << endl;
		close(sock);
		exit(1);
	}
	else if(strcmp(temp,"sleep")==0){
		temp=strtok(NULL," \n");
		int i=atoi(temp);
		cout << "Now Client Sleeping for " << i << " seconds" << endl;
		usleep(i);
	}
	else if(strcmp(temp,"print_balance")==0){
		char *rvalue;
		rvalue=new char[1000];
		cout << "print balance" << endl;
		read(sock,rvalue,1000);
		cout << rvalue << endl;
		delete[] rvalue;
		fflush(stdout);
	}
	else if(strcmp(temp,"print_multi_balance")==0){
		cout << "print multi balance" << endl;
		int flag=0;
		char *rvalue;
		rvalue=new char[100000];
		read(sock,rvalue,100000);
		char rvalue2[100000];
		strcpy(rvalue2,rvalue);
		char *temp2=strtok(rvalue2," \n");
		while(temp2!=NULL){
			if(strncmp(temp2,"Failed",strlen("Failed"))==0){
				cout << temp2 << endl;
				char operation3[10000];
				strcpy(operation3,operation);
				char *temp3=strtok(operation3," \n");
				temp3=strtok(NULL," \n");
				while(temp3!=NULL){
					cout << temp3 << endl;
					temp3=strtok(NULL," \n");
				}
				break;
			}
			else{
				cout << temp2 << endl;
				fflush(stdout);
			}
			temp2=strtok(NULL," \n");
		}
		delete[] rvalue;
	}
	else if(strcmp(temp,"add_account")==0){
		char *rvalue;
		rvalue=new char[1000];
		cout << "add account" << endl;
		read(sock,rvalue,1000);
		cout << rvalue << endl;
		delete[] rvalue;
		fflush(stdout);
	}
	else if(strcmp(temp,"add_transfer")==0){
		char *rvalue;
		rvalue=new char[1000];
		cout << "add transfer" << endl;
		read(sock,rvalue,1000);
		cout << rvalue << endl;
		delete[] rvalue;
		fflush(stdout);
	}
	else if(strcmp(temp,"add_multi_transfer")==0){//menh auth
		cout << "add multi transfer" << endl;
		int flag=0;
		char *rvalue;
		rvalue=new char[100000];
		read(sock,rvalue,100000);
		cout << rvalue << endl;
		fflush(stdout);
		delete[] rvalue;
	}
	else{
		cout << "Wrong Command" << endl;
	}
	//from now until the last command it parses the arguments if they are not correct(else statement) show failure if correct pass the id from hashfuction call the correct fuction
	cout << "------------------------" << endl;
	cout << endl;

	}
}
