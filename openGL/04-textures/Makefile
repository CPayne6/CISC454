LDFLAGS  = glad/src/glad.o -Llib32 -lglfw -lGL -ldl -lfreetype
CXXFLAGS = -g -I/usr/include/freetype2 -Wall -Wno-write-strings -Wno-parentheses -DLINUX

PROG = shader

OBJS = shader.o gpuProgram.o linalg.o wavefront.o axes.o font.o glerror.o

hier:  $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(PROG) $(OBJS) $(LDFLAGS) 

clean:
	rm -f *.o *~ $(PROG)

