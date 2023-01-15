build: libmymalloc.so
	
libmymalloc.so: mymalloc.o
	gcc -shared mymalloc.o -o libmymalloc.so

mymalloc.o: mymalloc.c mymalloc.h
	gcc -fPIC -c mymalloc.c -o mymalloc.o

.PHONY: clean
clean:
	rm *.o
	rm *.so