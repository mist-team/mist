include ../build/makeinclude


ALL = \
	matrix_test \
	limits_test \
	operators_test \
	boolean_test \


all: $(ALL)

matrix_test : matrix_test.o
	$(CXX) $(CXXFLAGS) -o $@ matrix_test.o $(LIBS) $(LDLIBS)

limits_test : limits_test.o
	$(CXX) $(CXXFLAGS) -o $@ limits_test.o $(LIBS) $(LDLIBS)

operators_test : operators_test.o
	$(CXX) $(CXXFLAGS) -o $@ operators_test.o $(LIBS) $(LDLIBS)

boolean_test : boolean_test.o
	$(CXX) $(CXXFLAGS) -o $@ boolean_test.o $(LIBS) $(LDLIBS)

clean:
	for binary in $(ALL); do\
		echo "=== deleting $$binary ===";\
		( cd ./; rm -f $$binary ) || break;\
	done
	rm -f ./*.o
	rm -f ./*~
