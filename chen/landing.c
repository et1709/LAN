#include<stdio.h>
#include<stdlib.h>
#include"registered.h"
#include<string.h>

extern int register_func(struct information *);


int main(void)
{
	struct information denglu;
	register_func(&denglu);
	dengluxx(&denglu);


	return 0;

}
