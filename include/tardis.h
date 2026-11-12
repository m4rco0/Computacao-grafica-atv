

#ifndef TARDIS_H
#define TARDIS_H

#include "basic_mesh.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

class Tardis {
public:
    Tardis();
    Tardis(GLfloat tamanho, const glm::vec3 &pos, const glm::vec3 &cor);

    bool carregarModelo(const std::string& caminho, std::string& erro);
    void desenhar(glm::vec3 centro_terra);
    void desenharCubos() const;
    void atualizar(float dt, const glm::vec3& centro_terra);
    void decolar(float distancia_alvo, float velocidade);
    glm::vec3 getPosicao() const { return posicao; }

private:
    float tamanho;
    float velocidade_decolagem = 0.0f;
    float distancia_alvo_decolagem = 0.0f;
    glm::vec3 posicao;
    glm::vec3 cor;
    float anguloY;
    BasicMesh modelo;
};

#endif
