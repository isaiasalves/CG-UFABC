#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>

#include "abcg.hpp"
#include "asteroids.hpp"
#include "bullets.hpp"
#include "bombs.hpp"
#include "ship.hpp"
#include "starlayers.hpp"
#include "ammobar.hpp"
#include "ammoboxes.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& event) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  GLuint m_ammoBarProgram{};
  GLuint m_starsProgram{};
  GLuint m_objectsProgram{};

  int m_viewportWidth{};
  int m_viewportHeight{};
  int m_ammoBoxesCount = 2;

  GameData m_gameData;

  Asteroids m_asteroids;
  Bullets m_bullets;
  Bombs m_bombs;
  Ship m_ship;
  StarLayers m_starLayers;
  AmmoBar m_ammoBar;
  AmmoBoxes m_ammoBoxes;

  abcg::ElapsedTimer m_restartWaitTimer;

  ImFont* m_font{};

  std::default_random_engine m_randomEngine;

  void restart();
  void update();
  void checkCollisions();
  void checkWinCondition();
};

#endif
