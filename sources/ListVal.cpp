#include "../headers/ListVal.hpp"



node::tran::tran(char *buffer,int a,tran* n){
	amount=a;
	strcpy(name,buffer);
	next=n;
}

node::node (char *buffer,int a,node* n){
		amount=a;
		strcpy(name,buffer);
		next=n;
		edge=NULL;
		size=0;
}