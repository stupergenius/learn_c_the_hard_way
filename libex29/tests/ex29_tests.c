#include <stdio.h>
#include <assert.h>
#include <dlfcn.h>
#include "../src/dbg.h"

typedef int (*lib_function) (const char *data);

void test_uppercase(lib_function func) {
  int rc = func("hello world");
  assert(rc == 0);
}

void test_lowercase(lib_function func) {
  int rc = func("HELLO WORLD");
  assert(rc == 0);
}

void test_fail_on_purpose(lib_function func) {
  int failed = func("dummy");
  assert(failed == 1);
}

int main(int argc, char *argv[]) {
  int rc = 0;

  char *lib_file = "./build/libex29.so";
  char *func_to_run;
  lib_function func;

  void *lib = dlopen(lib_file, RTLD_NOW);
  check(lib != NULL, "Failed to open the library %s: %s", lib_file, dlerror());

  func_to_run = "uppercase";
  func = dlsym(lib, func_to_run);
  check(func != NULL, "Did not find function %s in library %s: %s", func_to_run, lib_file, dlerror());
  test_uppercase(func);

  func_to_run = "lowercase";
  func = dlsym(lib, func_to_run);
  check(func != NULL, "Did not find function %s in library %s: %s", func_to_run, lib_file, dlerror());
  test_lowercase(func);

  func_to_run = "fail_on_purpose";
  func = dlsym(lib, func_to_run);
  check(func != NULL, "Did not find function %s in library %s: %s", func_to_run, lib_file, dlerror());
  test_fail_on_purpose(func);

  rc = dlclose(lib);
  check(rc == 0, "Failed to close %s", lib_file);

  return 0;

  error:
    return 1;
}
