#include "../include/esfera.h"
#include "../include/iluminacao.h"
#include "../include/primitivas.h"
#include "../include/tardis.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Registra o input do programa para renderizar o raio do planeta e a posição do
// sol
bool lerParametro(const char *texto, GLfloat &valor) {
  char *fim;
  valor = strtof(texto, &fim);
  return fim != texto && *fim == '\0' && isfinite(valor);
}

// Variável global para controlar o modo de renderização (malha ou preenchido)
void redimensionar(GLFWwindow *, int w, int h) {
  if (h == 0)
    h = 1;
  glViewport(0, 0, w, h);

  float aspect = (float)w / (float)h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if (w > h)
    glOrtho(-limite * aspect, limite * aspect, -limite, limite, -50.0, 50.0);
  else
    glOrtho(-limite, limite, -limite / aspect, limite / aspect, -50.0, 50.0);

  glMatrixMode(GL_MODELVIEW);
}

void GerenciarTeclado(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }

  // Variável que guarda o estado anterior da tecla R
  static bool teclaR_pressionada_anteriormente = false;

  bool teclaR_atual = (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS);

  if (teclaR_atual && !teclaR_pressionada_anteriormente) {
    renderiza_malha = !renderiza_malha;
  }

  teclaR_pressionada_anteriormente = teclaR_atual;
}

int main(int argc, char **argv) {
  // parsing dos argumentos de entrada
  //  <programa> <raio-planeta> <posicao-x-sol> <posicao-y-sol> <posicao-z-sol>
  if (argc != 5) {
    printf("Uso: %s <raio-planeta> <posicao-x-sol> <posicao-y-sol> "
           "<posicao-z-sol>\n",
           argv[0]);
    return EXIT_FAILURE;
  }

  GLfloat raio_planeta, posX_sol, posY_sol, posZ_sol;
  if (!lerParametro(argv[1], raio_planeta) || raio_planeta <= 0.0f ||
      !lerParametro(argv[2], posX_sol) || !lerParametro(argv[3], posY_sol) ||
      !lerParametro(argv[4], posZ_sol)) {
    fprintf(
        stderr,
        "Erro: informe um raio positivo e coordenadas numericas validas.\n");
    return EXIT_FAILURE;
  }

  // configuração da janela e do contexto OpenGL
  const int LARGURA = 800;
  const int ALTURRA = 600;
  if (!glfwInit())
    return -1;

  GLFWwindow *window =
      glfwCreateWindow(LARGURA, ALTURRA, "Desenhando esfera", NULL, NULL);

  if (!window) {
    glfwTerminate();
    return -1;
  }
  // limpando o buffer
  glfwMakeContextCurrent(window);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_SMOOTH);
  glfwSetFramebufferSizeCallback(window, redimensionar);

  // redimensiona a tela
  int largura_atual, altura_atual;
  glfwGetFramebufferSize(window, &largura_atual, &altura_atual);
  redimensionar(window, largura_atual, altura_atual);

  // loop principal
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    teste_iluminacao(posX_sol, posY_sol, posZ_sol);
    desenhar(raio_planeta, posX_sol, posY_sol, posZ_sol);

    GerenciarTeclado(window);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
