#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>
#ifndef ESFERA_H
#define ESFERA_H

void desenharEsfera(const float r, const int slices, const int stacks);
void girarEsfera(GLfloat velocidade_rot);

void desenharSol(GLfloat raio, GLfloat posX, GLfloat posY, GLfloat posZ);
void desenharTerra(GLfloat raio);
void desenhar(GLfloat raio_planeta, GLfloat posX_sol, GLfloat posY_sol,
              GLfloat posZ_sol);

#endif // ESFERA_H
