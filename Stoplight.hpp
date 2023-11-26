class StoplightPins
{
public:
  byte redPin;
  byte yellowPin;
  byte greenPin;
};

enum class Color
{
  RED,
  YELLOW,
  GREEN,
  RED_YELLOW,
  NONE
};

class Stoplight
{
public:
  static constexpr uint32_t interval_ = 5000;
  Stoplight(StoplightPins pins);
  void setColor(Color color);
  void startTimer();
  uint32_t getStartTime();
  bool isTimeout();

private:
  StoplightPins pins_;
  Color color_;
  uint32_t startTime_;
};

Stoplight::Stoplight(StoplightPins pins)
{
  pins_ = pins;
  color_ = Color::NONE;
}

void Stoplight::setColor(Color color)
{
  switch(color)
  {
    case Color::NONE:
    {
      analogWrite(pins_.redPin, 0);
      analogWrite(pins_.greenPin, 0);
      analogWrite(pins_.yellowPin, 0);
      break;
    }

    case Color::GREEN:
    {
      analogWrite(pins_.redPin, 0);
      analogWrite(pins_.greenPin, 255);
      analogWrite(pins_.yellowPin, 0);
      break;
    }

    case Color::RED:
    {
      analogWrite(pins_.redPin, 150);
      analogWrite(pins_.greenPin, 0);
      analogWrite(pins_.yellowPin, 0);
      break;
    }

    case Color::YELLOW:
    {
      analogWrite(pins_.redPin, 0);
      analogWrite(pins_.greenPin, 0);
      analogWrite(pins_.yellowPin, 150);
      break;
    }
    
    case Color::RED_YELLOW:
    {
      analogWrite(pins_.redPin, 150);
      analogWrite(pins_.greenPin, 0);
      analogWrite(pins_.yellowPin, 150);
      break;
    }
  }
}

void Stoplight::startTimer()
{
  startTime_ = millis();
}

uint32_t Stoplight::getStartTime()
{
  return startTime_;
}

bool Stoplight::isTimeout()
{
  return (millis() - startTime_) > interval_;
}
