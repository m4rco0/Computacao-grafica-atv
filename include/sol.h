#ifndef SOL
#define SOL
class Sol : public Esfera {
public:
    Sol(GLfloat raio, const glm::vec3 &posicao, const glm::vec3 &cor) : Esfera(raio, posicao, 0.0f, cor) {}
};


#endif // SOL