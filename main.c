/*
 * main.c
 *
 *  Created on: Mar 2, 2019
 *      Author: Elijah Basile
 */
#include <stdio.h>
#include <stdlib.h>

#include "interface.h"

//skeletal function for the code
int main() {
	//required command that makes the scanf and printf work properly
	setvbuf(stdout,  NULL,  _IONBF,  0);

	//menu for the program
	menu();

	return 0;
};
