#include "../include/iluminacao.h"
#include <GL/gl.h>
#include <glm/glm.hpp>

void teste_iluminacao(GLfloat posX, GLfloat posY, GLfloat posZ) {
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_NORMALIZE);

  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

  const GLfloat posicao[] = {posX, posY, posZ, 1.0f};
  const GLfloat ambiente[] = {0.08f, 0.08f, 0.08f, 1.0f};
  const GLfloat difusa[] = {1.0f, 0.95f, 0.8f, 1.0f};
  const GLfloat especular[] = {1.0f, 1.0f, 1.0f, 1.0f};

  glLightfv(GL_LIGHT0, GL_POSITION, posicao);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambiente);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, difusa);
  glLightfv(GL_LIGHT0, GL_SPECULAR, especular);
}