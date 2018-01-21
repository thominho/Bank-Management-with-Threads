#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "ListVal.hpp"
using namespace std;

class hashtable{
private:


	node *_start;
	int size;


public:

	//na ftia3w tis delete node kai delete trans
	hashtable();
	~hashtable();
	void destroy();
	int add_account(int amount,char *buffer);
	int add_transfer(int amount,char *dest,char *source);
	int print_balance(char *buffer,int fd);
	int get_amount(char *buffer,int money);
	int exist(char *buffer);

};



#endif
