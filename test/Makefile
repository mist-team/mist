include ../build/makeinclude


ALL = \
	matrix_test \


all: $(ALL)
	 
matrix_test : matrix_test.o
	$(CXX) $(CXXFLAGS) -o $@ matrix_test.o $(LIBS) $(LDLIBS)

clean:
	for binary in $(ALL); do\
		echo "=== deleting $$binary ===";\
		( cd ./; rm -f $$binary ) || break;\
	done
	rm -f ./*.o
	rm -f ./*~
