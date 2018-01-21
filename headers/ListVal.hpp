#ifndef LISTVAL_HPP
#define LISTVAL_HPP
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
using namespace std;


struct node
{
	struct tran{

		int amount;
		char name[1000];
		tran *next;
		
		tran(char *buffer1,int a,tran* n);

	};

		char name[1000];
		int amount;
		node*  next;
		tran*  edge;
		int size;


		node (char *buffer1,int a,node* n);
};


#endif
