include ./makeinclude


DIRS = ../test ../demo

all: release

release: makeinclude
	for dir in $(DIRS); do\
		echo "=== making $$dir ===";\
		( cd $$dir; $(MAKE) $(MFLAGS) CXXFLAGS="$(RELEASEFLAGS)" ) || break;\
	done

release_atlas: makeinclude
	for dir in $(DIRS); do\
		echo "=== making $$dir ===";\
		( cd $$dir; $(MAKE) $(MFLAGS) CXXFLAGS="$(RELEASEFLAGS)" LAPACKLIBS="$(ATLASLIBS)" ) || break;\
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
