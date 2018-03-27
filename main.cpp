#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>	     
#include <sys/types.h>	     
#include <sys/socket.h>	     
#include <netinet/in.h>	   
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>        
#include "headers/hashtable.hpp"
#include "headers/ListVal.hpp"
#include "headers/Queue.hpp"
using namespace std;

pthread_cond_t cvar;
pthread_mutex_t mtx2;
queue q;
socklen_t clientlen;





int hashfuction(int a,int size){
	return a%size;
}








struct worker_t{
	hashtable *arg1;
	int   arg2;
	pthread_mutex_t *arg3;
};






void *worker(void *args){
	struct worker_t *data;
	data = (struct worker_t*) args;
	int flag=0;
	int newsock;
	int magkas=0;
	while(1){
		char operation[9999];
		char *temp;
		char *save;
		char operation2[9999];
		if(flag==0){
			pthread_mutex_lock(&mtx2);
			pthread_cond_wait(&cvar,&mtx2);
			pthread_mutex_unlock(&mtx2);
			newsock=q.get_fd();
		}
		flag=1;
		read(newsock, operation, 9999);
		strcpy(operation2,operation);
		temp=strtok_r(operation2," \n",&save);

		if(strcmp(temp,"add_account")==0){
			temp=strtok_r(NULL," \n",&save);//amount
			int amount=atoi(temp);
			char buffer[1000];
			strcpy(buffer,temp);
			temp=strtok_r(NULL," \n",&save);//name
			char name[1000];
			strcpy(name,temp);
			int id=strlen(name);
			int pos=hashfuction(id,data->arg2);
			int delay=0;
			temp=strtok_r(NULL," \n",&save);//delay
			if(temp!=NULL){
				delay=atoi(temp);
			}
			pthread_mutex_lock(&data->arg3[pos]);
			usleep(delay);
			int rvalue=data->arg1[pos].add_account(amount,name);
			pthread_mutex_unlock(&data->arg3[pos]);
			if(rvalue==0){
				char *temp1;
				temp1=new char[1000];
				strcpy(temp1,"Success:");
				strcat(temp1,name);
				strcat(temp1," ");
				strcat(temp1,buffer);
				cout << "Sending " << temp1 << endl;
				write(newsock, temp1, strlen(temp1));
				delete[] temp1;
			}
			else{
				char *temp1;
				temp1=new char[1000];
				strcpy(temp1,"Failed:");
				strcat(temp1,name);
				strcat(temp1," ");
				strcat(temp1,buffer);
				cout << "Sending " << temp1 << endl;
				write(newsock, temp1, strlen(temp1));
				delete[] temp1;
			}
		}
		else if(strcmp(temp,"print_balance")==0){
			temp=strtok_r(NULL," \n",&save);
			int id=strlen(temp);
			int pos=hashfuction(id,data->arg2);
			char name[1000];
			strcpy(name,temp);
			temp=strtok_r(NULL," \n",&save);
			pthread_mutex_lock(&data->arg3[pos]);
			int retval=data->arg1[pos].print_balance(name,newsock);
			pthread_mutex_unlock(&data->arg3[pos]);
			if(retval==1){
				char *temp1;
				temp1=new char[1000];
				strcpy(temp1,"Failed:");
				strcat(temp1,name);
				cout << "Sending " << temp1 << endl;
				write(newsock, temp1, strlen(temp1));
				delete[] temp1;
			}
		}

		else if(strcmp(temp,"add_transfer")==0){
			temp=strtok_r(NULL," \n",&save);//amount
			int amount=atoi(temp);
			char buffer[1000];
			char name[1000];
			char name2[1000];
			strcpy(buffer,temp);
			temp=strtok_r(NULL," \n",&save);//source
			int id=strlen(temp);
			int pos=hashfuction(id,data->arg2);
			strcpy(name,temp);
			temp=strtok_r(NULL," \n",&save);//destination
			int id2=strlen(temp);
			int pos2=hashfuction(id2,data->arg2);
			strcpy(name2,temp);
			int max[2];
			max[1]=pos;
			max[2]=pos2;
			if(max[1]<max[2]){
				int temp=max[1];
				max[1]=max[2];
				max[2]=temp;
			}
			pthread_mutex_lock(&data->arg3[max[1]]);
			if(max[1]!=max[2]){
				pthread_mutex_lock(&data->arg3[max[2]]);
			}
			temp=strtok_r(NULL," \n",&save);//delay
			int delay=0;
			if(temp!=NULL){
				delay=atoi(temp);
			}
			int rvalue=data->arg1[pos2].exist(name2);
			if(rvalue>=0){
				rvalue=data->arg1[pos].get_amount(name,amount);
			}
			if(rvalue==0){
				usleep(delay);
				rvalue=data->arg1[pos2].add_transfer(amount,name2,name);
				if(rvalue==0){
					char *temp1;
					temp1=new char[1000];
					strcpy(temp1,"Success:");
					strcat(temp1,name);
					strcat(temp1," ");
					strcat(temp1,name2);
					strcat(temp1," ");
					strcat(temp1,buffer);
					cout << "Sending " << temp1 << endl;
					write(newsock, temp1, strlen(temp1));
					delete[] temp1;
				}
				else{
					char *temp1;
					temp1=new char[1000];
					strcpy(temp1,"Failed1:");
					strcat(temp1,name);
					strcat(temp1," ");
					strcat(temp1,name2);
					strcat(temp1," ");
					strcat(temp1,buffer);
					cout << "Sending " << temp1 << endl;
					write(newsock, temp1, strlen(temp1));
					delete[] temp1;
				}
			}
			else{
					char *temp1;
					temp1=new char[1000];
					strcpy(temp1,"Failed2:");
					strcat(temp1,name);
					strcat(temp1," ");
					strcat(temp1,name2);
					strcat(temp1," ");
					strcat(temp1,buffer);
					cout << "Sending " << temp1 << endl;
					write(newsock, temp1, strlen(temp1));
					delete[] temp1;
			}
			if(max[1]!=max[2]){
				pthread_mutex_unlock(&data->arg3[max[2]]);
			}
			pthread_mutex_unlock(&data->arg3[max[1]]);
		}

		else if(strcmp(temp,"print_multi_balance")==0){
			temp=strtok_r(NULL," \n",&save);
			int all=0;
			int *max;
			max=(int *) malloc(sizeof(int)*1000);
			while(temp!=NULL){//collect all pos
				int id=strlen(temp);
				int flag3=0;
				int pos=hashfuction(id,data->arg2);
				for(int i=0;i<all;i++){
					if(max[i]==pos){
						flag3=1;
					}
				}
				if(flag3==0){
					max[all]=pos;
					all=all+1;
				}
				temp=strtok_r(NULL," \n",&save);
			}
			max=(int *) realloc(max,sizeof(int)*all);
			for(int i=0;i<all;i++){//sort them
				for(int j=0; j<all-1;j++){
					if(max[j]<max[j+1]){
						int temp4=max[j];
						max[j]=max[j+1];
						max[j+1]=temp4;
					}
				}
			}
			char operation3[9999];
			strcpy(operation3,operation);
			char *temp2=strtok_r(operation3," \n",&save);
			temp2=strtok_r(NULL," \n",&save);
			int flag2=0;
			for(int i=0;i<all;i++){//lock them and search if they all exist
				pthread_mutex_lock(&data->arg3[max[i]]);
			}
			char buffer[100000];
			strcpy(buffer,temp2);
			int i=0;
			while(temp2!=NULL && flag2==0){
				int id=strlen(temp2);
				int pos=hashfuction(id,data->arg2);
				char name[100];
				strcpy(name,temp2);
				int amount=data->arg1[pos].exist(name);
				if(amount==-1){
					flag2=1;
				}
				else if(i==0){
					char buffer2[50];
					sprintf(buffer2,"%d",amount);
					strcat(buffer,":");
					strcat(buffer,buffer2);
					strcat(buffer," ");
					i++;
				}
				else{
					char buffer2[50];
					strcat(buffer,temp2);
					sprintf(buffer2,"%d",amount);
					strcat(buffer,":");
					strcat(buffer,buffer2);
					strcat(buffer," ");
				}
				temp2=strtok_r(NULL," \n",&save);
			}
			if(flag2==1){//error
				char *temp1;
				temp1=new char[1000];
				strcpy(temp1,"Failed");
				cout << "Sending " << temp1 << endl;
				write(newsock,temp1,strlen(temp1));
				delete[] temp1;
			}
			else{
				cout << "Sending " << buffer << endl;
				write(newsock,buffer,strlen(buffer));
			}
			for(int i=0;i<all;i++){
				pthread_mutex_unlock(&data->arg3[max[i]]);
			}
			free(max);
		}

		else if(strcmp(temp,"add_multi_transfer")==0){
			temp=strtok_r(NULL," \n",&save);
			int all=0;
			int *max;
			max=(int *) malloc(sizeof(int)*1000);
			int amount=atoi(temp);
			int delay=0;
			int flag2=0;
			int counter=0;
			temp=strtok_r(NULL," \n",&save);
			while(temp!=NULL){//collect all pos
				if(atoi(temp)==0){
					int id=strlen(temp);
					int flag3=0;
					int pos=hashfuction(id,data->arg2);
					for(int i=0;i<all;i++){
						if(max[i]==pos){
							flag3=1;
						}
					}
					if(flag3==0){
						max[all]=pos;
						all=all+1;
					}
					counter=counter+1;
				}
				else{
					delay=atoi(temp);
				}
				temp=strtok_r(NULL," \n",&save);
			}
			int amount2=amount;
			amount=amount*(counter-1);
			max=(int *) realloc(max,sizeof(int)*all);
			for(int i=0;i<all;i++){//sort them
				for(int j=0; j<all-1;j++){
					if(max[j]<max[j+1]){
						int temp4=max[j];
						max[j]=max[j+1];
						max[j+1]=temp4;
					}
				}
			}
			char operation3[9999];
			strcpy(operation3,operation);
			char *temp2=strtok_r(operation3," \n",&save);
			temp2=strtok_r(NULL," \n",&save);
			for(int i=0;i<all;i++){//lock them
				pthread_mutex_lock(&data->arg3[max[i]]);
			}
			usleep(delay);
			temp2=strtok_r(NULL," \n",&save);
			char buffer[100000];
			int id=strlen(temp2);
			int pos=hashfuction(id,data->arg2);
			char name[100];
			strcpy(name,temp2);
			char buffer2[1000];
			sprintf(buffer2,"%d",amount);
			strcpy(buffer,name);
			strcat(buffer,":");
			strcat(buffer,buffer2);
			while(temp2!=NULL){
				if(atoi(temp2)==0){
					int id=strlen(temp2);
					int pos=hashfuction(id,data->arg2);
					char name2[100];
					strcpy(name2,temp2);
					int rvalue=data->arg1[pos].exist(name2);
					if(rvalue==-1){
						flag2=1;
					}
				}
				temp2=strtok_r(NULL," \n",&save);
			}
			if(flag2==0){
				flag2=data->arg1[pos].get_amount(name,amount);
			}
			if(flag2<0){
				char *temp1;
				temp1=new char[1000];
				strcpy(temp1,"Failed");
				strcat(temp1,":");
				strcat(temp1,buffer);
				cout << "Sending " << temp1 << endl;
				write(newsock,temp1,strlen(temp1));
				delete[] temp1;
			}
			else{
				char operation4[9999];
				strcpy(operation4,operation);
				char *temp3=strtok_r(operation4," \n",&save);
				temp3=strtok_r(NULL," \n",&save);
				temp3=strtok_r(NULL," \n",&save);
				temp3=strtok_r(NULL," \n",&save);
				while(temp3!=NULL){
					if(atoi(temp3)==0){
						int id=strlen(temp3);
						int pos=hashfuction(id,data->arg2);
						char name2[100];
						strcpy(name2,temp3);
						data->arg1[pos].add_transfer(amount2,name2,name);
					}
					temp3=strtok_r(NULL," \n",&save);
				}
				strcat(buffer," SUCCESS");
				cout << "Sending " << buffer << endl;
				write(newsock,buffer,strlen(buffer));
			}
			for(int i=0;i<all;i++){
				pthread_mutex_unlock(&data->arg3[max[i]]);
			}
			free(max);
		}

		else if(strcmp(temp,"exit")==0){
			flag=0;
		}
	}
}





/*-------------------------------------------------------------------MAIN-----------------------------------------------------------------------------*/




int main(int argc,char **argv){
	hashtable *h;
	pthread_t *tids;
	pthread_mutex_t *mtx;       
	int size=10000;
	int pool; //threads
	int qsize; //queue size
    int port, sock;
    struct sockaddr_in server,client;
    struct sockaddr *serverptr=(struct sockaddr *)&server;
	struct sockaddr *clientptr=(struct sockaddr *)&client;
    if(argc>=7){
    	for(int i=1;i<7;i=i+2){
    		if(strcmp(argv[i],"-p")==0){
    			if(atoi(argv[i+1])>0){
    				port = atoi(argv[i+1]);
    			}
    		}
    		else if(strcmp(argv[i],"-s")==0){
    			if(atoi(argv[i+1])>0){
    				pool = atoi(argv[i+1]);
    			}
    		}
    		else if(strcmp(argv[i],"-q")==0){
    			if(atoi(argv[i+1])>0){
    				qsize = atoi(argv[i+1]);
    			}
    		}
    	}
    }
    else{
    	cout << "Not enough arguments" << endl;
    	exit(1);
	}
	pthread_mutex_init(&mtx2, 0);
	q.max_size1(qsize);
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0){
  		cout << "No socket" << endl;
        exit(1);
    }
    server.sin_family = AF_INET;       /* Internet domain */
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);      /* The given port */
    /* Bind socket to address */
    if (bind(sock, serverptr, sizeof(server)) < 0){
        cout << "No bind" << endl;
        exit(1);
	}	
    /* Listen for connections */
    if (listen(sock, 5) < 0){ 
        cout << "No listen" << endl;
        exit(1);
    }
    printf("Listening for connections to port %d\n", port);
	h=new hashtable[size];
	mtx=(pthread_mutex_t *) malloc(sizeof(pthread_mutex_t)*size);
	for(int i=0;i<size;i++){
		pthread_mutex_init(&mtx[i], NULL);
	}
	tids =(pthread_t *) malloc(pool * sizeof(pthread_t));
	pthread_cond_init(&cvar, 0);
	for(int i=0;i<pool;i++){
		if(i!=0){
			worker_t data;
			data.arg1=h;
			data.arg2=size;
			data.arg3=mtx;
			pthread_create(&tids[i],NULL,worker,(void *) &data);
		}
	}
	while(1){
		int flag=1;
		int newsock;
		while(flag==1){
			newsock = accept(sock, clientptr, &clientlen);
			if(newsock>0){
				flag=0;
			}
		}
			q.insert_fd(newsock);
			//q.print_queue();
			pthread_cond_signal(&cvar);
	}
	for(int i=0;i<pool;i++){
		pthread_join(tids[i],NULL);
	}
	pthread_cond_destroy(&cvar);
	if(h!=NULL){
	delete[] h;
	}
}
