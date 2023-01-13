#include <cairo/cairo-gl.h>
#include <EGL/egl.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>

EGLDisplay egl_display;
EGLSurface egl_surface;
EGLContext egl_context;

cairo_surface_t *cairo_surface;
cairo_device_t *cairo_device;

// function prototypes
static void on_realize(GtkWidget *widget);
static gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data);
// end of function prototypes

static void on_realize(GtkWidget *widget) {
  EGLConfig egl_config;
  EGLint n_config;
  EGLint attributes[] = {EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT, EGL_NONE};

  egl_display =
      eglGetDisplay((EGLNativeDisplayType)gdk_x11_display_get_xdisplay(
          gtk_widget_get_display(widget)));
  eglInitialize(egl_display, NULL, NULL);
  eglChooseConfig(egl_display, attributes, &egl_config, 1, &n_config);
  eglBindAPI(EGL_OPENGL_API);
  egl_surface = eglCreateWindowSurface(
      egl_display, egl_config,
      gdk_x11_window_get_xid(gtk_widget_get_window(widget)), NULL);
  egl_context = eglCreateContext(egl_display, egl_config, EGL_NO_CONTEXT, NULL);

  cairo_device = cairo_egl_device_create(egl_display, egl_context);
  cairo_surface =
      cairo_gl_surface_create_for_egl(cairo_device, egl_surface, 480, 360);
}

static gboolean on_draw(GtkWidget *widget, cairo_t *_cr, gpointer user_data) {
  // eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context);

  // glViewport(0, 0, gtk_widget_get_allocated_width(widget),
  //            gtk_widget_get_allocated_height(widget));

  cairo_t *cr = cairo_create(cairo_surface);
  cairo_set_source_rgb(cr, 1.0, 1.0, 0.0);
  cairo_paint(cr);

  cairo_set_source_rgb(cr, 0.0, 1.0, 0.0);
  cairo_select_font_face(cr, "cairo:serif", CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size(cr, 20);
  cairo_move_to(cr, 10.0, 50.0);
  cairo_show_text(cr, "hello");

  cairo_surface_flush(cairo_surface);
  cairo_destroy(cr);

  cairo_gl_surface_swapbuffers(cairo_surface);

  return TRUE;
}

// Main method.
int main(int argc, char *argv[]) {
  // Always have this to start GTK.
  gtk_init(&argc, &argv);

  // Set new window, set new drawing area.
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  GtkWidget *darea = gtk_drawing_area_new();

  // Add the drawing area to the window.
  gtk_container_add(GTK_CONTAINER(window), darea);

  // You need this to register mouse clicks.
  gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);

  g_signal_connect(G_OBJECT(darea), "realize", G_CALLBACK(on_realize), NULL);
  g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw), NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 480, 360);
  gtk_window_set_title(GTK_WINDOW(window), "EGL TestBed");

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}