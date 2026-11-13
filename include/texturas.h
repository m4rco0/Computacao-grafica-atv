#ifndef TEXTURAS_H
#define TEXTURAS_H

#include "../include/stb_image.h"
#include <GLFW/glfw3.h>
#include <cstdio>

inline GLuint carregarTextura(const char *caminho) {
  int largura, altura, canais;

  // Inverte a imagem verticalmente (OpenGL espera origem Y=0 na base)
  stbi_set_flip_vertically_on_load(true);

  // O último parâmetro '4' força a stb_image a converter qualquer imagem
  // (1 canal grayscale, 3 canais RGB ou 4 canais RGBA) para RGBA de 4 canais
  unsigned char *dados = stbi_load(caminho, &largura, &altura, &canais, 4);
  if (!dados) {
    fprintf(stderr, "Erro ao carregar textura: %s\n", caminho);
    return 0;
  }

  GLuint texturaID;
  glGenTextures(1, &texturaID);
  glBindTexture(GL_TEXTURE_2D, texturaID);

  // Configura repetição e filtros de suavização
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Envia os pixels para a GPU em formato RGBA padronizado
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, largura, altura, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, dados);

  stbi_image_free(dados);
  glBindTexture(GL_TEXTURE_2D, 0);

  return texturaID;
}

#endif // TEXTURAS_H
