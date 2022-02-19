#include <stdbool.h>
#include <lcthw/list_algos.h>
#include <lcthw/dbg.h>

// Bubble sort implementation

int List_bubble_sort(List *list, List_compare comparator) {
  // base cases - empty or list of one is effectively pre-sorted
  if (list->count <= 1) return 0;

  bool did_swap;

  do {
    did_swap = false;

    LIST_FOREACH(list, first->next, next, cur) {
      ListNode *prev = cur->prev;

      // if this pair is out of order
      if (comparator(prev->value, cur->value) > 0) {
        // swap them and remember something changed
        ListNode *cur_next = cur->next;
        ListNode *prev_prev = prev->prev;

        cur->prev = prev_prev;
        prev->next = cur_next;
        cur->next = prev;
        prev->prev = cur;

        did_swap = true;
      }
    }
  } while(did_swap);

  return 0;
}

// Merge sort implementation

List *merge(List *left, List *right, List_compare comparator) {
  List *result = List_create();

  while(left->count > 0 && right->count > 0) {
    if (comparator(left->first->value, right->first->value) < 0) {
      List_push(result, List_shift(left));
    } else {
      List_push(result, List_shift(right));
    }
  }

  // Either left or right may have elements left; consume them.
  // (Only one of the following loops will actually be entered.)
  while(left->count > 0) {
    List_push(result, List_shift(left));
  }
  while(right->count > 0) {
    List_push(result, List_shift(right));
  }

  return result;
}

List *List_merge_sort(List *list, List_compare comparator) {
  // Base case. A list of zero or one elements is sorted, by definition.
  if (list->count <= 1) return list;

  // Recursive case. First, divide the list into equal-sized sublists
  // consisting of the first half and second half of the list.
  // This assumes lists start at index 0.
  List *left = List_create();
  List *right = List_create();

  int i = 0;
  LIST_FOREACH(list, first, next, x) {
    if (i < (list->count)/2) {
      List_push(left, x->value);
    } else {
      List_push(right, x->value);
    }
    i++;
  }

  // Recursively sort both sublists.
  left = List_merge_sort(left, comparator);
  right = List_merge_sort(right, comparator);

  //TODO: this leaks quite a bit of memory

  // Then merge the now-sorted sublists.
  return merge(left, right, comparator);
}
