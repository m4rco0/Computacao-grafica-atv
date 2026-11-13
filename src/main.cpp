#define STB_IMAGE_IMPLEMENTATION
#include "../include/texturas.h"

#include "../include/camera.h"
#include "../include/iluminacao.h"
#include "../include/primitivas.h"
#include "../include/sol.h"
#include "../include/tardis.h"
#include "../include/terra.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
constexpr float ALTURA_DECOLAGEM = 1.0f;

bool lerParametro(const char *texto, GLfloat &valor) {
  char *fim;
  valor = strtof(texto, &fim);
  return fim != texto && *fim == '\0' && std::isfinite(valor);
}

void redimensionar(GLFWwindow *, int w, int h) {
  if (h == 0)
    h = 1;
  glViewport(0, 0, w, h);

  const float aspect = static_cast<float>(w) / static_cast<float>(h);

  glMatrixMode(GL_PROJECTION);
  const glm::mat4 proj =
      glm::perspective(glm::radians(60.0f), aspect, 0.05f, 200.0f);
  glLoadMatrixf(glm::value_ptr(proj));

  glMatrixMode(GL_MODELVIEW);
}

void desenhar(Terra &terra, Sol &sol, Camera &camera, Tardis &tardis,
              GLfloat posX_sol, GLfloat posY_sol, GLfloat posZ_sol) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  camera.update(terra.getRaio(), terra.getPosicao(), tardis.getPosicao(),
                tardis.getDirecaoFrente(terra.getPosicao()), tardis.estaNoAr());

  teste_iluminacao(posX_sol, posY_sol, posZ_sol);

  // Desenha a Terra
  terra.mudarRenderizacao(renderiza_malha);
  terra.desenhar();

  // Desenha o Sol (sem iluminação externa)
  GLboolean luz_ativa = glIsEnabled(GL_LIGHTING);
  if (luz_ativa)
    glDisable(GL_LIGHTING);
  sol.mudarRenderizacao(renderiza_malha);
  sol.desenhar();
  if (luz_ativa)
    glEnable(GL_LIGHTING);

  // Desenha a TARDIS
  tardis.desenhar(terra.getPosicao());
}

void GerenciarTeclado(GLFWwindow *window, Tardis &tardis, float raio_terra) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }

  static bool teclaR_anterior = false;
  const bool teclaR_atual = (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS);
  if (teclaR_atual && !teclaR_anterior) {
    renderiza_malha = !renderiza_malha;
  }
  teclaR_anterior = teclaR_atual;

  // SPACE: Decola até R + h e inicia órbita
  static bool espaco_anterior = false;
  const bool espaco_atual = (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);
  if (espaco_atual && !espaco_anterior) {
    tardis.decolar(raio_terra, ALTURA_DECOLAGEM, 0.5f, 0.35f);
  }
  espaco_anterior = espaco_atual;

  // P: Aterrissa suavemente de volta até o raio R
  static bool teclaP_anterior = false;
  const bool teclaP_atual = (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS);
  if (teclaP_atual && !teclaP_anterior) {
    tardis.pousar(raio_terra, 0.5f);
  }

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    tardis.aumentarVelocidadeOrbital();
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    tardis.diminuirVelocidadeOrbital();
  }
  teclaP_anterior = teclaP_atual;
}

int main(int argc, char **argv) {
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

  const int LARGURA = 900;
  const int ALTURA = 900;
  if (!glfwInit())
    return EXIT_FAILURE;

  GLFWwindow *window =
      glfwCreateWindow(LARGURA, ALTURA, "Desenhando esfera", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);

  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glShadeModel(GL_SMOOTH);
  glfwSetFramebufferSizeCallback(window, redimensionar);

  int largura_atual, altura_atual;
  glfwGetFramebufferSize(window, &largura_atual, &altura_atual);
  redimensionar(window, largura_atual, altura_atual);

  // Configura a Terra e sua textura (cor branca para não tingir a imagem)
  Terra terra(raio_planeta, glm::vec3(1.0f, 0.0f, 0.0f), 30.0f,
              glm::vec3(1.0f, 1.0f, 1.0f));
  GLuint texTerra = carregarTextura("texturas/terra/earth albedo.jpg");
  terra.setTextura(texTerra);

  // Configura o Sol
  Sol sol(1.0f, glm::vec3(posX_sol, posY_sol, posZ_sol),
          glm::vec3(1.0f, 1.0f, 0.0f));

  // Configura a TARDIS na superfície da Terra (+X)
  const glm::vec3 direcao_tardis = glm::vec3(1.0f, 0.0f, 0.0f);
  const glm::vec3 pos_tardis =
      terra.getPosicao() + direcao_tardis * raio_planeta;

  Tardis tardis(terra.getRaio() * 0.15f, pos_tardis,
                glm::vec3(0.0f, 0.0f, 1.0f));
  std::string error;
  if (!tardis.carregarModelo("models/tardis.obj", error)) {
    fprintf(stderr, "Erro ao carregar modelo: %s\n", error.c_str());
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_FAILURE;
  }

  Camera camera;
  double tempo_anterior = glfwGetTime();

  while (!glfwWindowShouldClose(window)) {
    const double agora = glfwGetTime();
    const float dt = static_cast<float>(agora - tempo_anterior);
    tempo_anterior = agora;

    tardis.atualizar(dt, terra.getPosicao());
    desenhar(terra, sol, camera, tardis, posX_sol, posY_sol, posZ_sol);
    GerenciarTeclado(window, tardis, terra.getRaio());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
