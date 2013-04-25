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
		make -C server
