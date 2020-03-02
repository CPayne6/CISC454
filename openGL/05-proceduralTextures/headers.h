// The standard headers included by all files


#ifndef HEADERS_H
#define HEADERS_H


#ifdef _WIN32
  #include <windows.h>
  #define M_PI 3.14159
  #define MAXFLOAT 9.9e9
  #define rint(x) (floor((x)+0.5))
#endif

#ifdef LINUX
  #include <sys/timeb.h>	// includes ftime (to return current time)
  #include <unistd.h>		// includes usleep (to sleep for some time)
  #include <values.h>
  #ifndef MAXFLOAT
    #define MAXFLOAT 9.9e99
  #endif
#endif

#ifdef __APPLE_CC__
  #include <OpenGL/glew.h>
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  #include <GLUT/glut.h>
#else
  #include <GL/glew.h>
  #include <GL/glut.h>
  #include <GL/glu.h>
  #include <GL/gl.h>
#endif

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

#include <math.h>

#define randIn01() (rand() / (float) RAND_MAX)   // random number in [0,1]


#endif
