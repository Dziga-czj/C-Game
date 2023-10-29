PROJECT=$(shell basename $(CURDIR))
ifeq (, $(shell which clang))
CC=gcc
else
CC=clang
endif
SOURCES=$(shell ls c_files/*.c)
OBJECTS=$(subst .c,.o,$(SOURCES))
HEADERS=$(shell ls headers/*.h)
INCLUDES=-I/home/$(USER)/raylib/src/
LIBS_EXTRA=-L/home/$(USER)/raylib/src/
LIBS=-lraylib -lGL -lm -lpthread -ldl -lrt $(LIBS_EXTRA)

%.o: %.c $(HEADERS)
	$(CC) $(INCLUDES) -O3 -c $< -o $@

$(PROJECT): $(OBJECTS)
	$(CC) $(LIBS) -o $(PROJECT) $(OBJECTS)

$(PROJECT).zip: $(SOURCES) $(HEADERS) Makefile
	zip $(PROJECT).zip $(SOURCES) $(HEADERS) Makefile

all: $(PROJECT)
package: $(PROJECT).zip
clean:
	rm -f $(PROJECT).zip $(OBJECTS)

