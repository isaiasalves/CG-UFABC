#ifndef BOMBS_HPP_
#define BOMBS_HPP_

#include <list>

#include "abcg.hpp"
#include "gamedata.hpp"
#include "ship.hpp"
#include "ammobar.hpp"

class OpenGLWindow;

class Bombs {
 public:
  void initializeGL(GLuint program);
  void paintGL();
  void terminateGL();

  void update(Ship &ship, const GameData &gameData, float deltaTime, AmmoBar &ammoBar);

 private:
  friend OpenGLWindow;

  GLuint m_program{};
  GLint m_colorLoc{};
  GLint m_rotationLoc{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};

  struct Bomb {
    bool m_dead{false};
    glm::vec2 m_translation{glm::vec2(0)};
    glm::vec2 m_velocity{glm::vec2(0)};
  };

  float m_scale{0.030f};

  std::list<Bomb> m_bombs;
};

#endif