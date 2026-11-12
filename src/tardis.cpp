#include "../include/tardis.h"
#include <GL/gl.h>
#include <cmath>
#include <glm/glm.hpp>
namespace {
void desenharBloco(float largura, float altura, float profundidade,
                   float x, float y, float z) {
  const float xMin = x - largura / 2.0f;
  const float xMax = x + largura / 2.0f;
  const float yMin = y - altura / 2.0f;
  const float yMax = y + altura / 2.0f;
  const float zMin = z - profundidade / 2.0f;
  const float zMax = z + profundidade / 2.0f;

  glBegin(GL_QUADS);
  glNormal3f(0.0f, 0.0f, 1.0f);
  glVertex3f(xMin, yMin, zMax);
  glVertex3f(xMax, yMin, zMax);
  glVertex3f(xMax, yMax, zMax);
  glVertex3f(xMin, yMax, zMax);

  glNormal3f(0.0f, 0.0f, -1.0f);
  glVertex3f(xMax, yMin, zMin);
  glVertex3f(xMin, yMin, zMin);
  glVertex3f(xMin, yMax, zMin);
  glVertex3f(xMax, yMax, zMin);

  glNormal3f(1.0f, 0.0f, 0.0f);
  glVertex3f(xMax, yMin, zMax);
  glVertex3f(xMax, yMin, zMin);
  glVertex3f(xMax, yMax, zMin);
  glVertex3f(xMax, yMax, zMax);

  glNormal3f(-1.0f, 0.0f, 0.0f);
  glVertex3f(xMin, yMin, zMin);
  glVertex3f(xMin, yMin, zMax);
  glVertex3f(xMin, yMax, zMax);
  glVertex3f(xMin, yMax, zMin);

  glNormal3f(0.0f, 1.0f, 0.0f);
  glVertex3f(xMin, yMax, zMax);
  glVertex3f(xMax, yMax, zMax);
  glVertex3f(xMax, yMax, zMin);
  glVertex3f(xMin, yMax, zMin);

  glNormal3f(0.0f, -1.0f, 0.0f);
  glVertex3f(xMin, yMin, zMin);
  glVertex3f(xMax, yMin, zMin);
  glVertex3f(xMax, yMin, zMax);
  glVertex3f(xMin, yMin, zMax);
  glEnd();
}

void desenharFachada(float largura, float profundidade, float tamanho) {
  const float frente = profundidade / 2.0f;
  const float espessura = tamanho * 0.012f;
  const float larguraPorta = largura * 0.36f;
  const float alturaPorta = tamanho * 0.43f;
  const float centroPortaY = tamanho * 0.34f;

  glColor3f(0.10f, 0.22f, 0.52f);
  for (int lado = -1; lado <= 1; lado += 2) {
    const float centroX = lado * largura * 0.23f;
    desenharBloco(larguraPorta, alturaPorta, espessura,
                  centroX, centroPortaY, frente + espessura / 2.0f);

    glColor3f(0.025f, 0.08f, 0.25f);
    const float painelLargura = larguraPorta * 0.32f;
    const float painelAltura = alturaPorta * 0.20f;
    for (int coluna = 0; coluna < 2; ++coluna) {
      for (int linha = 0; linha < 3; ++linha) {
        const float x = centroX + (coluna == 0 ? -1.0f : 1.0f) *
                                      larguraPorta * 0.25f;
        const float y = centroPortaY - alturaPorta * 0.32f +
                        linha * alturaPorta * 0.32f;
        desenharBloco(painelLargura, painelAltura, espessura,
                      x, y, frente + espessura * 1.6f);
      }
    }
    glColor3f(0.10f, 0.22f, 0.52f);
  }

  // Placa e quatro vidros na parte superior de cada face.
  glColor3f(0.82f, 0.86f, 0.90f);
  desenharBloco(largura * 0.86f, tamanho * 0.055f, espessura,
                0.0f, tamanho * 0.84f, frente + espessura / 2.0f);
  glColor3f(0.02f, 0.07f, 0.20f);
  for (int i = 0; i < 7; ++i) {
    const float x = -largura * 0.34f + i * largura * 0.11f;
    desenharBloco(largura * 0.035f, tamanho * 0.022f, espessura,
                  x, tamanho * 0.84f, frente + espessura * 1.6f);
  }

  const float janelaY = tamanho * 0.70f;
  const float janelaLargura = largura * 0.25f;
  const float janelaAltura = tamanho * 0.17f;
  for (int lado = -1; lado <= 1; lado += 2) {
    const float centroX = lado * largura * 0.25f;
    glColor3f(0.025f, 0.08f, 0.25f);
    desenharBloco(janelaLargura, janelaAltura, espessura,
                  centroX, janelaY, frente + espessura / 2.0f);
    glColor3f(0.58f, 0.78f, 0.91f);
    desenharBloco(janelaLargura * 0.78f, janelaAltura * 0.80f, espessura,
                  centroX, janelaY, frente + espessura * 1.6f);
    glColor3f(0.025f, 0.08f, 0.25f);
    desenharBloco(janelaLargura * 0.08f, janelaAltura * 0.92f, espessura,
                  centroX, janelaY, frente + espessura * 2.7f);
    desenharBloco(janelaLargura * 0.92f, janelaAltura * 0.08f, espessura,
                  centroX, janelaY, frente + espessura * 2.7f);
  }

  // Maçaneta na porta direita.
  glColor3f(0.78f, 0.62f, 0.24f);
  desenharBloco(largura * 0.025f, tamanho * 0.045f, espessura * 1.5f,
                largura * 0.06f, tamanho * 0.34f,
                frente + espessura * 2.5f);
}


} 

Tardis::Tardis() : Tardis(3.0f, glm::vec3( 0.0f, 0.0f, 0.0f),
                          glm::vec3(0.02f, 0.12f, 0.42f)) {}

Tardis::Tardis(GLfloat tamanho, const glm::vec3 &pos, const glm::vec3 &cor)
    : tamanho(tamanho), posicao(pos), cor(cor), anguloY(0.0f) {}


void Tardis::desenhar(glm::vec3 centro_terra) {
  const float largura = tamanho * (2.0f / 3.0f);
  const float profundidade = tamanho * (2.0f / 3.0f);

  GLint modo_anterior[2];
  glGetIntegerv(GL_POLYGON_MODE, modo_anterior);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // normal da esfera no ponto onde a TARDIS está = "para cima" local
  glm::vec3 n = glm::normalize(posicao - centro_terra);
  glm::vec3 y(0.0f, 1.0f, 0.0f);

  glPushMatrix();
  glTranslatef(posicao.x, posicao.y, posicao.z);

  // gira o +Y do bloco até alinhar com a normal
  glm::vec3 eixo = glm::cross(y, n);
  float comp = glm::length(eixo);
  if (comp > 1e-6f) {
    float ang = glm::degrees(std::atan2(comp, glm::dot(y, n)));
    glRotatef(ang, eixo.x, eixo.y, eixo.z);
  } else if (n.y < 0.0f) {
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);   // caso n = -Y
  }

  glColor3f(cor.r, cor.g, cor.b);
  desenharBloco(largura, tamanho, profundidade, 0.0f, tamanho / 2.0f, 0.0f);

  // Rodape, moldura superior e telhado em degraus.
  glColor3f(cor.r * 0.55f, cor.g * 0.55f, cor.b * 0.65f);
  desenharBloco(largura * 1.08f, tamanho * 0.08f, profundidade * 1.08f,
                0.0f, tamanho * 0.04f, 0.0f);
  glColor3f(cor.r * 0.75f, cor.g * 0.75f, cor.b * 0.85f);
  desenharBloco(largura * 1.08f, tamanho * 0.07f, profundidade * 1.08f,
                0.0f, tamanho * 0.91f, 0.0f);
  desenharBloco(largura * 1.16f, tamanho * 0.05f, profundidade * 1.16f,
                0.0f, tamanho * 0.97f, 0.0f);
  desenharBloco(largura * 0.76f, tamanho * 0.07f, profundidade * 0.76f,
                0.0f, tamanho * 1.03f, 0.0f);

  for (int face = 0; face < 4; ++face) {
    glPushMatrix();
    glRotatef(face * 90.0f, 0.0f, 1.0f, 0.0f);
    desenharFachada(largura, profundidade, tamanho);
    glPopMatrix();
  }

  // Luminaria no topo, caracteristica da cabine policial.
  glColor3f(cor.r * 0.55f, cor.g * 0.55f, cor.b * 0.65f);
  desenharBloco(tamanho * 0.07f, tamanho * 0.06f, tamanho * 0.07f,
                0.0f, tamanho * 1.10f, 0.0f);
  glColor3f(0.72f, 0.86f, 1.0f);
  desenharBloco(tamanho * 0.11f, tamanho * 0.14f, tamanho * 0.11f,
                0.0f, tamanho * 1.20f, 0.0f);
  glColor3f(cor.r * 0.55f, cor.g * 0.55f, cor.b * 0.65f);
  desenharBloco(tamanho * 0.15f, tamanho * 0.035f, tamanho * 0.15f,
                0.0f, tamanho * 1.29f, 0.0f);

  glPopMatrix();
  glPolygonMode(GL_FRONT_AND_BACK, modo_anterior[0]);
}

void Tardis::decolar(float velocidade) {
}
void Tardis::atualizar(float dt) {
  anguloY += dt;

  if (anguloY > 360.0f)
    anguloY -= 360.0f;

}