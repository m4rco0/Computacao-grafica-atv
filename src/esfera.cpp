
#include "../include/esfera.h"
#include "../include/primitivas.h"
#include <GL/gl.h>
#include <GL/glext.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

void desenharEsfera(const float r, const int slices, const int stacks) {
  float dTheta = 2.0f * M_PI / slices;
  float dPhi = M_PI / stacks;

  for (int i = 0; i < stacks; ++i) {
    // Ângulos phi para a pilha atual e a próxima pilha
    float phi1 = -M_PI / 2.0f + i * dPhi;
    float phi2 = -M_PI / 2.0f + (i + 1) * dPhi;
    glBegin(GL_TRIANGLE_STRIP);
    for (int j = 0; j <= slices; ++j) {
      float theta = j * dTheta;

      // Vértice 1 (Pilha atual)
      float x1 = r * cosf(phi1) * cosf(theta);
      float y1 = r * sinf(phi1);
      float z1 = r * cosf(phi1) * sinf(theta);

      // Vetor normal normalizado (Pilha atual)
      glNormal3f(x1 / r, y1 / r, z1 / r);
      glVertex3f(x1, y1, z1);

      // Vértice 2 (Próxima pilha)
      float x2 = r * cosf(phi2) * cosf(theta);
      float y2 = r * sinf(phi2);
      float z2 = r * cosf(phi2) * sinf(theta);

      glNormal3f(x2 / r, y2 / r, z2 / r);
      glVertex3f(x2, y2, z2);
    }
    glEnd();
  }
}

void girarEsfera(GLfloat velocidade_rot) {
  float angulo = (float)glfwGetTime() * velocidade_rot; // Velocidade de rotação
  glTranslatef(0.0f, 0.0f, -3.0f);
  glRotatef(angulo, 1.0f, 0.0f, 0.0f);
  glRotatef(angulo, 0.0f, 1.0f, 0.0f);
  if (renderiza_malha)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  desenharEsfera(2, 30, 30);
  if (renderiza_malha)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void desenharSol(GLfloat raio, GLfloat posX, GLfloat posY, GLfloat posZ) {
  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(posX, posY, posZ);
  glColor3f(1.0f, 1.0f, 0.0f);

  if (renderiza_malha)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  desenharEsfera(raio, 20, 20);
  glColor3f(1.0f, 1.0f, 1.0f);
  glPopMatrix();
}

void desenharTerra(GLfloat raio) {
  glPushMatrix();
  glColor3f(0.2f, 0.5f, 1.0f);
  if (renderiza_malha)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  desenharEsfera(raio, 20, 20);
  glColor3f(1.0f, 1.0f, 1.0f);
  glPopMatrix();
}

void desenhar(GLfloat raio_planeta, GLfloat posX_sol, GLfloat posY_sol,
              GLfloat posZ_sol) {
  glDisable(GL_LIGHTING);
  desenharSol(4.0f, posX_sol, posY_sol, posZ_sol);
  glEnable(GL_LIGHTING);

  desenharTerra(raio_planeta);
}
