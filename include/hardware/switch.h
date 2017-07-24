#pragma once
#include <stdint.h>

namespace hardware {


class Switch {
public:
  Switch(uint8_t pin, uint8_t pressedState);
  bool get() const;
  bool pressed() const;
  bool released() const;
  void tick();

private:
  uint8_t m_pin;
  bool m_pressedState;
  bool m_prevState;
  bool m_pressEvent;
  bool m_releaseEvent;
};

}