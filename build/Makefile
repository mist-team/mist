include ./makeinclude


DIRS = ../test

all: release

release: makeinclude
	for dir in $(DIRS); do\
		echo "=== making $$dir ===";\
		( cd $$dir; $(MAKE) $(MFLAGS) CXXFLAGS="$(RELEASEFLAGS)" ) || break;\
	done

debug: makeinclude
	for dir in $(DIRS); do\
		echo "=== making $$dir ===";\
		( cd $$dir; $(MAKE) $(MFLAGS) CXXFLAGS="$(DEBUGFLAGS)" ) || break;\
	done


clean:
	for dir in $(DIRS); do\
		echo "=== cleaning $$dir ===";\
		( cd $$dir; $(MAKE) $(MFLAGS) clean ) || break;\
	done