CC = gcc
CFLAGS = -O2
LIBS = -lglut -lGLU -lGL -lm

SRCS = main.c models/*.c objects/*.c texture/*.c

TARGET = xwing

$(TARGET):
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)