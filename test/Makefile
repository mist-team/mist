include ../build/makeinclude


ALL = \
	matrix_test \
	matrix_util_test \
	limits_test \
	operators_test \
	binary_test \
	color_test \
	iterator_test \
	thread_test \
	image_test \
	raw_test \
	wav_test \
	margin_array_test\
	median_test\
	morphology_test\


all: $(ALL)

matrix_test : matrix_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

matrix_util_test : matrix_util_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ -llapack -lblas -lF77 -lI77 $(LIBS) $(LDLIBS)

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

raw_test : raw_test.obj ct_image_window.obj
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS) `fltk-config --use-gl --ldflags` $(IMGLIBS)

wav_test : wav_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

margin_array_test : margin_array_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

median_test : median_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

morphology_test : morphology_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)


clean:
	for binary in $(ALL); do\
		echo "=== deleting $$binary ===";\
		( cd ./; rm -f $$binary; rm -f $$binary.exe ) || break;\
	done
	rm -f ./*.o
	rm -f ./*.obj
	rm -f ./*~
