#ifndef CAMERA_H 
#define CAMERA_H
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
class Camera{
  public:
    glm::vec3 pos;
    glm::vec3 forward;
    int h_atual;
    float velocidade;
    float theta;
    double ultimo_tempo;
    bool esta_girando;

    Camera() : 
      pos(0.0f, 0.0f, 1.0f), forward(0.0f, 0.0f, -1.0f), 
      h_atual(0.0f), velocidade(0.0f), esta_girando(false), theta(0.0f), ultimo_tempo(0.0) {}
 
    float distanciaAoCentro(float raio_terra) const {
      return raio_terra + h_atual;
    }
    
    void update(float raio_terra, glm::vec3 terra_pos, glm::vec3 alvo) {
      double tempo_agr = glfwGetTime();
      float dt = (float)(tempo_agr - ultimo_tempo);
      ultimo_tempo = tempo_agr;

      if (dt > 0.1f) dt = 0.1f;

      const float dist = distanciaAoCentro(raio_terra) * 2.5f;

      theta += velocidade * dt;
      if (theta > 2.0f * (float)M_PI)
        theta -= 2.0f * (float)M_PI;
      if(theta < 0.0f)
        theta += 2.0f * (float)M_PI;

      const glm::vec3 up = glm::normalize(alvo - terra_pos);
      glm::vec3 tangent = glm::cross(up, glm::vec3(0.0f, 1.0f, 0.0f));
      if (glm::length(tangent) < 1e-6f)
        tangent = glm::cross(up, glm::vec3(0.0f, 0.0f, 1.0f));
      tangent = glm::normalize(tangent);
      const glm::vec3 bitangent = glm::normalize(glm::cross(up, tangent));
      const glm::vec3 orbit =
          std::cos(theta) * tangent + std::sin(theta) * bitangent;
      const glm::vec3 eye = alvo + dist * (orbit + 0.35f * up);
      const glm::mat4 view = glm::lookAt(eye, alvo, up);

      this->pos = eye;
      glMatrixMode(GL_MODELVIEW);
      glLoadMatrixf(glm::value_ptr(view));
    }

    void decolar(float velocidade) {
      this->velocidade = velocidade;
      this->esta_girando = true;
    }
    bool Girando() {
      return this->esta_girando;
    }

};

#endif //CAMERA_H
