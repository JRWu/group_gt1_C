/********************************************************************************
 * avlTree.c
 *
 * To be used as the data structure for group_gt1
 * Version 1.0
 *
 * Author: Jia Rong Wu
 * jwu424@uwo.ca
 *
 * This software is Copyright 2014 Jia Rong Wu and is distrubuted under the terms
 * of the GNU General Public License.
 *
 * avlTree.c represents a tree data structure with guaranteed height (AVL)
 * Can be used for efficient sorting or storage of any STRING/INT values
 *******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include "avlTree.h"


// Function Prototypes (private functions)
int computeHeight(avlNode node);
void MakeLeftChild (avlNode* a, avlNode* b);
void MakeRightChild (avlNode* a, avlNode* b);

/**
 * avlTree_init allocates necessary memory for a pointer to the first element in a tree
 * @return pointer to pointer of first node in tree
 */
avlTree avlTree_init(void)
{
    avlTree tree;
    tree = (avlTree) malloc(sizeof(avlNode));
    *tree = NULL;
    return tree;
}

/**
 * avlTree_insert takes a sequence and identifier, and inserts it into a tree
 * @s represents the sequence to be inserted
 * @id represents the identifier of the sequence
 * @return a pointer to the node that it was stored at
 */
avlNode* avlTree_insert(avlTree tree, char* s, char* id)
{
    if (*tree == NULL) // Unique sequence [BASE CASE]
    {
        (*tree) = (avlNode*) malloc (sizeof(avlNode));
        (*tree) -> count = 1;       // Increment to 1 because unique
        (*tree) -> seq = s;         // Set sequence
        (*tree) -> identifier = id; // Set id of the node
        
        (*tree)->height = computeHeight(**tree);
        return *tree;
    }
    else
    {
        int comparator = strcmp((*tree)->seq, s);
        if (comparator < 0) // New seq is larger, insert right
        {
            (*tree)->right_child = avlTree_insert(&(*tree)->right_child,s,id);
            ((*tree)->right_child)->parent = *tree; // Set parent of new node
            (*tree)->height = computeHeight(**tree); //ERROR
        }
        else if (comparator > 0) // New seq is less, insert left
        {
            (*tree)->left_child = avlTree_insert(&(*tree)->left_child,s,id);
            ((*tree)->left_child)->parent = *tree; // Set parent of new node
            (*tree)->height = computeHeight(**tree); //ERROR

        }
        else // Strings equivalent, increment counter for seq
        {
            
            // Add code to append identifiers later*
            
            (*tree)->count ++; // Increment seq counter
            return (*tree);
        }
    }
    return *tree;
}


/**
 * inOrder_traversal walks the tree from smallest to largest element
 * takes O(n) runtime, n being the number of elements in the tree
 * @tree is the tree being traversed
 */
void inOrder_traversal(avlTree tree)
{
    if (*tree != NULL)
    {
        inOrder_traversal( &(*tree)->left_child );
        printf("Height: %d\n", (*tree)->height);    // DEBUG, REMOVE LATER
        printf("Seq: %s\n", (*tree)->seq);          // DEBUG, REMOVE LATER
        printf("Count: %d\n\n", (*tree)->count);    // DEBUG, REMOVE LATER
        inOrder_traversal( &(*tree)->right_child );
    }
}


/**
 * external determines if a given node is a leaf node
 * @node represents the node being checked
 * @return true if the node is external, false otherwise
 */
bool external(avlNode node)
{
    if ((node).right_child == NULL && (node).left_child == NULL)
    {
        return true;
    }
    
    return false;
}

/**
 * computeHeight takes a given node and recursively calculates its height
 * @node is the node whose height is being calculated
 * @return the height of the node
 */
int computeHeight(avlNode node)
{
    int left = 0;
    int right = 0;

    if (external(node)) // External nodes count as 0
    {
        return 1;
    }
    
    if (node.left_child != NULL) // find left subtree height
    {
        left = computeHeight(*(node.left_child));
    }
    if (node.right_child!=NULL) // find right subtree height
    {
        right = computeHeight(*(node.right_child));
    }
    
    return (1 + max(left,right)); // Return max of left and right + 1 for current
}

avlNode* triNodeRestructure(avlNode* grandchild,avlNode* child, avlNode* unbalanced)
{
    avlNode* x = malloc (sizeof(avlNode));
    x = grandchild;
    avlNode* y = malloc (sizeof(avlNode));
    y = child;
    avlNode* z = malloc (sizeof(avlNode));
    z = unbalanced;
    
    // CASES
    // ADD STRCMP HERE
    // NOTE: If strcmp is too unweildy must fix this here (BOTTLENECK)
    int zx = strcmp(z->seq, x->seq);
    int xy = strcmp(x->seq, y->seq);
    int zy = strcmp(z->seq, y->seq);
    
    // Might have to call malloc for these
    avlNode* a = malloc (sizeof(avlNode));
    avlNode* b = malloc (sizeof(avlNode));
    avlNode* c = malloc (sizeof(avlNode));
    
    if(zx <= 0 && xy <= 0) // If z <= x and x<=y
   	{
        a = z;
        b = x;
        c = y;
   	}
   	else if (zx >= 0 && xy >= 0) // If z>=x and x >=y
   	{
        a = y;
        b = x;
        c = z;
   	}
   	else if (zy <= 0 && xy >=0) // If z<=y and y <=x
   	{
        a = z;
        b = y;
        c = x;
   	}
    
   	else //if(zy >=0 && xy <=0) // If z>=y and y >=x
   	{
        a = x;
        b = y;
        c = z;
   	}
   	
    
    
    if (z->parent == NULL ) // Reached root
    {
        // Set left and right of b as z's left and right
        // set left parent and right parent of z as b
        // Remove parent of b
        
        z->left_child->parent = b;
        z->right_child->parent = b;
        
        b->left_child = z->left_child;
        b->right_child = z ->right_child;
        
        b->parent = NULL;

    }
    else // Replacing Z
    {
        if (z->parent->left_child == z)
        {
            MakeLeftChild(z->parent, b);
//            z->parent->left_child = b;
//            b->parent = z;
        }
        else
        {
            MakeRightChild(z->parent,b);
//            z->parent->right_child = b;
//            b->parent = z;
        }
    }
    
    if (b->left_child != x && b->left_child != y && b->left_child != z)
    {
        MakeRightChild(a, b->left_child);
//        a->right_child = b->left_child;
//        b->right_child->parent = a;
    }
    
    if (b->right_child != x && b->right_child != y && b->right_child != z)
    {
        MakeLeftChild(c,b->right_child);
    }
    
    MakeLeftChild(b,a);
    MakeRightChild(b,c);
    
    
    
    // Need to support free operations here because restructure may be called "n" times
    free(x);
    free(y);
    free(z);
    free(a);
    free(b);
    free(c);
    
    return NULL;
}

void MakeLeftChild (avlNode* a, avlNode* b)
{
    a->left_child = b;
    b->parent = a;
}

void MakeRightChild (avlNode* a, avlNode* b)
{
    a->right_child = b;
    b->parent = a;
}


