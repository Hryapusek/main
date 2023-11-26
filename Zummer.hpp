class Zummer
{
public:
  Zummer(byte pin);
  void beep();

private:
  byte pin_;
};

Zummer::Zummer(byte pin)
{
  pin_ = pin;
  pinMode(pin_, OUTPUT);
  digitalWrite(pin_, LOW);
}

inline void Zummer::beep()
{
  digitalWrite(pin_, HIGH);
  delay(100);
  digitalWrite(pin_, LOW);
}
