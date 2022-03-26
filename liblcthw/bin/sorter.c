#include <stdlib.h>
#include <stdio.h>
#include <lcthw/bstrlib.h>
#include <lcthw/list.h>
#include <lcthw/list_algos.h>

#undef NDEBUG

int bstr_cmpare(const void *a, const void *b) {
  return bstrcmp(a, b);
}

int main(int argc, char const *argv[]) {
  if (argc <= 1) return 0;

  List *input = List_create();
  for(int i = 1; i < argc; i++) {
    bstring str = bfromcstr(argv[i]);
    List_push(input, str);
  }

  List *sorted = List_merge_sort(input, bstr_cmpare);

  printf("Sorted input is: ");
  LIST_FOREACH(sorted, first, next, n) {
    printf("%s ", bstr2cstr(n->value, '\0'));
  }
  printf("\n");

  return 0;
}
