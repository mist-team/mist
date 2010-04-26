include ../build/makeinclude


ALL = \
	array_test \
	type_test \
	memory_test \
	hash_test \
	callback_test \
	pointer_test \
	limits_test \
	operators_test \
	binary_test \
	color_test \
	quaternion_test \
	iterator_test \
	thread_test \
	region_growing_test \
	machine_learning_test \
	wav_test \
	margin_array_test \
	median_test \
	morphology_test \
	fft_test \
	singleton_test \
	integer_test \
	decimal_test \
	vector_test \
	set_test \
	crc_test \
	random_test \
	minimization_test \
	mixture_test \
	environment_test \
	matrix_test \
	matrix_util_test \
	matrix_util_test2 \
	image_test \
	raw_test \
	volumerender_test \


all: $(ALL)

array_test : array_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

type_test : type_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

memory_test : memory_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

hash_test : hash_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

callback_test : callback_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

pointer_test : pointer_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

matrix_test : matrix_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LAPACKLIBS) $(LIBS) $(LDLIBS)

matrix_util_test : matrix_util_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LAPACKLIBS) $(LIBS) $(LDLIBS)

matrix_util_test2 : matrix_util_test2.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LAPACKLIBS) $(LIBS) $(LDLIBS)

vector_test : vector_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LAPACKLIBS) $(LIBS) $(LDLIBS)

region_growing_test : region_growing_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LAPACKLIBS) $(LIBS) $(LDLIBS)

limits_test : limits_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

operators_test : operators_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

binary_test : binary_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

color_test : color_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

quaternion_test : quaternion_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

iterator_test : iterator_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

thread_test : thread_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

image_test : image_test.obj image_test_window.obj
	$(CXX) $(CXXFLAGS) -o $@ $^ `fltk-config --use-gl --ldflags` $(LDLIBS) $(IMGLIBS) $(LIBS)

raw_test : raw_test.obj ct_image_window.obj
	$(CXX) $(CXXFLAGS) -o $@ $^ `fltk-config --use-gl --ldflags` $(LDLIBS) $(IMGLIBS) $(LIBS)

volumerender_test : volumerender_test.obj volr_image_window.obj
	$(CXX) $(CXXFLAGS) -o $@ $^ `fltk-config --use-gl --ldflags` $(LDLIBS) $(IMGLIBS) $(LIBS)

wav_test : wav_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

margin_array_test : margin_array_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

median_test : median_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS) $(IMGLIBS) $(LIBS)

morphology_test : morphology_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS) $(IMGLIBS) $(LIBS)

fft_test : fft_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

singleton_test : singleton_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

integer_test : integer_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

decimal_test : decimal_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

set_test : set_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

crc_test : crc_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

random_test : random_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

minimization_test : minimization_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

mixture_test : mixture_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

environment_test : environment_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

machine_learning_test : machine_learning_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)


clean:
	for binary in $(ALL); do\
		echo "=== deleting $$binary ===";\
		( cd ./; rm -f $$binary; rm -f $$binary.exe ) || break;\
	done
	rm -f ./*.o
	rm -f ./*.obj
	rm -f ./*~
