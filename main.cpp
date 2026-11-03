#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "esfera.h"

void redimensionar(int w, int h) {
  glViewport(0, 0, w, h);

  float aspect = (float)w / (float)h;
  glMatrixMode(GL_PROJECTION);

  glLoadIdentity();
  glOrtho(-5.0 * aspect, 5.0 * aspect, -5.0, 5.0, -5.0, 5.0);
}

#include <GL/gl.h>


int main (void) {
  const int LARGURA = 800;
  const int ALTURRA = 800;

  glfwInit();

  GLFWwindow * window = glfwCreateWindow(LARGURA, ALTURRA, "Desenhando esfera", NULL, NULL);

  glfwMakeContextCurrent(window);
  glEnable(GL_DEPTH_TEST);
  glfwWindowHint(GLFW_DEPTH_BITS, 24);
  redimensionar(LARGURA, ALTURRA);
  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    girarEsfera(10);
    
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  return 0;
}
