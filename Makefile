CC := gcc
CFLAGS := -g -Wall -Wextra
CPPFLAGS := $(shell pkg-config --cflags pangocairo)
LDLIBS := -lwayland-client $(shell pkg-config --libs pangocairo)

HEADERS := src/wl.h \
		   src/wlr-layer-shell-protocol.h src/xdg-shell-protocol.h
OBJS := src/main.o src/wl.o src/draw.o \
		src/wlr-layer-shell-protocol.o src/xdg-shell-protocol.o

make: bar

bar: $(FILES) $(OBJS)
	$(CC) $^ $(LDLIBS) -o $@

src/wlr-layer-shell-protocol.h:
	wayland-scanner client-header \
		protocols/wlr-layer-shell-unstable-v1.xml $@
src/wlr-layer-shell-protocol.c:
	wayland-scanner private-code \
		protocols/wlr-layer-shell-unstable-v1.xml $@

src/xdg-shell-protocol.h:
	wayland-scanner client-header \
		protocols/xdg-shell.xml $@
src/xdg-shell-protocol.c:
	wayland-scanner private-code \
		protocols/xdg-shell.xml $@

%.o: %.c $(HEADERS)
	$(CC) -c $< $(CFLAGS) $(CPPFLAGS) -o $@

clean:
	rm -f bar src/*.o src/*-protocol.*
