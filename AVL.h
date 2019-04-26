/*
 * AVL.h
 *
 *  Created on: Mar 3, 2019
 *      Author: Elijah Basile
 */

#ifndef AVL_H_
#define AVL_H_

#include <stdbool.h>

#define LH +1 	// Left High
#define EH  0 	// Even High
#define RH -1 	// Right High



//Structure Declarations

typedef struct {
	int low;
	int day;
	int month;
	int year;
} DATA;

typedef struct node {
	struct node* left;
	DATA* data;
	int bal;
	struct node* right;
} NODE;

typedef struct {
	int count;
	NODE* root;
	int (*compare) (int argu1, int argu2);
} AVL_TREE;

AVL_TREE* AVL_Create (int (*compare) (int,int));
bool AVL_Insert (AVL_TREE*,DATA*);
NODE* _insert(AVL_TREE*,NODE*,NODE*,bool*);
NODE* insLeftBal (NODE*,bool*);
NODE* insRightBal (NODE*,bool*); // new
NODE* rotateLeft(NODE*);
NODE* rotateRight(NODE*);
bool AVL_Delete(AVL_TREE*,int);
NODE* _delete(AVL_TREE*,NODE*,int,bool*,bool*);
NODE* dltRightBal(NODE*,bool*);
NODE* dltLeftBal(NODE*,bool*); // new
bool AVL_Destroy(AVL_TREE*);


#endif /* AVL_H_ */
