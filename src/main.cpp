#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "esfera.h"
#include "tardis.h"
void redimensionar(GLFWwindow *window, int w, int h) {
  if (h == 0) h = 1;
  glViewport(0, 0, w, h);

  float aspect = (float)w / (float)h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if (w > h)
    glOrtho(-10.0 * aspect, 10.0 * aspect, -10.0, 10.0, -10.0, 10.0);
  else
    glOrtho(-10.0, 10.0, -10.0 / aspect, 10.0 / aspect, -10.0, 10.0);

  glMatrixMode(GL_MODELVIEW);
}



void desenhar() {

  desenharSol(1.0f);
  girarSol(10.0f); // Rotaciona o sol
  desenharTerra(0.5f);
  girarTerra(10.0f); // Rotaciona a terra
}

int main (void) {
  const int LARGURA = 800;
  const int ALTURRA = 600;

  if(!glfwInit())
    return -1;

  glfwWindowHint(GLFW_DEPTH_BITS, 24);

  GLFWwindow * window = glfwCreateWindow(LARGURA, ALTURRA, "Desenhando esfera", NULL, NULL);

  if (!window) {
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glEnable(GL_DEPTH_TEST);

  glfwSetFramebufferSizeCallback(window, redimensionar);
  
  int largura_atual, altura_atual;
  glfwGetFramebufferSize(window, &largura_atual, &altura_atual);
  redimensionar(window, largura_atual, altura_atual);

  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    desenhar();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  return 0;
}
