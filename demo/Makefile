include ../build/makeinclude

CFILES = \
	demo.cpp \

OBJECTS = $(CFILES:.cpp=.o)


all: demo

demo : $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(IMGLIBS) $(LDLIBS)


clean:
	rm -f ./demo
	rm -f ./*.o
	rm -f ./*.obj
	rm -f ./*~
