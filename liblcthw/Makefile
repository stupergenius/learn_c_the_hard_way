CFLAGS=-g -O2 -Wall -Wextra -Isrc -rdynamic -DNDEBUG $(OPTFLAGS)
LIBS=-ldl $(OPTLIBS)
PREFIX?=/usr/local

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))
HEADERS=$(wildcard src/**/*.h src/*.h)

TEST_SRC=$(wildcard tests/*_tests.c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))

PROGRAMS_SRC=$(wildcard bin/*.c)
PROGRAMS=$(patsubst %.c,%,$(PROGRAMS_SRC))

TARGET=build/liblcthw.a
HEADER_TARGET=build/lcthw.h
SO_TARGET=$(patsubst %.a,%.so,$(TARGET))

# The target build
all: $(TARGET) $(SO_TARGET) tests $(PROGRAMS)

dev: CFLAGS=-g -Wall -Wextra $(OPTFLAGS)
dev: all

$(TARGET): CFLAGS += -fPIC
$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@
$(SO_TARGET): $(TARGET) $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS)

build:
	@mkdir -p build
	@mkdir -p bin

$(PROGRAMS): LDLIBS += $(TARGET)

# Currently hand-crafted. At some point should consider autoconf/automake
$(HEADER_TARGET): $(HEADERS)
	@rm -f $(HEADER_TARGET)
	@for f in "bstrlib" "list" "list_algos" "darray" "hashmap" "heap" "union_find"; do cat "src/lcthw/$${f}.h" >> build/tmp_header.h && echo >> build/tmp_header.h; done && \
		grep -v "^#include <lcthw/" build/tmp_header.h > $(HEADER_TARGET) && \
		rm -f build/tmp_header.h

# The unit tests
.PHONY: tests
tests: LDLIBS += $(TARGET)
tests: $(TESTS)
	sh ./tests/runtests.sh

# The cleaner
clean:
	rm -rf build $(OBJECTS) $(TESTS) $(PROGRAMS)
	rm -f tests/tests.log
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`

# The installer
install: all $(HEADER_TARGET)
	install -d $(DESTDIR)/$(PREFIX)/lib/
	install -d $(DESTDIR)/$(PREFIX)/include/
	install $(TARGET) $(DESTDIR)/$(PREFIX)/lib/
	install $(HEADER_TARGET) $(DESTDIR)/$(PREFIX)/include/

# The checker
check:
	@echo Files with potentially dangerous functions:
	@egrep '[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)' $(SOURCES) || true
