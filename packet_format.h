
typedef union  __attribute__ ((__packed__)) {
  struct {
    // pad 2 bytes to the float values at index 2,6,10,14 are 32-bit aligned
    // and usable directly from the structure
    char pad[2]; 
    char SOD;
    char action;
    union {
      struct { // C olor
        uint8_t red;
        uint8_t green;
        uint8_t blue;
      } color;
      struct { // B utton
        uint8_t button;
        uint8_t state;
      } button;
      struct { // L ocation
        float lat;
        float lon;
        float alt;
      } location;
      struct { // A ccelerometer
        float x;
        float y;
        float z;
      } accel;
      struct { // M agnetometer
        float x;
        float y;
        float z;
      } mag;
      struct { // G yroscope
        float x;
        float y;
        float z;
      } gyro;
      struct { // Q uaternions
        float x;
        float y;
        float z;
        float w;
      } quat;
    } type;
  } pkt;
  uint8_t buffer[23];
} packet_t;
