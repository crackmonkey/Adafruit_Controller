# Adafruit_Controller
Library to handle commands sent by the Adafruit BluefruitLE app

Abstracts away the details of the control packets sent by the BluefruitLE app into callback functions that don't clutter up your main loop().

Just add
```
#include <Adafruit_Controller.h>
Adafruit_Controller ctrl = Adafruit_Controller(ble);
```
to the top of your sketch, after configuring the Bluefruit and register callbacks for the controller inputs you want with the handle*() methods, then it's just:
```
ctrl.process();
```
in your loop().
