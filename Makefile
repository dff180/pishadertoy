CC=g++
CFLAGS=-c -g -Wall -O3  -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux
LDFLAGS=-L/opt/vc/lib -lGLESv2 -lEGL -lopenmaxil -lbcm_host -lSOIL
SOURCES=EGLConfig.cpp EGLWindow.cpp main.cpp MyGLWindow.cpp shader_utils.cpp
OBJECTS=$(SOURCES:%.cpp=%.o)
EXECUTABLE=pishadertoy

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean :
	rm -f *.o $(EXECUTABLE)
