include ../build/makeinclude

CFILES = \
	demo.cpp \

OBJECTS = $(CFILES:.c=.o)


all: demo

demo : $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)


clean:
	rm -f ./demo
	rm -f ./*.o
	rm -f ./*.obj
	rm -f ./*~
