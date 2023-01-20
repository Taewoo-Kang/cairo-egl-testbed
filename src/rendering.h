#ifndef RENDERING_H
#define RENDERING_H

struct vertex {
	float x;
	float y;
	float u;
	float v;
} __attribute__((packed));

void draw();
void initBuffers(float wd, float ht);
void generateTexture();

#endif  // RENDERING_H