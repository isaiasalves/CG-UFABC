#ifndef AMMOBAR_HPP_
#define AMMOBAR_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

class Asteroids;
class Bullets;
class OpenGLWindow;
class StarLayers;

class OpenGLWindow;

class AmmoBar {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();

  void update(const GameData &gameData, float deltaTime);
  //void setRotation(float rotation) { m_rotation = rotation; }

 private:
  friend OpenGLWindow;
  friend Bullets;

  GLuint m_program{};
  //GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};
  //GLint m_rotationLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};
  GLuint m_ebo{};

  int m_ammunitionCount = 40;
  int m_decrease = 2;

  glm::vec4 m_color{1};
  //float m_rotation{};
  float m_scale{0.125f};
  //glm::vec2 m_translation{glm::vec2(0)};
  //glm::vec2 m_velocity{glm::vec2(0)};

  // abcg::ElapsedTimer m_trailBlinkTimer;
  // abcg::ElapsedTimer m_bulletCoolDownTimer;
};

#endif