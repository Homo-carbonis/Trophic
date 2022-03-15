#include<glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace matrix {

  const float tau=glm::two_pi<float>();

  const glm::mat3 identity = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1} };

  glm::mat3 translate(glm::vec2 t)
  {
    return { {1, 0, 0}, {0, 1, 0}, {t[0], t[1], 1} };
  }

  glm::mat3 rotate(float r)
  {
    float sinr = sin(r);
    float cosr = cos(r);
    return { {cosr, sinr, 0}, {-sinr, cosr, 0}, {0, 0, 1} };
  }

  glm::mat3 scale(glm::vec2 s)
  {
    return { {s[0], 0, 0}, {0, s[1], 0}, {0, 0, 1} };
  }
  glm::mat3 scale(float s)
  {
    return scale({s,s});
  }

  glm::mat3 scaleRotateTranslate(glm::vec2 s, float r, glm::vec2 t)
  {
    float sx=s[0];
    float sy=s[1];
    float sinr = sin(r);
    float cosr = cos(r);
    float tx=t[0];
    float ty=t[1];
    glm::mat3 m = {
      {sx*cosr,  sx*sinr, 0},
      {-sy*sinr, sy*cosr, 0},
      {tx,       ty,      1} };
    return m;
  }

  glm::mat3 translateRotateScale(glm::vec2 s, float r, glm::vec2 t)
  {
    float sinr = sin(r);
    float cosr = cos(r);
    float sx=s[0];
    float sy=s[1];
    float txsx=t[0] * sx;
    float tysy=t[1] * sy;
    glm::mat3 m = {
      {sx*cosr,             sy*sinr,             0},
      {-sx*sinr,            sy*cosr,             0},
      {txsx*cosr-tysy*sinr, txsx*sinr+tysy*cosr, 1} };
    return m;
  }

  glm::vec2 getTranslation(glm::mat3 m)
  {
    return {m[0][2], m[1][2]};
  }

  glm::mat3 inverse(glm::mat3 m) {
    //m1 = {
    //  {sx*cosr,  sx*sinr, 0},
    //  {-sy*sinr, sy*cosr, 0},
    //  {tx,       ty,      1} };
    //m2 = {
    //  {sx*cosr,             sy*sinr,             0},
    //  {-sx*sinr,            sy*cosr,             0},
    //  {txsx*cosr-tysy*sinr, txsx*sinr+tysy*cosr, 1} };
    float tx=m[2][0];
    float ty=m[2][1];
    m[2][0] = tx*m[0][0]+ty*m[1][0];
    m[2][1] = tx*m[0][1]+ty*m[1][1];
    float sxSinr = m[0][1];
    m[0][1]=-m[1][0];
    m[1][0]=-sxSinr;
    return m;
  }
}
