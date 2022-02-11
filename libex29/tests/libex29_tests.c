#include "minunit.h"
#include "../src/dbg.h"
#include <dlfcn.h>

typedef int (*lib_function)(const char *data);
static char *lib_file = "./build/libex29.so";
static void *lib;

void *open_lib(char *lib_file) {
  lib = dlopen(lib_file, RTLD_NOW);
  check(lib != NULL, "Failed to open the library %s: %s", lib_file, dlerror());

  return lib;

  error:
    return NULL;
}

int run_func(void *lib, char *func_to_run, char *data) {
  check(lib != NULL, "Lib not opened");
  check(func_to_run != NULL, "Func name cant be null");

  lib_function func = dlsym(lib, func_to_run);
  check(func != NULL, "Did not find function %s in library %s: %s", func_to_run, lib_file, dlerror());
  return func(data);

  error:
    return 1;
}

char *test_dlopen() {
  lib = open_lib(lib_file);
  mu_assert(lib != NULL, "Failed to open the library");
  return NULL;
}

char *test_functions() {
  int rc = 0;

  rc = run_func(lib, "uppercase", "should be uppercase");
  mu_assert(rc == 0, "uppercase function failed");

  rc = run_func(lib, "lowercase", "SHOULD BE LOWERCASE");
  mu_assert(rc == 0, "lowercase function failed");

  return NULL;
}

char *test_failures() {
  int rc = run_func(lib, "fail_on_purpose", "this is an intentional failure");
  mu_assert(rc != 0, "fail_on_purpose function failed... to fail");
  return NULL;
}

char *test_dlclose() {
  mu_assert(dlclose(lib) == 0, "Failed to close lib");
  return NULL;
}

char *all_tests() {
  mu_suit_start();

  mu_run_test(test_dlopen);
  mu_run_test(test_functions);
  mu_run_test(test_failures);
  mu_run_test(test_dlclose);

  return NULL;
}

RUN_TESTS(all_tests);