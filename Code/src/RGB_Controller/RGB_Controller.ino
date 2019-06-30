#include <Adafruit_NeoPixel.h>
//https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#LEDStripEffectMultiColorBouncingBalls

#define BUTTON_PIN 3
#define PIXEL_PIN 4
#define PIXEL_COUNT 9
#define PAUSE_TIME 40
                                      //amount,pin
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

bool oldState = HIGH;
int showType = -1;
int counter = 0;


void setup() {
  pinMode(BUTTON_PIN,INPUT);
  strip.setBrightness(100);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  bool newState = digitalRead(BUTTON_PIN);

  if(newState == LOW && oldState == HIGH){
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newState = digitalRead(BUTTON_PIN);

    if (newState == LOW) {
      showType++;
      counter = PAUSE_TIME;
      if (showType > 5)//show amount!
        showType=0;
    }
  }
  else if(newState == LOW && oldState == LOW){
    counter ++;
    delay(1);
    if(counter >= PAUSE_TIME){
        counter = 0;
        startShow(showType);
    }
  }

  oldState = newState;
}


void startShow(int i) {
    switch(i){
      case 0:
            fadeInOut(0xFF,0x4A,00,5);
              fadeInOut(0x76,0x00,0xFF,5);
              break;
      case 1: theaterChase(strip.Color(0, 0x76, 0xCF), 75); // White
              break;
      case 2:
          fadeInOut(0xFF,0,0,3);
          fadeInOut(255,255,255,3);
          fadeInOut(0,0x00,0xFF,3);
              break;
      case 3: dragon();
              break;
      case 4: ringFade();
              break;
      case 5: theaterChaseRainbow(60);
              break;

    }
}



void ringFade(){
  uint16_t index = 0;
  for(uint16_t k = 0; k < 12; k++){
    for(uint16_t i = 0; i < strip.numPixels(); i++){
          uint16_t r = 0;
          uint16_t g = 0;
          uint16_t b = 255;

      for(uint16_t j = 0; j < strip.numPixels();j++){
        index = j + i;
        if(index > strip.numPixels() - 1) index -= strip.numPixels();


         b = 255 - (uint16_t)(255/strip.numPixels()) * j;
         r = (uint16_t)(255/strip.numPixels()) * j;

         strip.setPixelColor(index,r,g,b);
         strip.show();

      }
      delay(50);
    }
  }

}






void dragon(){
  uint16_t index = 0;
  uint16_t r = 0;
  uint16_t g = 0;
  uint16_t b = 0;
  uint16_t c = 0;
  for(uint16_t k = 0; k < strip.numPixels() - 1; k++){
    for(uint16_t i = 0; i < strip.numPixels(); i++){
      for(uint16_t j = 0; j < strip.numPixels();j++){
        index = j + i;
        if(index > strip.numPixels() - 1) index -= strip.numPixels();

         if(j > k){
          c = 0;
          r = 0;
          g = 0;
          b = 0;
         }
         else {
          c = 140;
          r = 140;
          g = j * 22;
          b = 0;
         }

         strip.setPixelColor(index,r,g,b);
         strip.show();

      }

      delay(map(strip.numPixels()-k,0,strip.numPixels() - 1,20,60));
    }
  }

}


void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      //strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}
//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<1; j++) {  //do 1 cycles of chasing

    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }

    }
  }
}

void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void fadeInOut(byte red, byte green, byte blue,int wait){
  float r, g, b;
  for(int k = 0; k < 256; k=k+1) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    strip.fill(strip.Color(r,g,b),0,9);

    strip.show();

    delay(wait);
  }

  for(int k = 255; k >= 0; k=k-2) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    strip.fill(strip.Color(r,g,b),0,9);
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
