#include "ex22.h"
#include "../dbg.h"

const char *MY_NAME = "Ben S";

void scope_demo(int count) {
  log_info("count is: %d", count);

  if (count > 0) {
    int count = 100; // bugs!
    log_info("count in this scope is: %d", count);
  }

  log_info("count is at exit: %d", count);

  count = 3000;

  log_info("count after assign: %d", count);
}

int main(int argc, char *argv[]) {
  log_info("my name: %s, age: %d", MY_NAME, get_age());

  set_age(100);

  log_info("my age is now: %d", get_age());

  log_info("THE_SIZE is: %d", THE_SIZE);
  print_size();

  THE_SIZE = 9;
  log_info("THE_SIZE is now: %d", THE_SIZE);
  print_size();

  log_info("Ratio at first: %.2f", update_ratio(2.0));
  log_info("Ratio again: %.2f", update_ratio(10.0));
  log_info("Ratio once more: %.2f", update_ratio(300.0));

  int count = 4;
  scope_demo(count);
  scope_demo(count * 20);

  log_info("count after calling scope_demo: %d", count);
}