LDFLAGS  = glad/src/glad.o -Llib32 -lglfw -lGL -ldl
CXXFLAGS = -g -Wall -Wno-write-strings -Wno-parentheses -DLINUX

all:    demo1 demo2 demo3 demo4 demo5 demo6 demo7 demo8 demo9

demo1:  demo1.o glad/src/glad.o 
	$(CXX) $(CXXFLAGS) -o demo1 demo1.o $(LDFLAGS) 

demo2:  demo2.o gpuProgram.o glad/src/glad.o 
	$(CXX) $(CXXFLAGS) -o demo2 demo2.o gpuProgram.o $(LDFLAGS) 

demo3:  demo3.o linalg.o gpuProgram.o glad/src/glad.o 
	$(CXX) $(CXXFLAGS) -o demo3 demo3.o gpuProgram.o linalg.o $(LDFLAGS) 

demo4:  demo4.o linalg.o gpuProgram.o glad/src/glad.o 
	$(CXX) $(CXXFLAGS) -o demo4 demo4.o gpuProgram.o linalg.o $(LDFLAGS) 

demo5:  demo5.o linalg.o gpuProgram.o glad/src/glad.o 
	$(CXX) $(CXXFLAGS) -o demo5 demo5.o gpuProgram.o linalg.o $(LDFLAGS) 

demo6:  demo6.o linalg.o gpuProgram.o glad/src/glad.o 
	$(CXX) $(CXXFLAGS) -o demo6 demo6.o gpuProgram.o linalg.o $(LDFLAGS) 

demo7:  demo7.o linalg.o gpuProgram.o glad/src/glad.o 
	$(CXX) $(CXXFLAGS) -o demo7 demo7.o gpuProgram.o linalg.o $(LDFLAGS) 

demo8:  demo8.o linalg.o gpuProgram.o glad/src/glad.o 
	$(CXX) $(CXXFLAGS) -o demo8 demo8.o gpuProgram.o linalg.o $(LDFLAGS) 

demo9:  demo9.o linalg.o gpuProgram.o glad/src/glad.o 
	$(CXX) $(CXXFLAGS) -o demo9 demo9.o gpuProgram.o linalg.o $(LDFLAGS) 

clean:
	rm -f *.o *~ demo1 demo2 demo3 demo4 demo5 demo6 demo7 demo8 demo9
