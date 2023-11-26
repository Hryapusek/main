#include "MainController.hpp"

MainController control;

void setup() {
  Serial.begin(9600);
  control.oleg.init();           // Инциализация дисплея
  control.oleg.setContrast(255); // Макс. яркость
}

void loop() {
  control.loop();
}
