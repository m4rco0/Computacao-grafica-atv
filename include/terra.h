#ifndef TERRA_H
#define TERRA_H

#include "../include/esfera.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Terra : public Esfera {
public:
  Terra(GLfloat raio, glm::vec3 posicao_sol, GLfloat rotacao, glm::vec3 cor)
      : Esfera(raio, posicao_sol, rotacao, cor), posicao_sol(posicao_sol) {}

  void setTextura(GLuint id) { texturaID = id; }
  float getRaio() const { return raio; }

  void desenhar() const {
    if (texturaID != 0) {
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texturaID);
      glColor3f(1.0f, 1.0f, 1.0f);
    }

    Esfera::desenhar();

    if (texturaID != 0) {
      glBindTexture(GL_TEXTURE_2D, 0);
      glDisable(GL_TEXTURE_2D);
    }
  }

private:
  glm::vec3 posicao_sol;
  GLuint texturaID = 0;
};

#endif // TERRA_H
