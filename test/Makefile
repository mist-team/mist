include ../build/makeinclude


ALL = \
	matrix_test \
	limits_test \
	operators_test \
	boolean_test \
	iterator_test \
	thread_test \
	image_test \


all: $(ALL)

matrix_test : matrix_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

limits_test : limits_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

operators_test : operators_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

boolean_test : boolean_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

iterator_test : iterator_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

thread_test : thread_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

image_test : image_test.cpp image_test_window.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(INCLUDE) $(LIBS) $(LDLIBS) `fltk-config --use-gl --cxxflags --ldflags` $(IMGLIBS)



clean:
	for binary in $(ALL); do\
		echo "=== deleting $$binary ===";\
		( cd ./; rm -f $$binary; rm -f $$binary.exe ) || break;\
	done
	rm -f ./*.o
	rm -f ./*~
