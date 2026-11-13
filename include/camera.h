#ifndef CAMERA_H
#define CAMERA_H

#include <GL/gl.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
  Camera() : distancia(3.0f), theta(0.0f), phi(0.4f), esta_girando(false) {}

  void update(float raio_terra, const glm::vec3 &centro_terra,
              const glm::vec3 &pos_tardis, const glm::vec3 &frente_tardis,
              bool no_ar) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (no_ar) {
      // Perspectiva da TARDIS vendo o solo da Terra se afastar abaixo
      glm::vec3 normal = glm::normalize(pos_tardis - centro_terra);
      glm::vec3 olho = pos_tardis + normal * 0.9f - frente_tardis * 1.4f;
      glm::vec3 alvo = pos_tardis - normal * 1.2f + frente_tardis * 0.8f;

      glm::mat4 view = glm::lookAt(olho, alvo, normal);
      glLoadMatrixf(glm::value_ptr(view));
    } else {
      // Câmera externa orbitando o planeta quando pousada
      float r = raio_terra * 3.0f;
      float x = centro_terra.x + r * std::cos(phi) * std::sin(theta);
      float y = centro_terra.y + r * std::sin(phi);
      float z = centro_terra.z + r * std::cos(phi) * std::cos(theta);

      glm::mat4 view = glm::lookAt(glm::vec3(x, y, z), centro_terra,
                                   glm::vec3(0.0f, 1.0f, 0.0f));
      glLoadMatrixf(glm::value_ptr(view));
    }
  }

  void setAngles(float t, float p) {
    theta = t;
    phi = p;
  }
  float getTheta() const { return theta; }
  float getPhi() const { return phi; }

private:
  float distancia;
  float theta;
  float phi;
  bool esta_girando;
};

#endif // CAMERA_H
