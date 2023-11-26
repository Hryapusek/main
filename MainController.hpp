#define OLED_SOFT_BUFFER_64
#include <GyverOLED.h>
#include <charMap.h>
#include <icons_7x7.h>
#include <icons_8x8.h>

#include "Stoplight.hpp"
#include "Button.hpp"
#include "Barrier.hpp"
#include "Zummer.hpp"

enum class State
{
  WAIT_FOR_BUTTON,
  TIMER
};

class MainController
{
public:
  MainController();
  void loop();

  GyverOLED<SSH1106_128x64> oleg;
private:
  State state_;
  Button button_;
  Stoplight carStoplight_;
  Stoplight humanStoplight_;
  Barrier barrier_;
  Zummer zummer_;
  void wait_for_button();
  void timer();
  void printNumber(int num);
};

const byte buttonPin = 26;
const byte carStoplightGreen = 2;
const byte carStoplightYellow = 3;
const byte carStoplightRed = 4;
const byte barrierPin = 5;
const byte humanStoplightGreen = 6;
const byte humanStoplightYellow = 7;
const byte humanStoplightRed = 8;
const byte zummerPin = 9;

MainController::MainController() :
  button_(Button(buttonPin)),
  carStoplight_(StoplightPins{
    .redPin = carStoplightRed,
    .yellowPin = carStoplightYellow,
    .greenPin = carStoplightGreen
  }),
  humanStoplight_(StoplightPins{
    .redPin = humanStoplightRed,
    .yellowPin = humanStoplightYellow,
    .greenPin = humanStoplightGreen
  }),
  state_(State::WAIT_FOR_BUTTON),
  barrier_(Barrier(barrierPin)),
  zummer_(Zummer(zummerPin))
{
  carStoplight_.setColor(Color::GREEN);
  barrier_.up();
  printNumber(5);
  delay(100);
}

void MainController::loop()
{
  switch(state_)
  {
    case State::WAIT_FOR_BUTTON:
      wait_for_button();
      break;
    
    case State::TIMER:
      timer();
      break;
  }
}

void MainController::wait_for_button()
{
  carStoplight_.setColor(Color::GREEN);
  humanStoplight_.setColor(Color::RED);
  auto isClicked = button_.isClicked();
  Serial.println(isClicked);
  if (isClicked)
  {
    Serial.println("Button is clicked");
    state_ = State::TIMER;
    for (int i = 0; i < 3; ++i)
    {
      carStoplight_.setColor(Color::NONE);
      delay(500);
      carStoplight_.setColor(Color::GREEN);
      delay(500);
    }
    carStoplight_.setColor(Color::YELLOW);
    humanStoplight_.setColor(Color::YELLOW);
    delay(3000);
    humanStoplight_.setColor(Color::GREEN);
    carStoplight_.setColor(Color::RED);
    barrier_.down();
    carStoplight_.startTimer();
  }
}

void MainController::timer()
{
  for (int i = 0; i < Stoplight::interval_/500; i += 1)
  {
    zummer_.beep();
    delay(500);
  }
  if (carStoplight_.isTimeout())
  {
    state_ = State::WAIT_FOR_BUTTON;
    for (int i = 3; i > 0; --i)
    {
      humanStoplight_.setColor(Color::NONE);
      zummer_.beep();
      delay(500);
      humanStoplight_.setColor(Color::GREEN);
      zummer_.beep();
      delay(500);
    }
    humanStoplight_.setColor(Color::YELLOW);
    carStoplight_.setColor(Color::RED_YELLOW);
    delay(3000);
    carStoplight_.setColor(Color::GREEN);
    humanStoplight_.setColor(Color::RED);
    barrier_.up();
  }
}

void MainController::printNumber(int num)
{
  oleg.clear();
  oleg.home();
  oleg.print(String(num));
  oleg.update();
}
