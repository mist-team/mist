include ../build/makeinclude


OTHERINCLUDE = -I/data6/CTProg/ddeguchi/systems/i386-linux/include/fox-1.3
FOXLIBS      = /data6/CTProg/ddeguchi/systems/i386-linux/lib/libFOX-1.3.a -lX11 -lXext -lGL

TARGET = mist_builder

CPPFILES = \
	config.cpp \
	data_view.cpp \
	enum_filters.cpp \
	filter_graph.cpp \
	main.cpp \
	main_window.cpp \
	mist_builder.cpp \
	property_list.cpp \


OBJECTS = $(CPPFILES:.cpp=.o)


all: $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS) $(FOXLIBS) $(IMGLIBS)


clean:
	rm -f ./$(TARGET)
	rm -f ./*.o
	rm -f ./*.obj
	rm -f ./*~
