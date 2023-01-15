build: libmyfirstfit.so
	
libmyfirstfit.so: myfirstfit.o
	gcc -shared myfirstfit.o -o libmyfirstfit.so

myfirstfit.o: myfirstfit.c myfirstfit.h
	gcc -fPIC -c myfirstfit.c -o myfirstfit.o

.PHONY: clean
clean:
	rm *.o
	rm *.so