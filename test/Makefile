include ../build/makeinclude


ALL = \
	matrix_test \
	limits_test \
	operators_test \
	binary_test \
	color_test \
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

binary_test : binary_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

color_test : color_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

iterator_test : iterator_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

thread_test : thread_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

image_test : image_test.obj image_test_window.obj
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS) `fltk-config --use-gl --ldflags` $(IMGLIBS)



clean:
	for binary in $(ALL); do\
		echo "=== deleting $$binary ===";\
		( cd ./; rm -f $$binary; rm -f $$binary.exe ) || break;\
	done
	rm -f ./*.o
	rm -f ./*.obj
	rm -f ./*~
