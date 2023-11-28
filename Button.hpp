class Button
{
public:
  // Digital
  Button(byte pin);
  bool isClicked();

private:
  byte pin_;
};

Button::Button(byte pin)
{
  pin_ = pin;
  pinMode(pin_, INPUT);
}

bool Button::isClicked()
{
  bool res = true;
  for (int i = 0; i < 100; ++i)
  {
    res = res && (digitalRead(pin_) == HIGH);
    delay(10);
  }
  return res;
}
