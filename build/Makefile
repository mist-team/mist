include ./makeinclude


DIRS = ../test

all: makeinclude
	for dir in $(DIRS); do\
		echo "=== making $$dir ===";\
		( cd $$dir; $(MAKE) $(MFLAGS) ) || break;\
	done

clean:
	for dir in $(DIRS); do\
		echo "=== cleaning $$dir ===";\
		( cd $$dir; $(MAKE) $(MFLAGS) clean ) || break;\
	done