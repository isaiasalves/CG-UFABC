#include "bombs.hpp"
#include "ammobar.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Bombs::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_colorLoc = glGetUniformLocation(m_program, "color");
  m_rotationLoc = glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = glGetUniformLocation(m_program, "scale");
  m_translationLoc = glGetUniformLocation(m_program, "translation");

  m_bombs.clear();

  // Create regular polygon
  auto sides{12};

  std::vector<glm::vec2> positions(0);
  positions.emplace_back(0, 0);
  auto step{M_PI * 2 / sides};
  for (auto angle : iter::range(0.0, M_PI * 2, step)) {
    positions.emplace_back(std::cos(angle), std::sin(angle));
  }
  positions.push_back(positions.at(1));

  // Generate VBO of positions
  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
               positions.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  glGenVertexArrays(1, &m_vao);

  // Bind vertex attributes to current VAO
  glBindVertexArray(m_vao);

  glEnableVertexAttribArray(positionAttribute);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  glBindVertexArray(0);
}

void Bombs::paintGL() {
  glUseProgram(m_program);

  glBindVertexArray(m_vao);
  glUniform4f(m_colorLoc, 1, 0, 0, 1);
  glUniform1f(m_rotationLoc, 0);
  glUniform1f(m_scaleLoc, m_scale);

  for (auto &bomb : m_bombs) {
    glUniform2f(m_translationLoc, bomb.m_translation.x,
                bomb.m_translation.y);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 14);
  }

  glBindVertexArray(0);

  glUseProgram(0);
}

void Bombs::terminateGL() {
  glDeleteBuffers(1, &m_vbo);
  glDeleteVertexArrays(1, &m_vao);
}

void Bombs::update(Ship &ship, const GameData &gameData, float deltaTime, AmmoBar &ammoBar) {
  // Create a pair of bombs
  if (gameData.m_input[static_cast<size_t>(Input::Bomb)] &&
      gameData.m_state == State::Playing) {
    // At least 250 ms must have passed since the last bombs
    if (ship.m_bombCoolDownTimer.elapsed() > 2000.0 / 1000.0 && ammoBar.m_ammunitionCount > 0) {
      ship.m_bombCoolDownTimer.restart();

      // Bombs are shot in the direction of the ship's forward vector
      glm::vec2 forward{glm::rotate(glm::vec2{0.0f, 1.0f}, ship.m_rotation)};
      glm::vec2 right{glm::rotate(glm::vec2{1.0f, 0.0f}, ship.m_rotation)};
      auto cannonOffset{(11.0f / 15.5f) * ship.m_scale};
      auto bombSpeed{0.5f};

      Bomb bomb{.m_dead = false,
                    .m_translation = ship.m_translation + right * cannonOffset,
                    .m_velocity = ship.m_velocity + forward * bombSpeed};
      m_bombs.push_back(bomb);

      bomb.m_translation = ship.m_translation - right * cannonOffset;
      m_bombs.push_back(bomb);

      // Moves ship in the opposite direction
      ship.m_velocity -= forward * 0.5f;

      // Decrease ammo count
      ammoBar.m_ammunitionCount -= 20;
    }
  }

  for (auto &bomb : m_bombs) {
    bomb.m_translation -= ship.m_velocity * deltaTime;
    bomb.m_translation += bomb.m_velocity * deltaTime;

    // Kill bomb if it goes off screen
    if (bomb.m_translation.x < -1.1f) bomb.m_dead = true;
    if (bomb.m_translation.x > +1.1f) bomb.m_dead = true;
    if (bomb.m_translation.y < -1.1f) bomb.m_dead = true;
    if (bomb.m_translation.y > +1.1f) bomb.m_dead = true;
  }

  // Remove dead bombs
  m_bombs.remove_if([](const Bomb &p) { return p.m_dead; });
}