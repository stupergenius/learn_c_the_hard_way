#ifndef _lcthw_List_h
#define _lcthw_List_h

#include <stdlib.h>

struct ListNode;
typedef struct ListNode {
  struct ListNode *next;
  struct ListNode *prev;
  void *value;
} ListNode;

typedef struct List {
  int count;
  ListNode *first;
  ListNode *last;
} List;

// make an empty list
List *List_create();
// destroy the list itself along with each node in the list
void List_destroy(List *list);
// free and clear each node value in the list
void List_clear(List *list);
// destroy the list itself, each node in the list, and each value in each node
void List_clear_destroy(List *list);

#define List_count(A) ((A)->count)
#define List_first(A) ((A)->first != NULL ? (A)->first->value : NULL)
#define List_last(A) ((A)->last != NULL ? (A)->last->value : NULL)

// push a new value onto the end of the list
void List_push(List *list, void *value);
// remove and return the end of the list
void *List_pop(List *list);

// add a value at the front of the list
void List_unshift(List *list, void *value);
// remove and return the value from the front of the list
void *List_shift(List *list);

// remove a specific node from the list
void *List_remove(List *list, ListNode *node);

/*
 * A FOREACH macro accepting, in order:
 * L: The list to iterate
 * S: Which node to start on: first, last, or an arbitrary traversal (first->next, etc.)
 * M: Which "direction" to iterate in: next or prev
 * V: A pointer to the current iteration value
 */
#define LIST_FOREACH(L, S, M, V) ListNode *_node = NULL; ListNode *V = NULL; \
  for(V = _node = L->S; _node != NULL; V = _node = _node->M)

#endif