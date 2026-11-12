#include "../include/tardis.h"
#include <GL/gl.h>
#include <glm/common.hpp>
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

bool Tardis::carregarModelo(const std::string& caminho, std::string& erro) {
  return modelo.carregar(caminho, erro);
}

void Tardis::desenhar(glm::vec3 centro_terra) {
  const glm::vec3 normal = glm::normalize(posicao - centro_terra);
  const glm::vec3 eixo = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), normal);
  const float comprimento_eixo = glm::length(eixo);
  const float escala = tamanho / 2.3f;

  glPushMatrix();
  glTranslatef(posicao.x, posicao.y, posicao.z);
  if (comprimento_eixo > 1e-6f) {
    const float angulo = glm::degrees(std::atan2(
        comprimento_eixo, glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), normal)));
    glRotatef(angulo, eixo.x, eixo.y, eixo.z);
  } else if (normal.y < 0.0f) {
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
  }
  glScalef(escala, escala, escala);
  glColor3f(cor.r, cor.g, cor.b);
  modelo.desenhar();
  glPopMatrix();
}

void Tardis::decolar(float distancia_alvo, float velocidade) {
  distancia_alvo_decolagem = distancia_alvo;
  velocidade_decolagem = velocidade;
}

void Tardis::atualizar(float dt, const glm::vec3& centro_terra) {
  const glm::vec3 deslocamento = posicao - centro_terra;
  const float distancia = glm::length(deslocamento);
  const float distancia_restante = distancia_alvo_decolagem - distancia;

  if (distancia > 1e-6f && velocidade_decolagem > 0.0f &&
      distancia_restante > 0.0f) {
    const float movimento = velocidade_decolagem * dt;
    const float movimento_aplicado =
        movimento < distancia_restante ? movimento : distancia_restante;
    posicao += (deslocamento / distancia) * movimento_aplicado;

    if (movimento_aplicado >= distancia_restante)
      velocidade_decolagem = 0.0f;
  }

  anguloY += dt;
  if (anguloY > 360.0f)
    anguloY -= 360.0f;
}