PROG = texture

OBJS = main.o persp.o arcballWindow.o window.o shader.o vector.o font.o torus.o texture.o

INCS = 
LIBS = -lGLEW -lglut -lGLU -lGL -lIL

CXXFLAGS = -O2 -Wno-write-strings -Wno-deprecated -DLINUX $(INCS)
CXX      = g++

$(PROG):	$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(PROG) $(OBJS) $(LIBS)

.C.o:
	$(CXX) $(CXXFLAGS) -c $<

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<


clean:
	rm -f *.o *~ core $(PROG) Makefile.bak

depend:	
	makedepend -Y *.h *.cpp
# DO NOT DELETE

arcballWindow.o: headers.h vector.h window.h
font.o: headers.h
light.o: vector.h
persp.o: headers.h arcballWindow.h vector.h window.h light.h shader.h
persp.o: texture.h seq.h
seq.o: headers.h
shader.o: headers.h
texture.o: headers.h seq.h vector.h
window.o: headers.h vector.h
arcballWindow.o: headers.h arcballWindow.h vector.h window.h font.h
font.o: headers.h
main.o: headers.h persp.h arcballWindow.h vector.h window.h light.h shader.h
main.o: texture.h seq.h
persp.o: headers.h persp.h arcballWindow.h vector.h window.h light.h shader.h
persp.o: texture.h seq.h torus.h
shader.o: shader.h headers.h
texture.o: headers.h texture.h seq.h vector.h
torus.o: headers.h vector.h
vector.o: headers.h vector.h
window.o: headers.h window.h vector.h
