#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
using namespace std;

class queue{
	struct node
	{
			int fd;
			node*  next;


			node (int a,node* n);
	};


	node *_start;
	int size;
	int max_size;

public:
	queue();
	~queue();
	void destroy();
	void max_size1(int a);
	void insert_fd(int a);
	int get_fd();
	void print_queue();


};


#endif
