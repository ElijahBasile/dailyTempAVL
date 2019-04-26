/*
 * interface.c
 *
 *  Created on: Mar 3, 2019
 *      Author: Elijah Basile
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "interface.h"
#include "AVL.h"


// interface for user, serves as the skeletal function for function calls throughout the program
void menu() {
	char choice;
	AVL_TREE* tree = AVL_Create((*compare));
	DATA *low = NULL, *high = NULL;

	// select (s), add (a), request (r), and quit (q)
	while (true) {
		printf("========================= Menu =========================\n");
		printf("s: 		Select CSV file to convert to AVL Tree          \n");
		printf("a:		Add new entry.                                  \n");
		printf("r:		Request highest and lowest temperatures entered.\n");
		printf("q:		Quit the program.                               \n");

		fflush(stdin);
		choice = getchar();
		switch (choice) {
			case 's': // select csv file
				convertData(tree);
				break;
			case 'a': // add new entry
				addNewEntry(tree);
				break;
			case 'r': // request highs and lows (if the same, the function will output the same number twice)
				if (tree->root == NULL) {
					printf("No inputed entries.\n");
				} else {
					low = getLow(tree);
					high = getHigh(tree);
					if (low != NULL)
						printf("Lowest Temperature and Date: %d/%d/%d, %d degrees\n",low->month,low->day,low->year,low->low);
					if (high != NULL)
						printf("Highest Temperature and Date:  %d/%d/%d, %d degrees\n",high->month,high->day,high->year,high->low);
				}
				break;
			case 'q':
				exit(0);
			default:
				printf("Enter valid input.\n");
		}
	}
}

// gets the lowest temperature in the tree by moving left
DATA* getLow(AVL_TREE* tree) {
	NODE* cur = tree->root;
	while(cur->left != NULL) {
		cur = cur->left;
	}
	return cur->data;
}

// much like getHigh except moves right for the high
DATA* getHigh(AVL_TREE* tree) {
	NODE* cur = tree->root;
	while(cur->right != NULL) {
		cur = cur->right;
	}
	return cur->data;
}

// adds new entry to the tree, user interface for inserting into tree (not kitchen sink)
void addNewEntry(AVL_TREE* tree) {
	int day,month,year,low;
	DATA* data;
	if (!(data = (DATA*)malloc(sizeof(DATA)))) {
		printf("Out of Memory. Cannot add.\n");
		exit(137);
	}

	do {
		printf("Enter the Year of the Date you are entering:\n");
		fflush(stdin);
		scanf("%d",&year);
	} while (year <= 0 || year > 2019);


	do {
		printf("Enter the Month of the Date you are entering:\n");
		fflush(stdin);
		scanf("%d",&month);
	} while (month > 12 || month <= 0);


	do {
		printf("Enter the Day of the Date you are entering:\n");
		fflush(stdin);
		scanf("%d",&day);
	} while (day <= 0 || day > 31);

	do {
		printf("Enter the Low Temperature of the Date you are entering:\n");
		fflush(stdin);
		scanf("%d",&low);
	} while (low <= -130 || low >= 140);

	data->day = day;
	data->month = month;
	data->year = year;
	data->low = low;
	AVL_Insert(tree,data);
}

// converts csv file data into the tree in the form of nodes
void convertData(AVL_TREE* tree) {
	FILE* fptr;
	char filename[256];
	bool isValid = false;
	char choice;

	do {
		fflush(stdin);
		printf("Enter the CSV file you wish to extract the temperatures from.\n");
		scanf("%s",filename);
		if (!(fptr = fopen(filename,"r"))) {
			printf("******* File Not Found *******\n");
			printf("e:		Enter a new file name \n");
			printf("q:		Quit this Menu.		  \n");
			while (!isValid) {
				isValid = true;
				fflush(stdin);
				choice = getchar();
				switch (choice) {
					case 'e':
						break;
					case 'q':
						return;
					default:
						printf("Enter valid input.\n");
						isValid = false;
				}
			}
		} else
			break;
	} while (true);
	buildTreeFromFile(tree,fptr);

}

// this builds tree from the file
void buildTreeFromFile(AVL_TREE* tree,FILE* fptr) {
	char* line = (char*)malloc(sizeof(char)*256);

	while(!feof(fptr)) {
		fgets(line,256,fptr);
		AVL_Insert(tree,convertCSVFormat(tree,line));
	}
	free(line);
}

// converts csv file data to a DATA node.
DATA* convertCSVFormat(AVL_TREE* tree,char* line) {\
	DATA* data = (DATA*)malloc(sizeof(DATA));
	int j;
	char day[5];
	char month[5];
	char year[7];
	char low[5];

	for(j = 0; *line != '/'; j++) {
		month[j] = *line;
		line++;
	}
	line++;
	j++;
	month[j] = '\0';
	data->month = atoi(month);
	for(j = 0; *line != '/'; j++) {
		day[j] = *line;
		line++;
	}
	line++;
	j++;
	day[j] = '\0';
	data->day = atoi(day);
	for(j = 0; *line != ','; j++) {
			year[j] = *line;
			line++;
	}
	line++;
	j++;
	year[j++] = '\0';
	data->year = atoi(year);
	for(j = 0; *line != '\0'; j++) {
		low[j] = *line;
		line++;
	}
	line++;
	j++;
	low[j++] = '\0';
	data->low = atoi(low);

	return data;
}

// built-in compare function for simple integers
int compare(int argu1, int argu2) {
	if (argu1 > argu2) {
		return 1;
	} else if (argu1 < argu2) {
		return -1;
	} else {
		return 0;
	}
}


