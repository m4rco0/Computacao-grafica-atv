#ifndef TERRA
#define TERRA

#include <glm/glm.hpp>
#include "../include/esfera.h"
class Terra : public Esfera {
    glm::vec3 posicao_sol;
    public:
        Terra(GLfloat raio, glm::vec3 posicao_sol, GLfloat rotacao, glm::vec3 cor) : Esfera(raio, posicao_sol, rotacao, cor) {
            this->posicao_sol = posicao_sol;
            this->raio = raio;
            this->cor = cor;
        }
        float getRaio() { return raio;};
};
#endif // TERRA
