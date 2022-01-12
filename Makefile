OUTPUT := tint2-wsdisplay
CFLAGS := -O3 -std=c99 -D_GNU_SOURCE -march=native -mtune=native
LDFLAGS := $(shell pkg-config --cflags --libs x11)

all: $(OUTPUT)
run: $(OUTPUT)
	./$(OUTPUT)

$(OUTPUT): main.c Makefile config.h
	gcc main.c $(CFLAGS) $(LDFLAGS) -o $(OUTPUT)

clean:
	rm $(OUTPUT)
