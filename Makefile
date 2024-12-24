CC := gcc
CFLAGS := -g -Wall -Wextra
CPPFLAGS := $(shell pkg-config --cflags pangocairo)
LDLIBS := -lwayland-client $(shell pkg-config --libs pangocairo)

HEADERS := src/wlr-layer-shell-protocol.h src/xdg-shell-protocol.h
OBJS := src/main.o src/wl.o src/utils.o src/widgets.o \
		src/wlr-layer-shell-protocol.o \
		src/xdg-shell-protocol.o

make: dtbar

dtbar: $(OBJS)
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
	rm -f dtbar src/*.o src/*-protocol.*

install: dtbar
	mkdir -p $(PKG)/usr/bin
	cp $< $(PKG)/usr/bin/$<
