#include "../headers/hashtable.hpp"

/*---------------------------------------------------------------------------------------------------------------------*/

hashtable::hashtable(){
	size=0;
	_start=NULL;
}

/*---------------------------------------------------------------------------------------------------------------------*/

hashtable::~hashtable(){
	while(size!=0){
		destroy();
	}
}

/*---------------------------------------------------------------------------------------------------------------------*/

void hashtable::destroy(){
	node *t=_start;
	if(size==0){
		return;
	}
	_start=_start->next;
	while(t->size!=0){
		node::tran *t2=t->edge;
		t->edge=t->edge->next;
		delete t2;
		t->size--;
	}
	delete t;
	size--;
}

/*---------------------------------------------------------------------------------------------------------------------*/

int hashtable::add_transfer(int amount,char *dest,char *source){
	node *t=_start;
	if(size==0){
		return -1;
	}
	while(t!=NULL){
		if(strcmp(t->name,dest)==0){
			t->amount=t->amount+amount;
			break;
		}
		t=t->next;
	}
	if(t==NULL){
		return -1;
	}
	node::tran *t2=t->edge;
	while(t2!=NULL){
		if(strcmp(t2->name,source)==0){
			t2->amount=t2->amount+amount;
			return 0;
		}
		t2=t2->next;
	}
	t->edge=new node::tran(source,amount,t->edge);
	if(t->edge==NULL){
		cout << "Could Not Add Transcations->RAM problem" << endl;
		exit(1);
	}
	t->size++;
	return 0;
}

/*---------------------------------------------------------------------------------------------------------------------*/

int hashtable::add_account(int amount,char *buffer){
	node *t=_start;
	while(t!=NULL){
		if(strcmp(t->name,buffer)==0){
			return -1;
		}
		t=t->next;
	}
	_start=new node(buffer,amount,_start);
	if(_start==NULL){
		cout << "Could Not Add Transcations->RAM problem" << endl;
		exit(1);
	}
	size++;
	return 0;
}

/*---------------------------------------------------------------------------------------------------------------------*/

int hashtable::print_balance(char *buffer,int fd){
	node *t=_start;
	while(t!=NULL){
		if(strcmp(t->name,buffer)==0){
			char *temp;
			temp=new char[1000];
			char *buffer1;
			buffer1=new char[1000];
			strcpy(temp,"Success:");
			strcat(temp,t->name);
			strcat(temp," ");
			sprintf(buffer1,"%d",t->amount);
			strcat(temp,buffer1);
			cout << "Sending " << temp << endl;
			write(fd, temp, strlen(temp));
			return 0;
		}
		t=t->next;
	}
	return 1;
}

/*---------------------------------------------------------------------------------------------------------------------*/

int hashtable::get_amount(char *buffer,int money){
	node *t=_start;
	while(t!=NULL){
		if(strcmp(t->name,buffer)==0){
			if(t->amount-money<0){
				return -1;
			}
			else{
				t->amount=t->amount-money;
				return 0;
			}
		}
		t=t->next;
	}
	return -1;
}

/*---------------------------------------------------------------------------------------------------------------------*/

int hashtable::exist(char *buffer){
	node *t=_start;
	while(t!=NULL){
		if(strcmp(t->name,buffer)==0){
			return t->amount;
		}
		t=t->next;
	}
	return -1;
}