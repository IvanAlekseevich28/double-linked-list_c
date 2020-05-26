#ifndef LIST_H_
#define LIST_H_
#include <stdio.h>

typedef struct node {
	void* data;

	struct node* next;
	struct node* prev;
} node;


node*	node_create (void* data, unsigned dataSize);
void	node_delete (node* n);
void*	node_delNSD (node* n);	// Delete node, but save data

node*	node_getNext( node* n);
node*	node_getPrev( node* n);
void*	node_getData( node* n);

node* 	node_addNext(node* n, void* data, unsigned dataSize);
node*	node_addPrev(node* n, void* data, unsigned dataSize);

void* 	node_delNext(node* n);
void*	node_delPrev(node* n);



typedef struct {
	node* first;
	node* last;

	unsigned dataSize;
} list;


list* 		list_create(unsigned dataSize);
void 		list_free(list* l);


void 		list_pushBack	(list* l,   void* data);
void 		list_pushFront	(list* l,   void* data);

void* 		list_popBack	(list* l);
void*		list_popFront	(list* l);

void*		list_erasePos	(list* l, int   pos);
void		list_deletePos	(list* l, int   pos);

unsigned	list_size( list* l);
void*		list_get( list* l, int pos);

int 		list_write	(list* l, FILE* f, void  (*dataWriter)(void *data, FILE* f), char* tag);
int 		list_read	(list* l, FILE* f, void* (*dataReader)(FILE *f), char *tag);

list*		list_search	( list* l, void* controlData , int (*comp)(void *, void *), void* (*copy)(void*));

int			list_isEmpty( list* l);

void        list_delNullNodes (list* l);

int         list_check  (const list* l, void* controlData , int (*comp)(void *, void *));
#endif /* LIST_H_ */
