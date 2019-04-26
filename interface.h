/*
 * interface.h
 *
 *  Created on: Mar 3, 2019
 *      Author: Elijah Basile
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_

#include "AVL.h"

void menu();
DATA* getLow(AVL_TREE*);
DATA* getHigh(AVL_TREE*);
void addNewEntry(AVL_TREE*);
void convertData(AVL_TREE*);
void buildTreeFromFile(AVL_TREE*,FILE*);
DATA* convertCSVFormat(AVL_TREE*,char*);
int compare(int,int);

#endif /* INTERFACE_H_ */
