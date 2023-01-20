#include <GLES3/gl3.h>
#include <gtk/gtk.h>

#include "program.h"
#include "rendering.h"

int WIDTH = 803;
int HEIGHT = 800;

static void on_resize (GtkGLArea *area, gint width, gint height) {
  float wd = (float)width / WIDTH;
  float ht = (float)height / HEIGHT;
  
  initBuffers(wd, ht);
}

// inside this function it's safe to use GL; the given
// GdkGLContext has been made current to the drawable
// surface used by the `GtkGLArea` and the viewport has
// already been set to be the size of the allocation
static gboolean on_render(GtkGLArea *area, GdkGLContext *context) {
  draw();

  return TRUE;
}


static void on_realize(GtkGLArea *gl_area) {
  gtk_gl_area_make_current(gl_area);

  if (gtk_gl_area_get_error(gl_area) != NULL) {
    return;
  }

  // Print version info:
  const GLubyte *renderer = glGetString(GL_RENDERER);
  const GLubyte *version = glGetString(GL_VERSION);
  printf("Renderer: %s\n", renderer);
  printf("OpenGL version supported %s\n", version);

  setupGraphics(WIDTH, HEIGHT);
  generateTexture();
}

// Main method.
int main(int argc, char *argv[]) {
  gtk_init(&argc, &argv);

  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  GtkWidget *gl_area = gtk_gl_area_new();

  // Add the drawing area to the window.
  gtk_container_add(GTK_CONTAINER(window), gl_area);

  // You need this to register mouse clicks.
  gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);

  g_signal_connect(gl_area, "realize", G_CALLBACK(on_realize), NULL);
  g_signal_connect(gl_area, "render", G_CALLBACK(on_render), NULL);
  g_signal_connect(gl_area, "resize", G_CALLBACK(on_resize), NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT);
  gtk_window_set_title(GTK_WINDOW(window), "TestBed");

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}