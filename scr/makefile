BIN = ../bin

CC = g++
CFLAGS = -I./

MCMC-OBSLoc: *.o
	$(CC) $^ -o $@ -lm
	
%.o: *.cpp
	$(CC) -c $^ $(CFLAGS)

install:
	cp MCMC-OBSLoc $(BIN)

clean:
	rm -f *.o MCMC-OBSLoc
