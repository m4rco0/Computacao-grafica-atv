#ifndef TARDIS_H
#define TARDIS_H

#include "basic_mesh.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

enum class EstadoTardis { POUSADA, SUBINDO, VOANDO, POUSANDO };

class Tardis {
public:
  Tardis() = default;
  Tardis(GLfloat tamanho, const glm::vec3 &pos, const glm::vec3 &cor);

  bool carregarModelo(const std::string &caminho, std::string &erro);
  void desenhar(const glm::vec3 &centro_terra) const;
  void atualizar(float dt, const glm::vec3 &centro_terra);
  void decolar(float raio_terra, float altura_h, float vel_subida,
               float vel_orbital);

  glm::vec3 getPosicao() const { return posicao; }
  glm::vec3 getDirecaoFrente(const glm::vec3 &centro_terra) const;
  bool estaNoAr() const { return estado != EstadoTardis::POUSADA; }

  void pousar(float raio_terra, float vel_subida);
  float getVelocidadeOrbital() { return this->velocidade_orbital; };
  void aumentarVelocidadeOrbital() { this->velocidade_orbital += 0.1f; };
  void diminuirVelocidadeOrbital() { this->velocidade_orbital -= 0.1f; };

private:
  float tamanho = 1.0f;
  EstadoTardis estado = EstadoTardis::POUSADA;
  float velocidade_subida = 0.0f;
  float velocidade_orbital = 0.0f;
  float distancia_alvo = 0.0f;
  glm::vec3 posicao{0.0f};
  glm::vec3 cor{0.0f, 0.2f, 0.6f};
  glm::vec3 eixo_orbita{0.0f, 0.0f, 1.0f};
  float anguloY = 0.0f;
  BasicMesh modelo;
};

#endif // TARDIS_H
