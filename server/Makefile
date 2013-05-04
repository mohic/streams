# Maxime LECAT (mlecat)
# Christophe MOHIMONT (cmohimo)
#
# compile le serveur

# particularite
.PHONY: clean

# retirer les regles implicites
.SUFFIXES:

# compilateur
CC      ?= gcc

# options
CFLAGS  ?=

# nom du prog
PROG  = server.out

# fichiers requis
OBJS  = main.o serversck.o game.o ../common/semaphore.o

# compilation
all: $(OBJS)
		$(CC) $(CFLAGS) $^ -o $(PROG)

%.o: %.c
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -rf *.o
