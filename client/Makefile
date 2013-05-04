# Maxime LECAT (mlecat)
# Christophe MOHIMONT (cmohimo)
#
# compile le client

# particularite
.PHONY: clean

# retirer les regles implicites
.SUFFIXES:

# compilateur
CC      ?= gcc

# options
CFLAGS  ?=

# nom du prog
PROG  = client

# fichiers requis
OBJS  = main.o clientsck.o ../common/semaphore.o game.o

# compilation
all: $(OBJS)
		$(CC) $(CFLAGS) $^ -o $(PROG)

%.o: %.c
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -rf *.o
