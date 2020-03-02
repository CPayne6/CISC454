LDFLAGS  = glad/src/glad.o -Llib32 -lglfw -lGL -ldl
CXXFLAGS = -g -Wall -Wno-write-strings -Wno-parentheses -DLINUX

PROG = demoSphere

OBJS = demoSphere.o gpuProgram.o linalg.o sphere.o axes.o

$(PROG): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(PROG) $(OBJS) $(LDFLAGS) 

clean:
	rm -f *.o *~ $(PROG)
