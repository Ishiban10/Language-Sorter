CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic 

all: banhammer

bvtest: bv.o bvtestmain.o
	$(CC) -o $@ $^

nodetest: node.o nodetestmain.o
	$(CC) -o $@ $^

lltest: node.o ll.o lltestmain.o
	$(CC) -o $@ $^

bftest: city.o bv.o bf.o bftestmain.o
	$(CC) -o $@ $^

httest: city.o node.o ll.o ht.o httestmain.o
	$(CC) -o $@ $^

ptest: city.o bv.o bf.o node.o ll.o ht.o parser.o parsertest.o
	$(CC) -o $@ $^

banhammer: city.o bv.o bf.o node.o ll.o ht.o parser.o banhammer.o
	$(CC) -o $@ $^ 

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f city ht ll node bf bv parser banhammer  *.o

spotless:
	rm -f city ht ll node bf bv parser banhammer bvtest bftest nodetest lltest httest ptest *.o

format:
	clang-format -i -style=file *.[ch]

