

#ifndef TARDIS_H
#define TARDIS_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Tardis {
public:
    Tardis();
    Tardis(GLfloat tamanho, const glm::vec3 &pos, const glm::vec3 &cor);

    void desenharCubos() const;
    void desenhar(glm::vec3 centro_terra);
    void  atualizar(float dt);
    void decolar(float velocidade);
    glm::vec3 getPosicao() const { return posicao; }
private:
    float tamanho;
    glm::vec3 posicao;
    glm::vec3 cor;
    float anguloY;
};

#endif
