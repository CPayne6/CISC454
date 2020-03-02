LDFLAGS  = glad/src/glad.o -Llib32 -lglfw -lGL -ldl
CXXFLAGS = -g -Wall -Wno-write-strings -Wno-parentheses -DLINUX

PROG = hier

OBJS = hier.o gpuProgram.o linalg.o axes.o object.o cylinder.o rectangle.o

hier:  $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(PROG) $(OBJS) $(LDFLAGS) 

clean:
	rm -f *.o *~ $(PROG)

depend:	
	makedepend -Y *.cpp *.C *.h

# DO NOT DELETE

axes.o: headers.h glad/include/glad/glad.h glad/include/KHR/khrplatform.h
axes.o: include/GLFW/glfw3.h linalg.h axes.h gpuProgram.h
cylinder.o: cylinder.h geometry.h headers.h glad/include/glad/glad.h
cylinder.o: glad/include/KHR/khrplatform.h include/GLFW/glfw3.h linalg.h
gpuProgram.o: gpuProgram.h headers.h glad/include/glad/glad.h
gpuProgram.o: glad/include/KHR/khrplatform.h include/GLFW/glfw3.h linalg.h
hier.o: headers.h glad/include/glad/glad.h glad/include/KHR/khrplatform.h
hier.o: include/GLFW/glfw3.h linalg.h gpuProgram.h link.h object.h geometry.h
hier.o: cylinder.h rectangle.h axes.h
linalg.o: linalg.h
object.o: object.h linalg.h geometry.h headers.h glad/include/glad/glad.h
object.o: glad/include/KHR/khrplatform.h include/GLFW/glfw3.h
rectangle.o: rectangle.h geometry.h headers.h glad/include/glad/glad.h
rectangle.o: glad/include/KHR/khrplatform.h include/GLFW/glfw3.h linalg.h
sphere.o: sphere.h linalg.h seq.h headers.h glad/include/glad/glad.h
sphere.o: glad/include/KHR/khrplatform.h include/GLFW/glfw3.h gpuProgram.h
axes.o: linalg.h gpuProgram.h headers.h glad/include/glad/glad.h
axes.o: glad/include/KHR/khrplatform.h include/GLFW/glfw3.h
cylinder.o: geometry.h headers.h glad/include/glad/glad.h
cylinder.o: glad/include/KHR/khrplatform.h include/GLFW/glfw3.h linalg.h
geometry.o: headers.h glad/include/glad/glad.h glad/include/KHR/khrplatform.h
geometry.o: include/GLFW/glfw3.h linalg.h
gpuProgram.o: headers.h glad/include/glad/glad.h
gpuProgram.o: glad/include/KHR/khrplatform.h include/GLFW/glfw3.h linalg.h
headers.o: glad/include/glad/glad.h glad/include/KHR/khrplatform.h
headers.o: include/GLFW/glfw3.h linalg.h
link.o: linalg.h object.h geometry.h headers.h glad/include/glad/glad.h
link.o: glad/include/KHR/khrplatform.h include/GLFW/glfw3.h cylinder.h
link.o: rectangle.h
object.o: linalg.h geometry.h headers.h glad/include/glad/glad.h
object.o: glad/include/KHR/khrplatform.h include/GLFW/glfw3.h
rectangle.o: geometry.h headers.h glad/include/glad/glad.h
rectangle.o: glad/include/KHR/khrplatform.h include/GLFW/glfw3.h linalg.h
seq.o: headers.h glad/include/glad/glad.h glad/include/KHR/khrplatform.h
seq.o: include/GLFW/glfw3.h linalg.h
sphere.o: linalg.h seq.h headers.h glad/include/glad/glad.h
sphere.o: glad/include/KHR/khrplatform.h include/GLFW/glfw3.h gpuProgram.h
