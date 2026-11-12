#include "../include/iluminacao.h"
#include "../include/primitivas.h"
#include "../include/tardis.h"
#include "../include/terra.h"
#include "../include/sol.h"
#include "../include/camera.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/geometric.hpp>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
  glm::mat4 proj = glm::perspective(glm::radians(60.0f), aspect, 0.4f, 200.0f);
  glLoadMatrixf(glm::value_ptr(proj));

  glMatrixMode(GL_MODELVIEW);
}


void desenhar(Terra &terra, Sol &sol, Camera &camera,Tardis &tardis, GLfloat posX_sol, GLfloat posY_sol, GLfloat posZ_sol) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  camera.update(terra.getRaio(), terra.getPosicao(), tardis.getPosicao());
  teste_iluminacao(posX_sol, posY_sol, posZ_sol);

  // esfera desenhos
  terra.desenhar();
  terra.mudarRenderizacao(renderiza_malha);

  // desenhar o sol
  sol.desenhar();
  sol.mudarRenderizacao(renderiza_malha);
  tardis.desenhar(terra.getPosicao());
  tardis.decolar(0.4f);
}

void GerenciarTeclado(GLFWwindow *window, Camera &camera) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }

  // Variável que guarda o estado anterior da tecla R
  static bool teclaR_pressionada_anteriormente = false;

  bool teclaR_atual = (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS);

  if (teclaR_atual && !teclaR_pressionada_anteriormente) {
    renderiza_malha = !renderiza_malha;
  }

  if (glfwGetKey(window , GLFW_KEY_SPACE) == GLFW_PRESS) {
      if(camera.esta_girando) {
        camera.decolar(0.0f);
        camera.esta_girando = false;
      } else {
        camera.decolar(0.3f);
        camera.esta_girando = true;
      }
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
  const int ALTURRA = 800;
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

  Terra terra(raio_planeta, glm::vec3(1.0f, 0.0f, 0.0f), 30.0f,
              glm::vec3(0.0f, 0.0f, 1.0f));

  Sol sol(1.0f, glm::vec3(posX_sol, posY_sol, posZ_sol), glm::vec3(100.0f, 100.0f, 0.0f));


  const glm::vec3 pos_tardis =
      terra.getPosicao() + glm::vec3(1.0f * raio_planeta,0.0f, 0.0f);
  Tardis tardis(raio_planeta * 0.28f, pos_tardis, glm::vec3(0, 0, 1));
  Camera camera;
  // loop principal
  while (!glfwWindowShouldClose(window)) {
    desenhar(terra, sol, camera, tardis, posX_sol, posY_sol, posZ_sol);
    GerenciarTeclado(window, camera);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
