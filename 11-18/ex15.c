#include <stdio.h>

int main(int argc, char *argv[]) {
  // create two arrays we care about
  int ages[] = {23, 43, 12, 98, 2};
  char *names[] = {
    "Alan", "Frank", "Mary", "John", "Lisa"
  };
  // safely get the size of ages
  int count = sizeof(ages) / sizeof(int);
  int i = 0;

  // first way using indexing
  for (i = 0; i < count; i++) {
    printf("%s has %d years alive.\n", names[i], ages[i]);
  }

  puts("====");

  // second way using pointers
  int *cur_age = ages;
  char **cur_name = names;
  for(i = 0; i < count; i++) {
    printf("%s is %d years old.\n", *(cur_name+i), *(cur_age+i));
  }

  puts("====");

  // third way, pointers are just arrays
  for(i = 0; i < count; i++) {
    printf("%s is %d years old again at %p.\n", *(&cur_name[i]), cur_age[i], &cur_name[i]);
  }

  puts("====");

  // fourth way with pointers in a stupid complex way
  // for (cur_name = names+count, cur_age = ages+count; (cur_age - count) > 0; cur_name--, cur_age--) {
  for (cur_name = names, cur_age = ages; (cur_age - ages) < count; cur_name++, cur_age++) {
    printf("%s lived %d years so far.\n", *cur_name, *cur_age);
  }

  return 0;
}