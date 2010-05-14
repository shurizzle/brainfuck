CC     ?=   gcc
CFLAGS  =   -Wall -Wextra -ansi -pedantic -pedantic-errors
INCS    =   -I/usr/include -Iincludes
DESTDIR?=   /usr/local/bin
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
	@echo "Installing binary"
	@cp "bin/$(TARGET)" "$(DESTDIR)"

uninstall:
	@echo "Uninstalling binary"
	@rm "$(DESTDIR)/$(TARGET)"

.PHONY: all clean install uninstall