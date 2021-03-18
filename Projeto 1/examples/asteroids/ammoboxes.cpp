#include "ammoboxes.hpp"
#include "ammoboxes.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

void AmmoBoxes::initializeGL(GLuint program, int quantity) {
  terminateGL();

  // Start pseudo-random number generator
  auto seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);

  m_program = program;
  m_colorLoc = glGetUniformLocation(m_program, "color");
  m_rotationLoc = glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = glGetUniformLocation(m_program, "scale");
  m_translationLoc = glGetUniformLocation(m_program, "translation");

  // Create ammoboxes
  m_ammoboxes.clear();
  m_ammoboxes.resize(quantity);

  for (auto &ammobox : m_ammoboxes) {
    ammobox = createAmmobox();

    // Make sure the ammobox won't collide with the ship
    do {
      ammobox.m_translation = {m_randomDist(m_randomEngine),
                                m_randomDist(m_randomEngine)};
    } while (glm::length(ammobox.m_translation) < 0.5f);
  }
}

void AmmoBoxes::paintGL() {
  glUseProgram(m_program);

  for (auto &ammobox : m_ammoboxes) {
    glBindVertexArray(ammobox.m_vao);

    glUniform4fv(m_colorLoc, 1, &ammobox.m_color.r);
    glUniform1f(m_scaleLoc, ammobox.m_scale);
    glUniform1f(m_rotationLoc, ammobox.m_rotation);

    for (auto i : {-2, 0, 2}) {
      for (auto j : {-2, 0, 2}) {
        glUniform2f(m_translationLoc, ammobox.m_translation.x + j,
                    ammobox.m_translation.y + i);

        glDrawArrays(GL_TRIANGLE_FAN, 0, ammobox.m_polygonSides + 2);
      }
    }

    glBindVertexArray(0);
  }

  glUseProgram(0);
}

void AmmoBoxes::terminateGL() {
  for (auto ammobox : m_ammoboxes) {
    glDeleteBuffers(1, &ammobox.m_vbo);
    glDeleteVertexArrays(1, &ammobox.m_vao);
  }
}

void AmmoBoxes::update(const Ship &ship, float deltaTime) {
  for (auto &ammobox : m_ammoboxes) {
    ammobox.m_translation -= ship.m_velocity * deltaTime;
    ammobox.m_rotation = glm::wrapAngle(
        ammobox.m_rotation + ammobox.m_angularVelocity * deltaTime);
    ammobox.m_translation += ammobox.m_velocity * deltaTime;

    // Wrap-around
    if (ammobox.m_translation.x < -1.0f) ammobox.m_translation.x += 2.0f;
    if (ammobox.m_translation.x > +1.0f) ammobox.m_translation.x -= 2.0f;
    if (ammobox.m_translation.y < -1.0f) ammobox.m_translation.y += 2.0f;
    if (ammobox.m_translation.y > +1.0f) ammobox.m_translation.y -= 2.0f;
  }
}

AmmoBoxes::AmmoBox AmmoBoxes::createAmmobox(glm::vec2 translation,
                                              float scale) {
  AmmoBox ammobox;

  auto &re{m_randomEngine};  // Shortcut

  // Randomly choose the number of sides
  std::uniform_int_distribution<int> randomSides(6, 20);
  ammobox.m_polygonSides = randomSides(re);

  // Choose a random color (actually, a grayscale)
  std::uniform_real_distribution<float> randomIntensity(0.5f, 1.0f);
  ammobox.m_color = glm::vec4(1) * randomIntensity(re);

  ammobox.m_color.a = 13.0f;
  ammobox.m_rotation = 0.0f;
  ammobox.m_scale = scale;
  ammobox.m_translation = translation;

  // Choose a random angular velocity
  ammobox.m_angularVelocity = m_randomDist(re);

  // Choose a random direction
  glm::vec2 direction{m_randomDist(re), m_randomDist(re)};
  ammobox.m_velocity = glm::normalize(direction) / 7.0f;

  // Create geometry
  std::vector<glm::vec2> positions(0);
  positions.emplace_back(0, 0);
  auto step{M_PI * 2 / ammobox.m_polygonSides};
  std::uniform_real_distribution<float> randomRadius(0.8f, 1.0f);
  for (auto angle : iter::range(0.0, M_PI * 2, step)) {
    auto radius{randomRadius(re)};
    positions.emplace_back(radius * std::cos(angle), radius * std::sin(angle));
  }
  positions.push_back(positions.at(1));

  // Generate VBO
  glGenBuffers(1, &ammobox.m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, ammobox.m_vbo);
  glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
               positions.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  glGenVertexArrays(1, &ammobox.m_vao);

  // Bind vertex attributes to current VAO
  glBindVertexArray(ammobox.m_vao);

  glBindBuffer(GL_ARRAY_BUFFER, ammobox.m_vbo);
  glEnableVertexAttribArray(positionAttribute);
  glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  glBindVertexArray(0);

  return ammobox;
}