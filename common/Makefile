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
PROG  = server

# fichiers requis
OBJS  = semaphore.o

# compilation
all: $(OBJS)
		#$(CC) $(CFLAGS) $^ -o $(PROG)

%.o: %.c
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -rf *.o
