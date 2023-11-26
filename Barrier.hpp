#include <Servo.h>

class Barrier
{
public:
  Barrier(byte pin);
  void up();
  void down();
private:
  Servo servo_;
  byte pin_;
};

Barrier::Barrier(byte pin)
{
  pin_ = pin;
  servo_.attach(pin_);
}

inline void Barrier::up()
{
  servo_.write(90);
}

inline void Barrier::down()
{
  servo_.write(0);
}
