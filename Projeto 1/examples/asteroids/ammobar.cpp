#include "ammobar.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void AmmoBar::initializeGL(GLuint program){
 terminateGL();

  m_program = program;
  m_colorLoc = glGetUniformLocation(m_program, "color");
  //m_rotationLoc = glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = glGetUniformLocation(m_program, "scale");
  //m_translationLoc = glGetUniformLocation(m_program, "translation");

 // m_rotation = 0.0f;
  //m_translation = glm::vec2(0);
  //m_velocity = glm::vec2(0);

  std::array<glm::vec2, 4> positions{
     
      glm::vec2{-120.0f, -120.0f}, glm::vec2{-120.0f, -110.0f},
      glm::vec2{-115.0f, -110.0f}, glm::vec2{-115.0f, -120.0f}

     
  };

  //Normalize
  for (auto &position :positions) {
    position /= glm::vec2{15.5f, 15.5f};
  }

  std::array indices{
                     0, 1, 2,
                     0, 2, 3,
                    };

  // Generate VBO
  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  glGenBuffers(1, &m_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

  // End of binding to current VAO
  glBindVertexArray(0);
}

void AmmoBar::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  glUseProgram(m_program);

  glBindVertexArray(m_vao);

  glUniform1f(m_scaleLoc, m_scale);
  //glUniform1f(m_rotationLoc, m_rotation);
  //glUniform2fv(m_translationLoc, 1, &m_translation.x);

  // Restart thruster blink timer every 100 ms
  //if (m_trailBlinkTimer.elapsed() > 100.0 / 1000.0) m_trailBlinkTimer.restart();

  // if (gameData.m_input[static_cast<size_t>(Input::Up)]) {
  //   // Show thruster trail during 50 ms
  //   if (m_trailBlinkTimer.elapsed() < 50.0 / 1000.0) {
  //     glEnable(GL_BLEND);
  //     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  //     // 50% transparent
  //     glUniform4f(m_colorLoc, 1, 1, 1, 0.5f);

  //     glDrawElements(GL_TRIANGLES, 14 * 3, GL_UNSIGNED_INT, nullptr);

  //     glDisable(GL_BLEND);
  //   }
  // }

  glUniform4fv(m_colorLoc, 1, &m_color.r);
  glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, nullptr);

  glBindVertexArray(0);

  glUseProgram(0);
}

void AmmoBar::terminateGL() {
  glDeleteBuffers(1, &m_vbo);
  glDeleteBuffers(1, &m_ebo);
  glDeleteVertexArrays(1, &m_vao);
}

void AmmoBar::update(const GameData &gameData, float deltaTime) {
 
  if (gameData.m_input[static_cast<size_t>(Input::Fire)] &&
      gameData.m_state == State::Playing) {
   
  }
}