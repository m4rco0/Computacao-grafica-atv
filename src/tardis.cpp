#include "../include/tardis.h"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cmath>
#include <glm/gtc/constants.hpp>

namespace {
constexpr float ALTURA_MODELO_TARDIS = 12.17f;
constexpr float OFFSET_BASE_Y = 6.041f;
constexpr float VELOCIDADE_GIRO_GRAUS = 90.0f;
constexpr float EPSILON = 1e-6f;
const glm::vec3 EIXO_Y(0.0f, 1.0f, 0.0f);
} // namespace

Tardis::Tardis(GLfloat tamanho, const glm::vec3 &pos, const glm::vec3 &cor)
    : tamanho(tamanho), posicao(pos), cor(cor) {}

bool Tardis::carregarModelo(const std::string &caminho, std::string &erro) {
  return modelo.carregar(caminho, erro);
}

void Tardis::desenhar(const glm::vec3 &centro_terra) const {
  const glm::vec3 deslocamento = posicao - centro_terra;
  const float distancia = glm::length(deslocamento);
  if (distancia <= EPSILON)
    return;

  const glm::vec3 normal = deslocamento / distancia;
  const glm::vec3 eixo_rotacao = glm::cross(EIXO_Y, normal);
  const float comprimento_eixo = glm::length(eixo_rotacao);
  const float escala = tamanho / ALTURA_MODELO_TARDIS;

  glPushMatrix();

  glTranslatef(posicao.x, posicao.y, posicao.z);

  if (comprimento_eixo > EPSILON) {
    const float angulo =
        glm::degrees(std::atan2(comprimento_eixo, glm::dot(EIXO_Y, normal)));
    glRotatef(angulo, eixo_rotacao.x, eixo_rotacao.y, eixo_rotacao.z);
  } else if (normal.y < 0.0f) {
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
  }

  glRotatef(anguloY, 0.0f, 1.0f, 0.0f);

  glScalef(escala, escala, escala);
  glTranslatef(0.0f, OFFSET_BASE_Y, 0.0f);

  modelo.desenhar();

  glPopMatrix();
}

void Tardis::decolar(float raio_terra, float altura_h, float vel_subida,
                     float vel_orbital) {
  if (estado != EstadoTardis::POUSADA)
    return;

  distancia_alvo =
      raio_terra + altura_h; // Distância R + h do centro do planeta
  velocidade_subida = vel_subida;
  velocidade_orbital = vel_orbital;
  estado = EstadoTardis::SUBINDO;
}

void Tardis::pousar(float raio_terra, float vel_descida) {
  if (estado == EstadoTardis::POUSADA || estado == EstadoTardis::POUSANDO)
    return;

  distancia_alvo = raio_terra; // Alvo volta a ser o raio R da superfície
  velocidade_subida = vel_descida;
  estado = EstadoTardis::POUSANDO;
}

void Tardis::atualizar(float dt, const glm::vec3 &centro_terra) {
  if (estado == EstadoTardis::POUSADA)
    return;

  const glm::vec3 deslocamento = posicao - centro_terra;
  const float distancia = glm::length(deslocamento);
  if (distancia <= EPSILON)
    return;

  const glm::vec3 normal = deslocamento / distancia;

  if (estado == EstadoTardis::SUBINDO) {
    const float distancia_restante = distancia_alvo - distancia;
    if (distancia_restante > 0.0f) {
      const float passo = std::min(velocidade_subida * dt, distancia_restante);
      posicao += normal * passo;
    }

    if (glm::length(posicao - centro_terra) >= distancia_alvo - EPSILON) {
      estado = EstadoTardis::VOANDO;
    }
  } else if (estado == EstadoTardis::VOANDO) {
    const glm::mat4 rotacao =
        glm::rotate(glm::mat4(1.0f), velocidade_orbital * dt, eixo_orbita);
    const glm::vec3 novo_deslocamento =
        glm::vec3(rotacao * glm::vec4(deslocamento, 0.0f));
    posicao = centro_terra + glm::normalize(novo_deslocamento) * distancia_alvo;
  } else if (estado == EstadoTardis::POUSANDO) {
    // Desce radialmente em direção ao solo até atingir o raio R
    const float distancia_restante = distancia - distancia_alvo;
    if (distancia_restante > 0.0f) {
      const float passo = std::min(velocidade_subida * dt, distancia_restante);
      posicao -= normal * passo;
    }

    // Ao encostar na superfície (R), trava exatamente em R e muda para POUSADA
    if (glm::length(posicao - centro_terra) <= distancia_alvo + EPSILON) {
      posicao = centro_terra + normal * distancia_alvo;
      estado = EstadoTardis::POUSADA;
    }
  }

  anguloY = std::fmod(anguloY + VELOCIDADE_GIRO_GRAUS * dt, 360.0f);
}
glm::vec3 Tardis::getDirecaoFrente(const glm::vec3 &centro_terra) const {
  const glm::vec3 deslocamento = posicao - centro_terra;
  if (glm::length(deslocamento) <= EPSILON) {
    return glm::vec3(0.0f, 0.0f, -1.0f);
  }
  const glm::vec3 normal = glm::normalize(deslocamento);
  return glm::normalize(glm::cross(eixo_orbita, normal));
}
