CC=gcc
CFLAGS=-I.
DEPS = ass2.h
OBJ = main.o ass2.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

scheduler: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
	rm -f scheduler