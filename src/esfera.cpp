// Esfera.cpp
#include "../include/esfera.h"
#include <cmath>

Esfera::Esfera(float r, const glm::vec3& pos, float vel, const glm::vec3& c)
    : raio(r), posicao(pos), cor(c), velocidadeRot(vel) {}

void Esfera::atualizar(float dt) {
    anguloRot += velocidadeRot * dt;
    if (anguloRot >= 360.0f) anguloRot -= 360.0f;
}

void Esfera::desenhar() const {
    glPushMatrix();
        glTranslatef(posicao.x, posicao.y, posicao.z);
        glRotatef(anguloRot, 0.0f, 1.0f, 0.0f);
        glColor3f(cor.r, cor.g, cor.b);
        desenharMalha();
    glPopMatrix();
}


void Esfera::desenharMalha(int slices, int stacks) const {
    for (int i = 0; i < stacks; ++i) {
        float lat0 = M_PI * (-0.5f + (float)i / stacks);
        float lat1 = M_PI * (-0.5f + (float)(i + 1) / stacks);
        float z0 = sin(lat0), zr0 = cos(lat0);
        float z1 = sin(lat1), zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; ++j) {
            float lng = 2.0f * M_PI * (float)j / slices;
            float x = cos(lng), y = sin(lng);
            glNormal3f(x * zr0, y * zr0, z0);
            glVertex3f(raio * x * zr0, raio * y * zr0, raio * z0);
            glNormal3f(x * zr1, y * zr1, z1);
            glVertex3f(raio * x * zr1, raio * y * zr1, raio * z1);
        }
        glEnd();
    }
}