LIB_NAME=libex29

CFLAGS=-g -O2 -Wall -Wextra -Isrc -DNDEBUG $(OPTFLAGS)
LIBS=-ldl $(OPTLIBS)
PREFIX?=/usr/local

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

TEST_SRC=$(wildcard tests/*_tests.c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))

TARGET="build/$(LIB_NAME).so"
# SO_TARGET=$(patsubst %.a,%.so,$(TARGET))

# The target build
all: $(TARGET) $(SO_TARGET) tests

dev: CFLAGS=-g -Wall -Wextra $(OPTFLAGS)
dev: all

$(TARGET): CFLAGS += -fPIC
$(TARGET): build $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS)

build:
	@mkdir -p build
	@mkdir -p bin

# The unit tests
.PHONY: tests
tests: LDLIBS += -ldl
tests: $(TESTS)
	sh ./tests/runtests.sh

# The cleaner
clean:
	rm -rf build $(OBJECTS) $(TESTS)
	rm -f tests/tests.log
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`

# The installer
install: all
	install -d $(DESTDIR)/$(PREFIX)/lib/
	install $(TARGET) $(DESTDIR)/$(PREFIX)/lib/

# The checker
check:
	@echo Files with potentially dangerous functions:
	@egrep '[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)' $(SOURCES) || true
