#include "minunit.h"
#include <lcthw/bstrlib.h>

char *test_bfromcstr() {
  bstring str = bfromcstr("1234");
  mu_assert(str != NULL, "string is null");
  mu_assert(blength(str) == 4, "string length is wrong");
  mu_assert(strcmp(bdata(str), "1234") == 0, "string data is wrong");

  return NULL;
}

char *test_blk2bstr() {
  bstring str = blk2bstr("12345", 4);
  mu_assert(str != NULL, "string is null");
  mu_assert(blength(str) == 4, "string length is wrong");
  mu_assert(strcmp(bdata(str), "1234") == 0, "string data is wrong");

  return NULL;
}

char *test_bstrcpy() {
  bstring str = bfromcstr("1234");
  bstring cpy = bstrcpy(str);
  mu_assert(cpy != NULL, "string is null");
  mu_assert(blength(cpy) == 4, "string length is wrong");
  mu_assert(strcmp(bdata(cpy), "1234") == 0, "string data is wrong");

  return NULL;
}

char *test_bassign() {
  bstring str = bfromcstr("");
  bstring str2 = bfromcstr("1234");

  int rc = bassign(str, str2);
  mu_assert(rc == BSTR_OK, "returned error code");

  mu_assert(str != NULL, "string is null");
  mu_assert(blength(str) == 4, "string length is wrong");
  mu_assert(strcmp(bdata(str), "1234") == 0, "string data is wrong");

  return NULL;
}

char *test_bsplit() {
  struct bstrList *split = bsplit(bfromcstr("1234_5678"), '_');
  mu_assert(split != NULL, "split list is null");
  mu_assert(split->qty == 2, "number of splits its wrong");
  mu_assert(biseq(split->entry[0], bfromcstr("1234")), "first split is wrong");
  mu_assert(biseq(split->entry[1], bfromcstr("5678")), "second split is wrong");
  bstrListDestroy(split);

  split = bsplit(bfromcstr("1234_5678"), '&');
  mu_assert(split != NULL, "split list is null");
  mu_assert(split->qty == 1, "number of splits its wrong");
  mu_assert(biseq(split->entry[0], bfromcstr("1234_5678")), "first split is wrong");
  bstrListDestroy(split);

  return NULL;
}

char *test_bformat() {
  bstring str = bformat("12%d%d", 3, 4);
  mu_assert(biseq(str, bfromcstr("1234")), "string data is wrong");

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_bfromcstr);
  mu_run_test(test_blk2bstr);
  mu_run_test(test_bstrcpy);
  mu_run_test(test_bassign);
  mu_run_test(test_bsplit);
  mu_run_test(test_bformat);

  return NULL;
}

RUN_TESTS(all_tests);