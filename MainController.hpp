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

private:
  State state_;
  Button button_; // Katya
  Button rubila_; // Misha
  Stoplight carStoplight_; // Danya
  Stoplight humanStoplight_; // Gena
  Barrier barrier_; // Sanya
  Zummer zummer_; // Sanya
  bool isShutdown = false;
  void wait_for_button(); // Katya
  void timer(); // Danya Gena
  void rubilaClicked(); // Sanya
  void shutdown(); // Misha
  void restart(); // Misha
};

const byte buttonPin = 26;
const byte rubilaPin = 28;
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
  zummer_(Zummer(zummerPin)),
  rubila_(Button(rubilaPin))
{
  carStoplight_.setColor(Color::GREEN);
  barrier_.up();
  delay(100);
}

void MainController::loop()
{
  auto rubilaClickedVal = rubila_.isClicked();
  Serial.println("rubila: " + String(rubilaClickedVal));
  if (rubilaClickedVal)
  {
    rubilaClicked();
    return;
  }
  if (isShutdown)
    return;
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
  barrier_.up();
  carStoplight_.setColor(Color::GREEN);
  humanStoplight_.setColor(Color::RED);
  auto isClicked = button_.isClicked();
  Serial.println("btn: " + String(isClicked));
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

void MainController::rubilaClicked()
{
  if (isShutdown)
    restart();
  else
    shutdown();  
}

void MainController::shutdown()
{
  isShutdown = true;
  barrier_.up();
  carStoplight_.setColor(Color::NONE);
  humanStoplight_.setColor(Color::NONE);
}

void MainController::restart()
{
  state_ = State::WAIT_FOR_BUTTON;
  isShutdown = false;
}
