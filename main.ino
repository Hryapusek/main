#include "MainController.hpp"

MainController control;

void setup() {
  Serial.begin(9600);
}

void loop() {
  control.loop();
}
