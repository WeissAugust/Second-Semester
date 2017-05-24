#include "btree.h"
#include <assert.h>

struct btree
{
	btree_node* root;
	int btree_max;
	int btree_min;
	int btree_size;
};

struct btree_node
{
	int node_value;
	btree_node* node_rightChild;
	btree_node* node_leftChild;
};

btree* btree_create()
{
	btree* newBtree = (btree*)malloc(sizeof(btree));
	newBtree->btree_size = 0;
	return newBtree;
}

void btree_destroyRec(btree_node* n)
{
	if (n != NULL)
	{
		btree_destroyRec(n->node_leftChild);
		btree_destroyRec(n->node_rightChild);
		free(n);
	}
}

void btree_destroy(btree* t)
{
	if (t != NULL)
	{
		if (t->root != NULL)
			btree_destroyRec(t->root);

		free(t);
	}
}

void subtree_insertRec(btree_node* n, int d)
{
	if (n->node_value > d)
	{
		if (n->node_leftChild == NULL)
		{
			btree_node* newNode = (btree_node*)malloc(sizeof(btree_node));
			newNode->node_value = d;
			newNode->node_leftChild = NULL;
			newNode->node_rightChild = NULL;
			n->node_leftChild = newNode;
		}
		else
		{
			subtree_insertRec(n->node_leftChild, d);
		}
	}
	else
	{
		if (n->node_rightChild == NULL)
		{
			btree_node* newNode = (btree_node*)malloc(sizeof(btree_node));
			newNode->node_value = d;
			newNode->node_leftChild = NULL;
			newNode->node_rightChild = NULL;
			n->node_rightChild = newNode;
		}
		else
		{
			subtree_insertRec(n->node_rightChild, d);
		}
	}
}

void btree_insert(btree* t, int d)
{
	if (t != NULL || !btree_contains(t, d))
	{
		if (t->btree_size == 0 || t->btree_max < d)
			t->btree_max = d;

		if (t->btree_size == 0 || t->btree_min > d)
			t->btree_min = d;

		if (t->btree_size == 0)
		{
			btree_node* root = (btree_node*)malloc(sizeof(btree_node));
			root->node_leftChild = NULL;
			root->node_rightChild = NULL;
			root->node_value = d;
			t->root = root;
		}
		else
		{
			subtree_insertRec(t->root, d);
		}

		t->btree_size += 1;
	}
}

void btree_removeRec(btree_node* n, int d)
{
	if (n == NULL)
	{
		return;
	}
	else if (n->node_value == d)
	{
		n->node_value = n->node_leftChild->node_value;
		n->node_leftChild = n->node_leftChild->node_leftChild;
		
	}
	else
	{
		if (n->node_value > d)
		{
			btree_removeRec(n->node_leftChild, d);
		}
		else
		{
			btree_removeRec(n->node_rightChild, d);
		}
	}
}

void btree_remove(btree* t, int d)
{
	if (t != NULL || btree_contains(t, d))
	{
		if (t->btree_size == 1)
		{
			free(t->root);
			t->root = NULL;
		}
		else
		{
			btree_removeRec(t->root, d);
		}

		t->btree_size -= 1;
	}
}

int btree_minimum(const btree* t)
{
	return t->btree_min;
}

int btree_maximum(const btree* t)
{
	return t->btree_max;
}

bool subtree_containsRec(btree_node* n, int d)
{
	if(n == NULL)
		return false;
		
	if (n->node_value == d)
		return true;

	if (n->node_leftChild != NULL || subtree_containsRec(n->node_leftChild, d))
		return true;
	if (n->node_rightChild != NULL || subtree_containsRec(n->node_rightChild, d))
		return true;

	return false;
}

bool btree_contains(const btree* t, int d)
{
	if(t == NULL || t->root == NULL)
		return false;
		
	if (t->root->node_value == d)
		return true;

	if (subtree_containsRec(t->root->node_leftChild, d))
		return true;
	if (subtree_containsRec(t->root->node_rightChild, d))
		return true;

	return false;
}


size_t btree_size(const btree* t)
{
	return t->btree_size;
}

void btree_printRec(const btree_node* n, FILE* out, int lvl)
{
	if (n == NULL)
		return;

	printf("(");
	btree_printRec(n->node_leftChild, out, lvl + 1);
	printf("%d", n->node_value);
	printf(")");
	btree_printRec(n->node_rightChild, out, lvl + 1);
}

void btree_print(const btree* t, FILE* out)
{
	if (t->btree_size == 0)
	{
		printf("( NIL ) : 0\n");
	}
	else
	{
		printf("(");
		btree_printRec(t->root->node_leftChild, NULL, 1);
		printf("%d ", t->root->node_value);
		btree_printRec(t->root->node_rightChild, NULL, 1);
		printf(")");

		printf(" : %ld\n", btree_size(t));
	}
}

int main(void)
{
	btree *ta = btree_create();

	assert(btree_size(ta) == 0);
	assert(btree_contains(ta, 4) == false);
	btree_print(ta, stdout);

	btree_insert(ta, 4);
	btree_insert(ta, 7);
	btree_insert(ta, 3);

	assert(btree_size(ta) == 3);
	assert(btree_contains(ta, 4) == true);
	assert(btree_minimum(ta) == 3);
	assert(btree_maximum(ta) == 7);
	btree_print(ta, stdout);

	btree_remove(ta, 4);

	assert(btree_size(ta) == 2);
	assert(btree_contains(ta, 4) == false);
	assert(btree_minimum(ta) == 3);
	assert(btree_maximum(ta) == 7);
	btree_print(ta, stdout);

	btree_insert(ta, 2);
	btree_insert(ta, -3);
	btree_insert(ta, 6);
	btree_insert(ta, 9);

	assert(btree_size(ta) == 6);
	btree_print(ta, stdout);

	btree_remove(ta, 6);

	assert(btree_size(ta) == 5);
	assert(btree_contains(ta, 6) == false);
	btree_print(ta, stdout);

	btree_insert(ta, 5);
	btree_insert(ta, 5);

	assert(btree_size(ta) == 6);
	assert(btree_contains(ta, 5) == true);
	btree_print(ta, stdout);

	btree_insert(ta, 6);
	btree_insert(ta, 4);
	btree_remove(ta, 7);
	btree_remove(ta, 7);

	assert(btree_size(ta) == 7);
	assert(btree_contains(ta, 7) == false);
	btree_print(ta, stdout);

	btree_destroy(ta);

	return EXIT_SUCCESS;
}


