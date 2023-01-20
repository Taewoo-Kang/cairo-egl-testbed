#ifndef PROGRAM_H
#define PROGRAM_H

#include <GLES3/gl3.h>

void useProgram();
GLuint loadShader(GLenum shaderType, const char *pSource);
GLuint createProgram(
    const char *pVertexSource,
    const char *pFragmentSource);
bool setupGraphics(int w, int h);

#endif  // PROGRAM_H