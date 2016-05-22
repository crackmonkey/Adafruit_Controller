#include <Adafruit_BLE.h>
#include <Adafruit_BluefruitLE_SPI.h>
#include <Adafruit_Controller.h>

// connect to the BlueFruit according to the documentation
#define BLUEFRUIT_SPI_CS               8
#define BLUEFRUIT_SPI_IRQ              7
#define BLUEFRUIT_SPI_RST              4    // Optional but recommended, set to -1 if unused
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

// Bind a controller to the BlueFruit
Adafruit_Controller ctrl = Adafruit_Controller(ble);

void setup() {
  Serial.begin(0);

  // initialized the BlueFruit and kick it into data mode
  ble.begin(false);
  ble.setMode(BLUEFRUIT_MODE_DATA);

  // register callbacks 
  ctrl.handleColor(doColor);
  ctrl.handleButton(doButton);
}

void loop() {
  // ctrl.process() reads input from the Bluefruit and
  // calls the callback functions then returns true when 
  // it has a complete packet
  if (ctrl.process()) {
    Serial.println("Found a packet");
    // printPacket shows the details of the packet that just completed
    // sending the output to Serial
    ctrl.printPacket(Serial);
  }
  
}

void doButton(byte btn, bool state)
{
  Serial.print("Button ");
  Serial.print(btn);
  Serial.print(" ");
  Serial.println(state ? "Pressed" : "Released");
}
void doColor(byte r, byte g, byte b)
{
  Serial.print("got a color!");
  Serial.print(r,HEX);
  Serial.print(g,HEX);
  Serial.print(b,HEX);
  Serial.println();
}
