#include "list.h"
#include "dbg.h"

// List memory management and lifecycle

List *List_create() {
  return calloc(1, sizeof(List));
}

void List_destroy(List *list) {
  // forward iterate (passing next) starting at the beginning (passing first)
  // access cur for the current ListNode element
  LIST_FOREACH(list, first, next, cur) {
    // free each node looking backwards (so we can iterate forwards)
    if (cur->prev) {
      free(cur->prev);
    }

    // free the last one (which the loop wont have free'd)
    free(list->last);

    // free the list itself
    free(list);
  }
}

void List_clear(List *list) {
  LIST_FOREACH(list, first, next, cur) {
    free(cur->value);
    cur->value = NULL;
  }
  list->first = NULL;
  list->last = NULL;
  list->count = 0;
}

void List_clear_destroy(List *list) {
  List_clear(list);
  List_destroy(list);
}

// List push/pop operations

void List_push(List *list, void *value) {
  // new list node
  ListNode *new_node = calloc(1, sizeof(ListNode));
  check_mem(new_node);

  new_node->value = value;

  if (list->count == 0 || list->last == NULL) {
    // base case - list is effectively empty
    list->first = new_node;
    list->last = new_node;
  } else {
    // otherwise point the "old" last elemenet's next node to the new node
    // and point the new node's prev node to the new node
    ListNode *old_last = list->last;
    old_last->next = new_node;
    new_node->prev = old_last;

    // finally update the list itself with the new last node
    list->last = new_node;
  }

  list->count += 1;

  error:
    return;
}

void *List_pop(List *list) {
  ListNode *last = list->last;

  if (last == NULL) {
    return NULL;
  }

  // save off the last node's value to return
  void *last_value = last->value;

  if (list->first == list->last) {
    // a list of one, effectively
    // after popping it becomes empty
    list->first = NULL;
    list->last = NULL;
  } else {
    // list connection updates
    list->last = last->prev;
    if (list->last != NULL) {
      list->last->next = NULL;
    }
  }

  // update the list count
  list->count -= 1;
  // free the node we just popped
  free(last);

  return last_value;
}

// List shift/unshift operations

void List_unshift(List *list, void *value) {
  // new list node
  ListNode *new_node = calloc(1, sizeof(ListNode));
  check_mem(new_node);

  new_node->value = value;

  if (list->count == 0 || list->first == NULL) {
    // base case - list is empty
    list->first = new_node;
    list->last = new_node;
  } else {
    ListNode *old_first = list->first;
    new_node->next = old_first;
    old_first->prev = new_node;
    list->first = new_node;
  }

  list->count += 1;

  error:
    return;
}

void *List_shift(List *list) {
  ListNode *old_first = list->first;

  if (old_first == NULL) {
    return NULL;
  }

  void *old_value = old_first->value;

  if (list->first == list->last) {
    // a list of one, effectively
    // after shifting it becomes empty
    list->first = NULL;
    list->last = NULL;
  } else {
    // list connection updates
    list->first = old_first->next;
    if (list->first) {
      list->first->prev = NULL;
    }
  }

  list->count -= 1;
  free(old_first);

  return old_value;
}

// List remove

void *List_remove(List *list, ListNode *node) {
  void *result = NULL;

  check(list->first && list->last, "List is empty"); // falls through to return NULL
  check(node, "node can't be NULL");

  if (list->first == node && list->last == node) {
    // node is the only element in the list
    list->first = NULL;
    list->last = NULL;
  } else if (list->first == node) {
    // node is first element in the list (effectively a shift operation)
    list->first = node->next;
    check(list->first != NULL, "Invalidated list assumptions - we've previously checked that there's more than one element in this list so there should be a node following the given node.");
    list->first->prev = NULL;
  } else if (list->last == node) {
    // node is last element in the list (effectively a pop operation)
    list->last = node->prev;
    check(list->last, "Invalid list - got a next that is null");
    list->last->next = NULL;
  } else {
    // node is in the middle
    ListNode *prev = node->prev;
    ListNode *next = node->next;
    // now join them, severing the given node
    prev->next = next;
    next->prev = prev;
  }

  result = node->value;
  list->count -= 1;
  free(node);

  // no previous return falls through the error block to return the result value
  error:
    return result;
}
