#ifndef _REGISTERD_H_
#define _REGISTERD_H_

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct information
{
	char nickname[20];
	char password[20];
	char gender;
	char age[20];
};

extern struct information nap;

int pass();

int name();

int gen_der();

int age();

#endif
