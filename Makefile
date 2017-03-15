.PHONY: clean all
cc = gcc
CFLAGS = -Wall -g
BIN = main
OBJS1 = main.o condtion.o threadpool.o
all: $(BIN)
%.o:%.c
	$(cc) $(CFLAGS) -c $< -o $@
main:$(OBJS1)
	$(cc) $(CFLAGS)  $^ -o $@ -lpthread -lrt
clean:
	rm -f *.o $(BIN)
