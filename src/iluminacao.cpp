#include "../include/iluminacao.h"
#include <GL/gl.h>
#include <glm/glm.hpp>

void teste_iluminacao(GLfloat posX, GLfloat posY, GLfloat posZ) {
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

  // Posição pontual (w = 1.0f) exatamente no centro do Sol
  GLfloat posicao_luz[] = {posX, posY, posZ, 1.0f};

  // Aumente os valores acima de 1.0f (ex: 1.5f a 2.5f) para uma luz solar mais
  // forte
  GLfloat luz_ambiente[] = {0.25f, 0.25f, 0.25f,
                            1.0f}; // Ilumina levemente o lado escuro
  GLfloat luz_difusa[] = {1.8f, 1.75f, 1.6f,
                          1.0f}; // Luz principal forte e levemente quente
  GLfloat luz_especular[] = {
      1.5f, 1.5f, 1.5f, 1.0f}; // Brilho reflexivo (oceanos e metal da TARDIS)

  glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);
  glLightfv(GL_LIGHT0, GL_AMBIENT, luz_ambiente);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_difusa);
  glLightfv(GL_LIGHT0, GL_SPECULAR, luz_especular);

  // Reduz a perda de brilho pela distância (evita que a luz enfraqueça rápido)
  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.01f);
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.001f);
}
