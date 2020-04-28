#include "list.h"
#include <malloc.h>

node* node_create(void* data, unsigned dataSize) {
	node* n = malloc(dataSize + sizeof(node*) * 2);
	n->data = data;
	n->next = 0;
	n->prev = 0;

	return n;
}

void node_delete(node* n) {
	if (!n) {
		return;
	}

    free(node_delNSD(n));
}

void* node_delNSD(node* n) {
	if (n->next && n->prev) {
        n->next->prev = n->prev;
        n->prev->next = n->next;
	} else if (n->next) {
		n->next->prev = 0;
	} else if (n->prev) {
		n->prev->next = 0;
	}

	node* d = n->data;
	free(n);

	return (d);
}

node* node_getNext( node* n) {
	return n->next;
}

node* node_getPrev( node* n) {
	return n->prev;
}

void* node_getData( node* n) {
	return n->data;
}

node* node_addNext(node* n, void* data, unsigned dataSize) {
	node* new = node_create(data, dataSize);

    if (!n) {
        new->next = 0;
        new->prev = 0;
    } else if (!n->next) {
		n->next = new;
		new->prev = n;
	} else {
		n->next->prev = new;
		new->next = n->next;

		n->next = new;
		new->prev = n;
	}

	return new;
}
node* node_addPrev(node* n, void* data, unsigned dataSize) {
	node* new = node_create(data, dataSize);

    if (!n) {
        new->next = 0;
        new->prev = 0;
    } else if (!n->prev) {
		n->prev = new;
		new->next = n;
	} else {
		n->prev->next = new;
		new->prev = n->prev;

		n->prev = new;
		new->next = n;
	}

	return new;
}

void* node_delNext(node* n) {
	return node_delNSD(n->next);
}

void* node_delPrev(node* n) {
	return node_delNSD(n->prev);
}


list* list_create(unsigned dataSize) {
	list* l = malloc(sizeof(node*) * 2 + dataSize);

	l->first = 0;
	l->last = 0;

	return l;
}

void list_free(list* l) {
	node* iterNode = l->first;
	while (iterNode) {
		node* fNode = iterNode;
		iterNode = node_getNext(iterNode);

		node_delete(fNode);
	}

    free(l);
}

void list_pushBack(list* l,  void* data) {
	if (!l->last) {
		l->first = l->last = node_create(data, l->dataSize);
        return;
	}
	l->last = node_addNext(l->last, data, l->dataSize);
}

void list_pushFront(list* l,  void* data) {
	if (!l->first) {
		l->first = l->last = node_create(data, l->dataSize);
        return;
	}
    l->first = node_addPrev(l->first, data, l->dataSize);
}

void* list_popBack(list* l) {
	if (!l->last) {
		return 0;
	}

	if (!l->last->prev) {
        node* delNode = l->last;
        l->last = l->first = 0;
        return node_delNSD(delNode);
	}
	l->last = l->last->prev;
	return node_delNSD(l->last->next);
}

void* list_popFront(list* l) {
	if (!l->first) {
		return 0;
	}

	if (!l->first->next) {
        node* delNode = l->first;
        l->last = l->first = 0;
        return node_delNSD(delNode);
	}
	l->first = l->first->next;
	return node_delNSD(l->first->prev);
}

void* list_erasePos(list* l, int pos) {
	int i = 0;
    node* iter = (pos >= 0 ? l->first : l->last);

    while (iter != 0) {
		if (i == pos) {
            if (iter == l->first) {
                l->first = l->first->next;
            }
            if (iter == l->last) {
                l->last = l->last->prev;
            }
            return node_delNSD(iter);
		}
        i += (pos >= 0 ? 1 : -1);
        iter = (pos >= 0 ? node_getNext(iter) : node_getPrev(iter));
	}

	return 0;
}

void list_deletePos(list* l, int pos) {
	node* n = list_erasePos(l, pos);
	if (n)
		free(n);
}

unsigned list_size( list* l) {
	unsigned C = 0;
	node* iter = l->first;
	while (iter) {
		iter = node_getNext(iter);
		C++;
	}

    return C;
}

void* list_get( list* l, int pos) {
	int i = 0;
    node* iter = (pos >= 0 ? l->first : l->last);

    while (iter != 0) {
		if (i == pos) {
			return iter->data;
		}
        i += (pos >= 0 ? 1 : -1);
        iter = (pos >= 0 ? node_getNext(iter) : node_getPrev(iter));
	}

	return 0;
}

int list_write(list* l, FILE* f, void (*dataWriter)(void *data, FILE* f)) {
	if (!f) {
		return -1;
	}
	node* iter = l->first;
	while (iter) {
		dataWriter(iter->data, f);
		iter = node_getNext(iter);
	}

	return 0;
}

int list_read(list* l, FILE* f, void* (*dataReader)(FILE *f)) {
	if (!f) {
		return -1;
	}
    while (!feof(f)) {
		list_pushBack(l, dataReader(f));
	}

	return 0;
}

list* list_search( list* l, void* controlData, int (*comp)(void *, void *), void* (*copy)(void*)) {
	list* ansList = list_create(l->dataSize);
	node* iter = l->first;
	while (iter) {
		if (comp(controlData, iter->data)) {
			list_pushBack(ansList, copy(iter->data));
		}
		iter = node_getNext(iter);
	}


	return ansList;
}

int			list_isEmpty( list* l) {
    return l->first ? 0 : 1;
}

void        list_delNullNodes (list* l) {
    node* iterNode = l->first;
    while (iterNode) {
        node* fNode = iterNode;
        iterNode = node_getNext(iterNode);

    if (!fNode->data)
        if (fNode == l->first) {
            l->first = l->first->next;
        }
        if (fNode == l->last) {
            l->last = l->last->prev;
        }
        node_delete(fNode);
    }
}
