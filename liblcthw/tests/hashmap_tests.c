#include "minunit.h"
#include <lcthw/hashmap.h>
#include <assert.h>
#include <lcthw/bstrlib.h>

Hashmap *map = NULL;
static int traverse_called = 0;
struct tagbstring test1 = bsStatic("test data 1");
struct tagbstring test2 = bsStatic("test data 2");
struct tagbstring test3 = bsStatic("test data 3");
struct tagbstring expect1 = bsStatic("THE VALUE 1");
struct tagbstring expect2 = bsStatic("THE VALUE 2");
struct tagbstring expect3 = bsStatic("THE VALUE 3");

static int traverse_good_cb(HashmapNode *node) {
	debug("KEY: %s", bdata((bstring)node->key));
	traverse_called++;
	return 0;
}

static int traverse_fail_cb(HashmapNode *node) {
	debug("KEY: %s", bdata((bstring)node->key));
	traverse_called++;

	if (traverse_called == 2) {
		return 1;
	} else {
		return 0;
	}
}

char *test_create(void) {
	map = Hashmap_create(NULL, NULL);
	mu_assert(map != NULL, "Failed to create map.");
	return NULL;
}

char *test_create_buckets() {
	size_t num_buckets = 5;
	Hashmap *m = Hashmap_create_buckets(NULL, NULL, num_buckets);
	mu_assert(m != NULL, "Failed to create map.");
	mu_assert(m->num_buckets == num_buckets, "number of buckets wrong");

	return NULL;
}

char *test_destroy(void) {
	Hashmap_destroy(map);
	return NULL;
}

char *test_get_set(void) {
	int rc = Hashmap_set(map, &test1, &expect1);
	mu_assert(rc == 0, "Failed to set &test1.");
	bstring result = Hashmap_get(map, &test1);
	mu_assert(result == &expect1, "Wrong value for test1.");

	rc = Hashmap_set(map, &test2, &expect2);
	mu_assert(rc == 0, "Failed to set &test2.");
	result = Hashmap_get(map, &test2);
	mu_assert(result == &expect2, "Wrong value for test2.");

	rc = Hashmap_set(map, &test3, &expect3);
	mu_assert(rc == 0, "Failed to set &test3.");
	result = Hashmap_get(map, &test3);
	mu_assert(result == &expect3, "Wrong value for test3.");

	return NULL;
}

char *test_count(void) {
	int count = Hashmap_count(map);
	mu_assert(count == 3, "expected count of 3");

	return NULL;
}

char *test_traverse(void) {
	int rc = Hashmap_traverse(map, traverse_good_cb);
	mu_assert(rc == 0, "Failed to traverse for good.");
	mu_assert(traverse_called == 3, "Wrong count traverse for good.");

	traverse_called = 0;
	rc = Hashmap_traverse(map, traverse_fail_cb);
	mu_assert(rc == 1, "Failed to traverse for fail.");
	mu_assert(traverse_called == 2, "Wrong count traverse for fail.");

	return NULL;
}

char *test_delete(void) {
	bstring deleted = Hashmap_delete(map, &test1);
	mu_assert(deleted != NULL, "Got NULL on delete.");
	mu_assert(deleted == &expect1, "Should get expect1.");
	bstring result = Hashmap_get(map, &test1);
	mu_assert(result == NULL, "Should deleted.");

	deleted = Hashmap_delete(map, &test2);
	mu_assert(deleted != NULL, "Got NULL on delete.");
	mu_assert(deleted == &expect2, "Should get expect2.");
	result = Hashmap_get(map, &test2);
	mu_assert(result == NULL, "Should deleted.");

	deleted = Hashmap_delete(map, &test3);
	mu_assert(deleted != NULL, "Got NULL on delete.");
	mu_assert(deleted == &expect3, "Should get expect3.");
	result = Hashmap_get(map, &test3);
	mu_assert(result == NULL, "Should deleted.");

	int count = Hashmap_count(map);
	mu_assert(count == 0, "expected count of 0");

	return NULL;
}


char *all_tests(void) {
	mu_suite_start();

	mu_run_test(test_create);
	mu_run_test(test_create_buckets);
	mu_run_test(test_get_set);
	mu_run_test(test_count);
	mu_run_test(test_traverse);
	mu_run_test(test_delete);
	mu_run_test(test_destroy);

	return NULL;
}

RUN_TESTS(all_tests);