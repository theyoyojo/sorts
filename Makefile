CC = gcc
CFLAGS = -g -Wall --std=c99
EXEC = sorts
OBJECTS = sorts.o

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^

clean:
	rm $(EXEC) $(OBJECTS)
