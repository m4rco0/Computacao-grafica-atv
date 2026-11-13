#ifndef BASIC_MESH_H
#define BASIC_MESH_H

#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

struct SubMalha {
  GLenum modoDesenho = GL_TRIANGLES; // GL_TRIANGLES ou GL_LINES
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec3> normais;
  std::vector<unsigned int> indices;
  glm::vec4 corDifusa = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
  glm::mat4 transformacao = glm::mat4(1.0f);
};

class BasicMesh {
public:
  bool carregar(const std::string &caminho, std::string &erro) {
    Assimp::Importer importer;

    const aiScene *cena = importer.ReadFile(
        caminho, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                     aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

    if (!cena || !cena->mRootNode ||
        (cena->mFlags & AI_SCENE_FLAGS_INCOMPLETE)) {
      erro = importer.GetErrorString();
      return false;
    }

    subMalhas.clear();
    processarNo(cena->mRootNode, cena, glm::mat4(1.0f));
    return true;
  }

  void desenhar(float espessuraLinha = 1.5f) const {
    glLineWidth(espessuraLinha);

    for (const auto &sm : subMalhas) {
      glPushMatrix();
      glMultMatrixf(glm::value_ptr(sm.transformacao));

      // Aplica a cor do material
      glColor4f(sm.corDifusa.r, sm.corDifusa.g, sm.corDifusa.b, sm.corDifusa.a);
      float difusa[] = {sm.corDifusa.r, sm.corDifusa.g, sm.corDifusa.b,
                        sm.corDifusa.a};
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, difusa);

      // Se for linha e a iluminação estiver ligada na cena, desativa
      // temporariamente para que a linha não fique preta por falta de normais
      GLboolean iluminacaoAtiva = glIsEnabled(GL_LIGHTING);
      if (sm.modoDesenho == GL_LINES && iluminacaoAtiva) {
        glDisable(GL_LIGHTING);
      }

      glBegin(sm.modoDesenho);
      for (unsigned int i : sm.indices) {
        if (!sm.normais.empty() && sm.modoDesenho == GL_TRIANGLES) {
          glNormal3f(sm.normais[i].x, sm.normais[i].y, sm.normais[i].z);
        }
        glVertex3f(sm.vertices[i].x, sm.vertices[i].y, sm.vertices[i].z);
      }
      glEnd();

      // Restaura a iluminação caso tenha sido desativada para a linha
      if (sm.modoDesenho == GL_LINES && iluminacaoAtiva) {
        glEnable(GL_LIGHTING);
      }

      glPopMatrix();
    }
  }

private:
  std::vector<SubMalha> subMalhas;

  glm::mat4 converterMatriz(const aiMatrix4x4 &m) {
    glm::mat4 res;
    res[0][0] = m.a1;
    res[1][0] = m.a2;
    res[2][0] = m.a3;
    res[3][0] = m.a4;
    res[0][1] = m.b1;
    res[1][1] = m.b2;
    res[2][1] = m.b3;
    res[3][1] = m.b4;
    res[0][2] = m.c1;
    res[1][2] = m.c2;
    res[2][2] = m.c3;
    res[3][2] = m.c4;
    res[0][3] = m.d1;
    res[1][3] = m.d2;
    res[2][3] = m.d3;
    res[3][3] = m.d4;
    return res;
  }

  void processarNo(aiNode *no, const aiScene *cena,
                   const glm::mat4 &transformacaoPai) {
    glm::mat4 transformacaoAtual =
        transformacaoPai * converterMatriz(no->mTransformation);

    for (unsigned int i = 0; i < no->mNumMeshes; ++i) {
      aiMesh *malha = cena->mMeshes[no->mMeshes[i]];
      SubMalha sm;
      unsigned int indicesEsperados = 0;

      // Define o modo de desenho conforme o tipo primitivo da malha
      if (malha->mPrimitiveTypes & aiPrimitiveType_TRIANGLE) {
        sm.modoDesenho = GL_TRIANGLES;
        indicesEsperados = 3;
      } else if (malha->mPrimitiveTypes & aiPrimitiveType_LINE) {
        sm.modoDesenho = GL_LINES;
        indicesEsperados = 2;
      } else {
        continue; // Ignora pontos soltos (aiPrimitiveType_POINT)
      }

      sm.transformacao = transformacaoAtual;

      // Extrai a cor difusa do material associado
      if (malha->mMaterialIndex < cena->mNumMaterials) {
        aiMaterial *mat = cena->mMaterials[malha->mMaterialIndex];
        aiColor4D cor;
        if (AI_SUCCESS ==
            aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &cor)) {
          sm.corDifusa = glm::vec4(cor.r, cor.g, cor.b, cor.a);
        }
      }

      for (unsigned int v = 0; v < malha->mNumVertices; ++v) {
        sm.vertices.emplace_back(malha->mVertices[v].x, malha->mVertices[v].y,
                                 malha->mVertices[v].z);
        if (malha->HasNormals()) {
          sm.normais.emplace_back(malha->mNormals[v].x, malha->mNormals[v].y,
                                  malha->mNormals[v].z);
        }
      }

      for (unsigned int f = 0; f < malha->mNumFaces; ++f) {
        const aiFace &face = malha->mFaces[f];
        if (face.mNumIndices == indicesEsperados) {
          for (unsigned int idx = 0; idx < indicesEsperados; ++idx) {
            sm.indices.push_back(face.mIndices[idx]);
          }
        }
      }

      subMalhas.push_back(std::move(sm));
    }

    for (unsigned int i = 0; i < no->mNumChildren; ++i) {
      processarNo(no->mChildren[i], cena, transformacaoAtual);
    }
  }
};

#endif // BASIC_MESH_H
