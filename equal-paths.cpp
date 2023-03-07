#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int process(Node * root){
	if(root==NULL) return 0;
	int rheight = process(root->right);
	int lheight = process(root->left);
	if(lheight==-1) return -1;
	if(rheight==-1) return -1;
	if(lheight==rheight) return lheight+1;
	if(lheight==0) return rheight+1;
	if(rheight==0) return lheight+1;
	return -1;
}

bool equalPaths(Node * root)
{
    // Add your code below
    if(process(root)==-1) return false;
		return true;
}

