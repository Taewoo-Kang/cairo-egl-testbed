CFLAGS = -Wall -g `pkg-config --cflags gtk+-3.0 gdk-x11-3.0`
LDFLAGS = `pkg-config --libs gtk+-3.0` -lEGL -lGL

all: \
        example

example: main.cc
	$(CC) -o EGL_TestBed main.cc $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *~
	rm -f *.o
	rm -f EGL_TestBed