#ifndef BASIC_MASH_H
#define BASIC_MASH_H
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

class BasicMesh {
public:
  bool carregar(const std::string& caminho, std::string& erro) {
    Assimp::Importer importer;
    const aiScene* cena = importer.ReadFile(
        caminho,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_JoinIdenticalVertices
    );

    if (!cena || !cena->mRootNode ||
        (cena->mFlags & AI_SCENE_FLAGS_INCOMPLETE)) {
      erro = importer.GetErrorString();
      return false;
    }

    for (unsigned int m = 0; m < cena->mNumMeshes; ++m) {
      const aiMesh* malha = cena->mMeshes[m];
      const unsigned int deslocamento = vertices.size();

      for (unsigned int i = 0; i < malha->mNumVertices; ++i) {
        const auto& v = malha->mVertices[i];
        vertices.emplace_back(v.x, v.y, v.z);

        const auto& n = malha->mNormals[i];
        normais.emplace_back(n.x, n.y, n.z);
      }

      for (unsigned int f = 0; f < malha->mNumFaces; ++f) {
        const aiFace& face = malha->mFaces[f];
        for (unsigned int i = 0; i < face.mNumIndices; ++i)
          indices.push_back(deslocamento + face.mIndices[i]);
      }
    }

    return true;
  }

  void desenhar() const {
    glBegin(GL_TRIANGLES);
    for (unsigned int i : indices) {
      glNormal3f(normais[i].x, normais[i].y, normais[i].z);
      glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
    }
    glEnd();
  }

private:
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec3> normais;
  std::vector<unsigned int> indices;
};

#endif //BASIC_MASH_H