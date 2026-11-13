// Esfera.cpp
#include "../include/esfera.h"
#include <cmath>

Esfera::Esfera(float r, const glm::vec3 &pos, float vel, const glm::vec3 &c)
    : raio(r), posicao(pos), cor(c), velocidadeRot(vel) {}

void Esfera::atualizar(float dt) {
  anguloRot += velocidadeRot * dt;
  if (anguloRot >= 360.0f)
    anguloRot -= 360.0f;
}

void Esfera::desenhar() const {
  glPushMatrix();
  glTranslatef(posicao.x, posicao.y, posicao.z);
  glRotatef(anguloRot, 0.0f, 1.0f, 0.0f);

  // Se houver textura ativa, usa branco para preservar as cores originais da
  // imagem
  if (glIsEnabled(GL_TEXTURE_2D)) {
    glColor3f(1.0f, 1.0f, 1.0f);
  } else {
    glColor3f(cor.r, cor.g, cor.b);
  }

  // Alinha os polos (que a sua fórmula coloca no eixo Z) ao eixo Y
  glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

  desenharMalha();
  glPopMatrix();
}

void Esfera::desenharMalha(int slices, int stacks) const {
  for (int i = 0; i < stacks; ++i) {
    float v0 = static_cast<float>(i) / stacks;
    float v1 = static_cast<float>(i + 1) / stacks;

    float lat0 = M_PI * (-0.5f + v0);
    float lat1 = M_PI * (-0.5f + v1);
    float z0 = std::sin(lat0), zr0 = std::cos(lat0);
    float z1 = std::sin(lat1), zr1 = std::cos(lat1);

    glBegin(GL_QUAD_STRIP);
    for (int j = 0; j <= slices; ++j) {
      float u = static_cast<float>(j) / slices;
      float lng = 2.0f * M_PI * u;
      float x = std::cos(lng), y = std::sin(lng);

      glTexCoord2f(u, v0);
      glNormal3f(x * zr0, y * zr0, z0);
      glVertex3f(raio * x * zr0, raio * y * zr0, raio * z0);

      glTexCoord2f(u, v1);
      glNormal3f(x * zr1, y * zr1, z1);
      glVertex3f(raio * x * zr1, raio * y * zr1, raio * z1);
    }
    glEnd();
  }
}
