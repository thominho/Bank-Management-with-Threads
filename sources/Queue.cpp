#include "../headers/Queue.hpp"

queue::node::node(int a,node *n){
	fd=a;
	next=n;
}

queue::queue(){
	_start=NULL;
	max_size=0;
	size=0;
}

void queue::max_size1(int a){
	max_size=a;
}

void queue::insert_fd(int a){
	_start=new node(a,_start);
	size++;
}

queue::~queue(){
	while(size!=0){
		destroy();
	}
}

void queue::destroy(){
	node *t=_start;
	if(size==0){
		return;
	}
	_start=_start->next;
	delete t;
	size--;
}

int queue::get_fd(){
	node* t=_start;
	_start=_start->next;
	int a=t->fd;
	delete t;
	size--;
	return a;
}

void queue::print_queue(){
	node* t=_start;
	while(t!=NULL){
		cout << t->fd << endl;
		t=t->next;
	}
}