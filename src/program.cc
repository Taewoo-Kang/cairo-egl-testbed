#include "program.h"

#include <GLES3/gl3.h>
#include <stdlib.h>

auto gVertexShader =
"#version 150\n"

"in vec2 vertex;\n"
"in vec2 texture;\n"

"out vec2 ftex;\n"

"void main (void)\n"
"{\n"
"	ftex = texture;\n"
"	gl_Position = vec4(vertex, 0.5, 1.0);\n"
"}";

auto gFragmentShader =
"#version 150\n"

"uniform sampler2D tex;\n"
"in vec2 ftex;\n"

"out vec4 fragcolor;\n"

"void main (void)\n"
"{\n"
"	fragcolor = texture(tex, ftex);\n"
"}";

GLuint gProgram;

void useProgram() {
  glUseProgram(gProgram);
  GLint mlocation = glGetUniformLocation(gProgram, "tex");
  glUniform1i(mlocation, 0);
}

GLuint loadShader(GLenum shaderType, const char *pSource) {
  GLuint shader = glCreateShader(shaderType);
  if (shader) {
    glShaderSource(shader, 1, &pSource, NULL);
    glCompileShader(shader);
    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
      GLint infoLen = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
      if (infoLen) {
        char *buf = (char *)malloc(infoLen);
        if (buf) {
          glGetShaderInfoLog(shader, infoLen, NULL, buf);
          free(buf);
        }
        glDeleteShader(shader);
        shader = 0;
      }
    }
  }
  return shader;
}

GLuint createProgram(
    const char *pVertexSource,
    const char *pFragmentSource) {  // Load and Bind Fragments to Progam and
                                    // link program then return result
  GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
  if (!vertexShader) {
    return 0;
  }

  GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
  if (!pixelShader) {
    return 0;
  }

  GLuint program = glCreateProgram();
  if (program) {
    glAttachShader(program, vertexShader);
    glAttachShader(program, pixelShader);
    glLinkProgram(program);
    GLint linkStatus = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE) {
      GLint bufLength = 0;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
      if (bufLength) {
        char *buf = (char *)malloc(bufLength);
        if (buf) {
          glGetProgramInfoLog(program, bufLength, NULL, buf);
          free(buf);
        }
      }
      glDeleteProgram(program);
      program = 0;
    }
    glDetachShader(program, vertexShader);
	  glDetachShader(program, pixelShader);

	  glDeleteShader(vertexShader);
	  glDeleteShader(pixelShader);
  }
  return program;
}

bool setupGraphics(int w, int h) {
  gProgram = createProgram(gVertexShader, gFragmentShader);
  if (!gProgram) {
    return false;
  }
  // gvPositionHandle = glGetAttribLocation(gProgram, "position");

  // glViewport(0, 0, w, h);
  return true;
}