#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>
#ifndef ESFERA_H
#define ESFERA_H



void desenharEsfera(const float r, const int slices, const int stacks);
void girarEsfera(GLfloat velocidade_rot) ;

void desenharSol(GLfloat raio);
void desenharTerra(GLfloat raio);

void girarSol(GLfloat velocidade_rot);
void girarTerra(GLfloat velocidade_rot);
#endif // ESFERA_H
