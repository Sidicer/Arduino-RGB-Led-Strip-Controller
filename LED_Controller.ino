#define BUTTON_Green 4
#define BUTTON_Red 8
#define BUTTON_Blue 7
#define BUTTON_Function 2

#define LED_Red 10
#define LED_Green 9
#define LED_Blue 11

// LED (RGB) Values are stored in here
int LEDS[3] = {0,0,0};

// Rainbow Function Variables
int counter = 0,
    numColors = 255*2,   // The bigger the value - the smoother
    animationDelay = 10;

int rainbowOn = false;

void setup() {
 
  pinMode(BUTTON_Red, INPUT_PULLUP);
  pinMode(BUTTON_Green, INPUT_PULLUP);
  pinMode(BUTTON_Blue, INPUT_PULLUP);
  pinMode(BUTTON_Function, INPUT_PULLUP);
  
  pinMode(LED_Red, OUTPUT);
  pinMode(LED_Green, OUTPUT);
  pinMode(LED_Blue, OUTPUT);

  setColor(LEDS[0], LEDS[1], LEDS[2]);
}

void loop() {

  if(!rainbowOn) setColor(LEDS[0], LEDS[1], LEDS[2]);
  else rainbow();
  
  if(!rainbowOn) {
    // Controlling the RED, GREEN and Blue Values
    // BUTTON_R/G/B pressed                  - values go up,
    // BUTTON_Fuction + BUTTON_R/G/B pressed - values go down
    if ((digitalRead(BUTTON_Function) == HIGH) && (digitalRead(BUTTON_Blue) == HIGH) && (digitalRead(BUTTON_Green) == HIGH) && (digitalRead(BUTTON_Red) == LOW) && (LEDS[0] < 255))
      { LEDS[0]++; delay(15); }
    if ((digitalRead(BUTTON_Function) == LOW) && (digitalRead(BUTTON_Blue) == HIGH) && (digitalRead(BUTTON_Green) == HIGH) && (digitalRead(BUTTON_Red) == LOW) && (LEDS[0] > 0))
      { LEDS[0]--; delay(15); }
    if ((digitalRead(BUTTON_Function) == HIGH) && (digitalRead(BUTTON_Blue) == HIGH) && (digitalRead(BUTTON_Red) == HIGH) && (digitalRead(BUTTON_Green) == LOW) && (LEDS[1] < 255))
      { LEDS[1]++; delay(15); }
    if ((digitalRead(BUTTON_Function) == LOW) && (digitalRead(BUTTON_Blue) == HIGH) && (digitalRead(BUTTON_Red) == HIGH) && (digitalRead(BUTTON_Green) == LOW) && (LEDS[1] > 0))
      { LEDS[1]--; delay(15); }
    if ((digitalRead(BUTTON_Function) == HIGH) && (digitalRead(BUTTON_Red) == HIGH) && (digitalRead(BUTTON_Green) == HIGH) && (digitalRead(BUTTON_Blue) == LOW) && (LEDS[2] < 255))
      { LEDS[2]++; delay(15); }
    if ((digitalRead(BUTTON_Function) == LOW) && (digitalRead(BUTTON_Red) == HIGH) && (digitalRead(BUTTON_Green) == HIGH) && (digitalRead(BUTTON_Blue) == LOW) && (LEDS[2] > 0))
      { LEDS[2]--; delay(15); }
  }
  
    if ((digitalRead(BUTTON_Red) == LOW) && (digitalRead(BUTTON_Green) == LOW) && !rainbowOn) {
      delay(50); 
      if ((digitalRead(BUTTON_Red) == HIGH) && (digitalRead(BUTTON_Green) == HIGH) && !rainbowOn) {
        rainbowOn = true;
      }
    }
    if ((digitalRead(BUTTON_Red) == LOW) && (digitalRead(BUTTON_Green) == LOW) && rainbowOn) { 
      delay(50); 
      if ((digitalRead(BUTTON_Red) == HIGH) && (digitalRead(BUTTON_Green) == HIGH) && rainbowOn) {
        rainbowOn = false;
      }
    }
   
}
  
void setColor (unsigned char red, unsigned char green, unsigned char blue) {
  analogWrite(LED_Red, red);
  analogWrite(LED_Green, green);
  analogWrite(LED_Blue, blue);
}

void rainbow() {

  
  // This part takes care of displaying the
  // color changing in reverse by counting backwards if counter
  // is above the number of available colors
  float colorNumber = counter > numColors ? counter - numColors : counter;

  // Play with the saturation and brightness values
  // to see what they do
  float saturation = 1; // Between 0 and 1 (0 = gray, 1 = full color)
  float brightness = 1; // Between 0 and 1 (0 = dark, 1 is full brightness)
  float hue = (colorNumber / float(numColors)) * 360; // Number between 0 and 360
  long color = HSBtoRGB(hue, saturation, brightness);

  // Get the red, blue and green parts from generated color
  int red = color >> 16 & 255;
  int green = color >> 8 & 255;
  int blue = color & 255;

  setColor(red, green, blue);

  // Counter can never be greater then 2 times the number of available colors
  // the colorNumber = line above takes care of counting backwards (nicely looping animation)
  // when counter is larger then the number of available colors
  counter = (counter + 1) % (numColors * 2);

  // If you uncomment this line the color changing starts from the
  // beginning when it reaches the end (animation only plays forward)
  // counter = (counter + 1) % (numColors);

  delay(animationDelay);
}

long HSBtoRGB(float _hue, float _sat, float _brightness) {
  float red = 0.0;
  float green = 0.0;
  float blue = 0.0;

  if (_sat == 0.0) {
    red = _brightness;
    green = _brightness;
    blue = _brightness;
  } else {
    if (_hue == 360.0) {
      _hue = 0;
    }

    int slice = _hue / 60.0;
    float hue_frac = (_hue / 60.0) - slice;

    float aa = _brightness * (1.0 - _sat);
    float bb = _brightness * (1.0 - _sat * hue_frac);
    float cc = _brightness * (1.0 - _sat * (1.0 - hue_frac));

    switch (slice) {
      case 0:
        red = _brightness;
        green = cc;
        blue = aa;
        break;
      case 1:
        red = bb;
        green = _brightness;
        blue = aa;
        break;
      case 2:
        red = aa;
        green = _brightness;
        blue = cc;
        break;
      case 3:
        red = aa;
        green = bb;
        blue = _brightness;
        break;
      case 4:
        red = cc;
        green = aa;
        blue = _brightness;
        break;
      case 5:
        red = _brightness;
        green = aa;
        blue = bb;
        break;
      default:
        red = 0.0;
        green = 0.0;
        blue = 0.0;
        break;
    }
  }

  long ired = red * 255.0;
  long igreen = green * 255.0;
  long iblue = blue * 255.0;

  return long((ired << 16) | (igreen << 8) | (iblue));
}
