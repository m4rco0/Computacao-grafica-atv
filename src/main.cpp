#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/esfera.h"
#include "../include/tardis.h"
#include "../include/iluminacao.h"

bool lerParametro(const char *texto, GLfloat &valor) {
  char *fim;
  valor = strtof(texto, &fim);
  return fim != texto && *fim == '\0' && isfinite(valor);
}

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




int main (int argc, char **argv) {

  if (argc != 5) {
    printf("Uso: %s <raio-planeta> <posicao-x-sol> <posicao-y-sol> <posicao-z-sol>\n", argv[0]);
    return EXIT_FAILURE;
  }

  GLfloat raio_planeta;
  GLfloat posX_sol;
  GLfloat posY_sol;
  GLfloat posZ_sol;
  if (!lerParametro(argv[1], raio_planeta) || raio_planeta <= 0.0f ||
      !lerParametro(argv[2], posX_sol) ||
      !lerParametro(argv[3], posY_sol) ||
      !lerParametro(argv[4], posZ_sol)) {
    fprintf(stderr, "Erro: informe um raio positivo e coordenadas numericas validas.\n");
    return EXIT_FAILURE;
  }

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
    teste_iluminacao(posX_sol, posY_sol, posZ_sol);
    desenhar(raio_planeta, posX_sol, posY_sol, posZ_sol);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
