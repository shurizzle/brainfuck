# Makefile for ``brainfuck''

VERSION =   0.1

CC     ?=   gcc
CFLAGS  =   -Wall -Wextra -ansi -pedantic -pedantic-errors -D_BSD_SOURCE
INCS    =   -I/usr/include -Iincludes
PREFIX ?=		/usr/local
SOURCES =   sources/main.c sources/storage.c
OBJECTS =   build/main.o build/storage.o
TARGET  =   brainfuck

all: init $(TARGET)

init:
	@mkdir -p build
	@mkdir -p bin

$(TARGET): $(OBJECTS)
	@echo "CC -o $@"
	@$(CC) $? -o bin/$@

$(OBJECTS): build/%.o: sources/%.c
	@echo "CC    $<"
	@$(CC) -c $(CFLAGS) $(INCS) $< -o $@

clean: $(OBJECTS) bin/$(TARGET)
	@echo "Cleaning..."
	@rm -R build bin

install: all
	@mkdir -p "$(DESTDIR)/$(PREFIX)/bin"
	@echo "Installing binary"
	@cp "bin/$(TARGET)" "$(DESTDIR)/$(PREFIX)/bin"

uninstall:
	@echo "Uninstalling binary"
	@rm "$(DESTDIR)/$(PREFIX)/bin/$(TARGET)"

dist:
	@echo "Creating tarball..."
	@mkdir -p "$(TARGET)-$(VERSION)"
	@cp -R includes sources Makefile README "$(TARGET)-$(VERSION)"
	@tar -cf "$(TARGET)-$(VERSION).tar" "$(TARGET)-$(VERSION)"
	@gzip "$(TARGET)-$(VERSION).tar"
	@rm -R "$(TARGET)-$(VERSION)"

.PHONY: all clean install uninstall
