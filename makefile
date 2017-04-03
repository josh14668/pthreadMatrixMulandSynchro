# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

# the build target executable:
TARGET1 = matrixMul
TARGET2 = synchro

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(TARGET1).c
	$(CC) $(CFLAGS) -o $(TARGET1) $(TARGET1).c

	$(TARGET2): $(TARGET2).c
		$(CC) $(CFLAGS) -o $(TARGET2) $(TARGET2).c

clean:
	$(RM) $(TARGET1)	$(TARGET2)
