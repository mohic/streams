# Maxime LECAT (mlecat)
# Christophe MOHIMONT (cmohimo)
#
# compile tout le projet

# particularite
.PHONY: clean

# retirer les regles implicites
.SUFFIXES:

# compilation
all:
		make -C client
		make -C common
		make -C server

clean:
		make -C client clean
		make -C common clean
		make -C server clean
