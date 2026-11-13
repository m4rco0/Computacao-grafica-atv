#ifndef ESFERA_H
#define ESFERA_H

#include <GL/gl.h>
#include <glm/glm.hpp>

class Esfera {
public:
  Esfera(float raio, const glm::vec3 &posicao, float velocidadeRot,
         const glm::vec3 &cor = glm::vec3(1.0f));
  virtual ~Esfera() = default;

  virtual void atualizar(float dt);
  virtual void desenhar() const;

  void setPosicao(const glm::vec3 &p) { posicao = p; }
  const glm::vec3 &getPosicao() const { return posicao; }

  void mudarRenderizacao(bool wireframe) {
    if (wireframe)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

protected:
  void desenharMalha(int slices = 32, int stacks = 32) const;

  float raio;
  glm::vec3 posicao;
  glm::vec3 cor;
  float velocidadeRot; // graus por segundo
  float anguloRot = 0.0f;
};

#endif // ESFERA_H
