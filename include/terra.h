#ifndef TERRA
#define TERRA

class Terra : public Esfera {
    glm::vec3 posicao_sol;
    public:
        Terra(GLfloat raio, glm::vec3 posicao_sol, GLfloat rotacao, glm::vec3 cor) : Esfera(raio, posicao_sol, rotacao, cor) {
            this->posicao_sol = posicao_sol;
            this->raio = raio;
            this->cor = cor;
        }
};
#endif // TERRA