#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { Right, Left, Down, Up, Fire , Bomb};
enum class State { Playing, GameOver, Win };

struct GameData {
  State m_state{State::Playing};
  std::bitset<6> m_input;  // [fire, up, down, left, right]
};

#endif