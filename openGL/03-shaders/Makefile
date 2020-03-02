LDFLAGS  = glad/src/glad.o -Llib32 -lglfw -lGL -ldl
CXXFLAGS = -g -Wall -Wno-write-strings -Wno-parentheses -DLINUX

PROG = shader

OBJS = shader.o gpuProgram.o linalg.o wavefront.o axes.o

hier:  $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(PROG) $(OBJS) $(LDFLAGS) 

clean:
	rm -f *.o *~ $(PROG)

