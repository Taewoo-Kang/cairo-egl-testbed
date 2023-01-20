#include "rendering.h"

#include <GLES3/gl3.h>
#include <gdk/gdk.h>

#include "program.h"

GLuint mTexture, vao, vbo;  // Create reference Id for the texture
GLuint gvPositionHandle;

static GLubyte indices[6] = {
    0, 2, 3,
    2, 0, 1,
};

void draw() {
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  useProgram();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, mTexture);
  // initBuffers();
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
  glBindVertexArray(0);
}

void initBuffers(float wd, float ht) {
  // GLuint VBOs[2], EBO;  // Initialize an buffer to store all the verticles and
  //                       // transfer them to the GPU
  // glGenVertexArrays(1, &VAO);  // Generate VAO
  // glGenBuffers(1, VBOs);       // Generate VBO
  // glGenBuffers(1, &EBO);       // Generate EBO
  // glBindVertexArray(VAO);      // Bind the Vertex Array

  // glBindBuffer(GL_ARRAY_BUFFER,
  //              VBOs[0]);  // Bind verticles array for OpenGL to use
  // glBufferData(GL_ARRAY_BUFFER, sizeof(recVertices), recVertices,
  //              GL_STATIC_DRAW);

  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
  //              EBO);  // Bind the indices for information about drawing sequence
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
  //              GL_STATIC_DRAW);

  // // 1. set the vertex attributes pointers
  // // Position Attribute
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
  //                       (GLvoid *)0);
  // glEnableVertexAttribArray(0);
  // // Color Attribute
  // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
  //                       (GLvoid *)(3 * sizeof(GLfloat)));
  // glEnableVertexAttribArray(1);
  // // Texture Coordinate Attribute
  // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
  //                       (GLvoid *)(6 * sizeof(GLfloat)));
  // glEnableVertexAttribArray(2);

  // glBindVertexArray(0);  // 3. Unbind VAO

	struct vertex vertex[4] = {
		{ -1, -1,  0,  0 },	// Bottom left
		{  1, -1, wd,  0 },	// Bottom right
		{  1,  1, wd, ht },	// Top right
		{ -1,  1,  0, ht },	// Top left
	};

  glBindVertexArray(vao);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
		sizeof(struct vertex),
		(void *) offsetof(struct vertex, x));

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
		sizeof(struct vertex),
		(void *) offsetof(struct vertex, u));

	glBindVertexArray(0);
}

void generateTexture() {
  // Load the image
  GError *error = nullptr;
  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("../capture.png", &error);
	pixbuf = gdk_pixbuf_flip(pixbuf, FALSE);

  glGenTextures(1, &mTexture);
  glBindTexture(GL_TEXTURE_2D, mTexture);  // Bind our 2D texture so that
                                           // following set up will be applied
  // Generate the image
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gdk_pixbuf_get_width(pixbuf),
               gdk_pixbuf_get_height(pixbuf), 0, GL_RGBA, GL_UNSIGNED_BYTE,
               gdk_pixbuf_get_pixels(pixbuf));

  // Set texture wrapping parameter
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // Set texture Filtering parameter
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // glGenerateMipmap(GL_TEXTURE_2D);

  // stbi_image_free(image);// Free the reference to the image
  // glBindTexture(GL_TEXTURE_2D, 0);  // Unbind 2D textures

  // Generate empty buffer:
	glGenBuffers(1, &vbo);

	// Generate empty vertex array object:
	glGenVertexArrays(1, &vao);
}
