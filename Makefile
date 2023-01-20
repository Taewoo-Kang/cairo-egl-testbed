CC = g++
CXXFLAGS = -Wall -g `pkg-config --cflags gtk+-3.0`
LDFLAGS = `pkg-config --libs gtk+-3.0` -lGL

TARGET = test_gl

SRC_DIR = ./src
OBJ_DIR = ./obj

SRCS = $(notdir $(wildcard $(SRC_DIR)/*.cc))
OBJS = $(patsubst %.o,$(OBJ_DIR)/%.o,$(SRCS:.cc=.o))
DEPS = $(OBJS:.o=.d)

dir_guard = $(shell [ ! -d $(@D) ] && mkdir -p $(@D))

all: $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
		$(dir_guard)
		$(CC) $(CXXFLAGS) -c $< -o $@ -MD $(LDFLAGS)

$(TARGET): $(OBJS)
		$(CC) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

.PHONY: clean all
clean:
		rm -f $(OBJS) $(DEPS) $(TARGET)

-include $(DEPS)