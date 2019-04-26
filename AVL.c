/*
 * AVL.c
 *
 *  Created on: Mar 3, 2019
 *      Author: Elijah Basile
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "AVL.h"

AVL_TREE* AVL_Create (int (*compare) (int argu1, int argu2)) {
	//local definitions
	AVL_TREE* tree;

	//statements
	tree = (AVL_TREE*)malloc(sizeof(AVL_TREE));
	if(tree) {
		tree->root = NULL;
		tree->count = 0;
		tree->compare = compare;
	} // end if

	return tree;
} // AVL_Create

bool AVL_Insert (AVL_TREE* tree,DATA* dataInPtr) {
	//Local definitions
	NODE* newPtr;
	bool forTaller;

	//statements
	newPtr = (NODE*)malloc(sizeof(NODE));
	if (!newPtr)
		return false;

	newPtr->bal		= EH;
	newPtr->right	= NULL;
	newPtr->left	= NULL;
	newPtr->data	= dataInPtr;

	tree->root = _insert(tree, tree->root, newPtr, &forTaller);
	(tree->count)++;
	return true;
} // AVL_Insert


NODE* _insert(AVL_TREE* tree,NODE* root,NODE* newPtr,bool* taller) {
	if (!root) {
		// insert at root
		root = newPtr;
		*taller = true;
		return root;
	}

	if (tree->compare(newPtr->data->low,root->data->low) < 0) {
		//newData < root -- go left
		root->left = _insert(tree, root->left, newPtr, taller);
		if(*taller)
			// Left subtree is taller
			switch(root->bal) {
				case LH: // Was left high--rotate
					root = insLeftBal(root,taller);
					break;
				case EH: // Was balanced--now LH
					root->bal = LH;
					break;
				case RH: // Was right high--now EH
					root->bal = EH;
					*taller = false;
					break;
			} // new < node
	}
	else {
		root->right = _insert(tree, root->right, newPtr, taller);
		if (*taller)
			// Right subtree is taller
			switch (root->bal) {
				case LH: // Was left high--now EH
					root->bal = EH;
					*taller = false;
					break;

				case EH: // Was balanced--now RH
					root->bal = RH;
					break;

				case RH: // was right high--rotate
					root = insRightBal(root, taller);
			} //switch
		return root;
		} //else new data >= root data
	return root;
} // _insert

NODE* insLeftBal (NODE* root,bool* taller) {
	//local definitions
	NODE* rightTree;
	NODE* leftTree;

	//statements
	leftTree = root->left;
	switch (leftTree->bal) {
		case LH: //Left High --Rotate Right
			root->bal = EH;
			leftTree->bal = EH;

			//Rotate Right
			root = rotateRight(root);
			*taller = false;
			break;
		case EH: // this is an error
			printf("\n\aError in insLeftBal\n");
			exit(100);
		case RH: // Right High -- Requires double
				 // Rotation: first left, then right
			rightTree = leftTree->right;
			switch(rightTree->bal) {
				case LH:
					root->bal = RH;
					leftTree->bal = EH;
					break;
				case EH:
					root->bal = EH;
					leftTree->bal = LH;
					break;
				case RH:
					root->bal = EH;
					leftTree->bal = LH;
					break;
			} // switch rightTree

			rightTree->bal = EH;
			//Rotate Left
			root->left = rotateLeft(leftTree);

			//Rotate Right
			root = rotateRight (root);
			*taller = false;
	} //switch
	return root;
} //left balance

NODE* insRightBal (NODE* root,bool* taller) {
	//local definitions
	NODE* rightTree;
	NODE* leftTree;

	//statements
	rightTree = root->right;
	switch (rightTree->bal) {
		case LH: //Left High --Requires double
				// rotation: first right, then left
			leftTree = rightTree->left;
			switch(leftTree->bal) {
				case LH:
					root->bal = EH;
					leftTree->bal = RH;
					break;
				case EH:
					root->bal = EH;
					leftTree->bal = RH;
					break;
				case RH:
					root->bal = LH;
					leftTree->bal = EH;
					break;
			} // switch rightTree

			leftTree->bal = EH;
			//Rotate Left
			root->left = rotateRight(rightTree);

			//Rotate Right
			root = rotateLeft (root);
			*taller = false;
			break;
		case EH: // this is an error
			printf("\n\aError in insLeftBal\n");
			exit(100);
		case RH:  //Right High --Rotate Left
			root->bal = EH;
			rightTree->bal = EH;

			//Rotate Right
			root = rotateLeft(root);
			*taller = false;
			break;

	} //switch
	return root;
}

NODE* rotateLeft(NODE* root) {
	//local definitions
	NODE* tempPtr;

	//statements
	tempPtr = root->right;
	root->right = tempPtr->left;
	tempPtr->left = root;

	return tempPtr;
} //rotateLeft

NODE* rotateRight(NODE* root) {
	//local definitions
	NODE* tempPtr;

	//statements
	tempPtr = root->left;
	root->left = tempPtr->right;
	tempPtr->right = root;

	return tempPtr;
} //rotateRight

bool AVL_Delete(AVL_TREE* tree,int dltKey) {
	//local definitions
	bool shorter;
	bool success;
	NODE* newRoot;

	//statements
	newRoot = _delete(tree, tree->root,dltKey,&shorter,&success);

	if(success) {
		tree->root = newRoot;
		(tree->count)--;
		return true;
	} // if
	else
		return false;
} // AVL_Delete

NODE* _delete(AVL_TREE* tree,NODE* root,int dltKey,bool* shorter,bool* success) {
	//local definitions
	NODE* dltPtr;
	NODE* exchPtr;
	NODE* newRoot;

	//statements
	if (!root) {
		*shorter = false;
		*success = false;
		return NULL;
	}

	if (tree->compare(dltKey,root->data->low) < 0) {	// NOTE: Take a closer look at the built-in compare function in the node
		root->left = _delete (tree, root->left, dltKey, shorter, success);
		if (*shorter)
			root = dltLeftBal(root,shorter);
	} // if greater
	else { // found equal node
		dltPtr = root;
		if (!root->right) { // only left subtree
			newRoot = root->left;
			*success = true;
			*shorter = true;
			free(dltPtr);
			return newRoot;
		} else {
			if (!root->left) {
				newRoot = root->right;

				*success = true;
				*shorter = true;
				free(dltPtr);
				return newRoot;
			} else {
				exchPtr = root->left;
				while (exchPtr->right)
					exchPtr = exchPtr->right;
				root->data = exchPtr->data;
				root->left = _delete(tree, root->left,exchPtr->data->low,shorter,success);
				if(*shorter)
					root = dltRightBal(root,shorter);
			} // else
		}
	}// equal node
	return root;
}


NODE* dltRightBal(NODE* root,bool* shorter) {
	//local definitions
	NODE* rightTree;
	NODE* leftTree;

	//statements
	switch (root->bal) {
		case LH: //deleted left--now balanced
			root->bal = EH;
			break;
		case EH: //now right high
			root->bal = RH;
			*shorter = false;
			break;
		case RH: // right high - rotate left
			rightTree = root->right;
			if(rightTree->bal == LH) { //double rotation required
				leftTree = rightTree->left;

				switch(leftTree->bal) {
					case LH:
						rightTree->bal = RH;
						root->bal = EH;
						break;
					case EH:
						root->bal = EH;
						rightTree->bal = EH;
						break;
					case RH:
						root->bal = LH;
						rightTree->bal = EH;
						break;
				} // switch

			leftTree->bal = EH;

			//Rotate right then left
			root->right = rotateRight (rightTree);
			root = rotateLeft (root);
			}  else {// if rightTree->bal == LH
				switch (rightTree->bal) {
					case LH:
					case RH:
						root->bal = EH;
						rightTree->bal = EH;
						break;
					case EH:
						root->bal = RH;
						rightTree->bal = LH;
						*shorter = false;
						break;
				} // switch rightTree->bal
				root = rotateLeft(root);
			}	//else
	} // switch
	return root;
}

NODE* dltLeftBal(NODE* root,bool* shorter) {
	//local definitions
	NODE* rightTree;
	NODE* leftTree;

	//statements
	switch (root->bal) {
		case RH: //deleted right--now balanced
			root->bal = EH;
			break;
		case EH: //now left high
			root->bal = LH;
			*shorter = false;
			break;
		case LH: // left high - rotate right
			leftTree = root->left;
			if(leftTree->bal == LH) { //double rotation required
				rightTree = leftTree->right;

				switch(leftTree->bal) {
					case LH:
						leftTree->bal = RH;
						root->bal = EH;
						break;
					case EH:
						root->bal = EH;
						leftTree->bal = EH;
						break;
					case RH:
						root->bal = LH;
						leftTree->bal = EH;
						break;
				} // switch

			rightTree->bal = EH;

			//Rotate right then left
			root->left = rotateLeft (leftTree);
			root = rotateRight (root);
			}  else {// if rightTree->bal == LH
				switch (leftTree->bal) {
					case RH:
					case LH:
						root->bal = EH;
						leftTree->bal = EH;
						break;
					case EH:
						root->bal = LH;
						leftTree->bal = RH;
						*shorter = false;
						break;
				} // switch rightTree->bal
				root = rotateRight(root);
			}	//else
	} // switch
	return root;
}

//function to destroy AVL tree once program has completed
bool AVL_Destroy(AVL_TREE* tree) {
	if (tree->root == NULL) // empty tree
		return false;
	while(AVL_Delete(tree,tree->root->data->low));
	return true;
}













