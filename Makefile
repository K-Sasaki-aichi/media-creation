CC = gcc
CFLAGS = -Wall -O2
LIBS = -lglut -lGLU -lGL -lm

SRCS = main.c models/*.h objects/*.c

TARGET = xwing

$(TARGET):
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)