// Based on Peter Knight's Arduino Granular Synth project 
// https://github.com/ndonegan/auduino/blob/master/auduino.pde
#include <avr/io.h>
#include <avr/interrupt.h>
// Define Analogue Inputs
#define SYNC_CONTROL         (4) // Sync pot pin
#define GRAIN_FREQ_CONTROL   (1) // Freq1 pot pin
#define GRAIN_DECAY_CONTROL  (0) // Decay1 pot pin
#define GRAIN2_FREQ_CONTROL  (3) // Freq2 pot pin
#define GRAIN2_DECAY_CONTROL (2) // Decay2 pot
#define LEFT_PROX_SENSOR     (6) // Left proximity sensor pin 
#define RIGHT_PROX_SENSOR    (7) // Right proximity sensor pin 
//Define Digital IOs
#define TOGGLE_DOWN_PIN      (2) // toggle switch down pin 
#define TOGGLE_UP_PIN        (4) // toggle switch up pin 
#define MODE_BTN_PIN         (6) // push button pin 
#define SPH_LED_PIN          (10)// SPH LED pin 
#define FRQ_LED_PIN          (8) // FRQ LED pin 
#define MAP_LED_PIN          (9) // MAP LED pin  
// Define audio values for Arduino Mega
#define LED_PIN       13
#define LED_PORT      PORTB
#define LED_BIT       7
#define PWM_PIN       3 
#define PWM_VALUE     OCR3C
#define PWM_INTERRUPT TIMER3_OVF_vect

uint16_t syncPhaseAcc;
uint16_t syncPhaseInc;
uint16_t grainPhaseAcc;
uint16_t grainPhaseInc;
uint16_t grainAmp;
uint8_t grainDecay;
uint16_t grain2PhaseAcc;
uint16_t grain2PhaseInc;
uint16_t grain2Amp;
uint8_t grain2Decay;
volatile uint16_t mapLEDState = LOW;   
long previousMillis = 0;       
uint16_t BlinkRate = 5;           
uint16_t mapLEDBlinkCount = 0;          
uint16_t mapLEDBlinkLoopLength = 14;    
uint16_t syncValue = 0;
uint16_t freq1Value = 0;
uint16_t freq2Value = 0;
uint16_t toggleDownValue = 0;
uint16_t toggleUpValue = 0;
uint16_t btnModeValue;          
uint16_t buttonState;         
uint16_t leftPSValue = 0;     
uint16_t rightPSValue = 0;    
uint16_t psMin = 40;           // minimum proximity sensor value
uint16_t psMax = 620;         // maximum proximity sensor value
uint16_t mapMode = 0;        
//Mode 0:Smooth Logarithmic mapping
//Mode 1:Chromatic mapping
//Mode 2:Major Diatonic mapping
//Mode 3:Minor Diatonic mapping
//Mode 4:Pentatonic mapping
//Mode 5:Major Pentatonic mapping
//Mode 6:Minor Pentatonic mapping

void setup() {
  pinMode(SPH_LED_PIN, OUTPUT);
  pinMode(FRQ_LED_PIN, OUTPUT);   
  pinMode(MAP_LED_PIN, OUTPUT);
  digitalWrite(SPH_LED_PIN, HIGH);
  digitalWrite(FRQ_LED_PIN, HIGH);
  digitalWrite(MAP_LED_PIN, HIGH);
  pinMode(PWM_PIN, OUTPUT);
  audioOn();
  pinMode(TOGGLE_DOWN_PIN, INPUT_PULLUP);
  pinMode(TOGGLE_UP_PIN, INPUT_PULLUP);
  pinMode(MODE_BTN_PIN, INPUT_PULLUP);
  digitalWrite(SPH_LED_PIN, LOW);
  digitalWrite(FRQ_LED_PIN, LOW);
  digitalWrite(MAP_LED_PIN, LOW);
}

void loop() {
  toggleDownValue = digitalRead(TOGGLE_DOWN_PIN); 
  toggleUpValue   = digitalRead(TOGGLE_UP_PIN); 
  btnModeValue    = digitalRead(MODE_BTN_PIN); 
  if (btnModeValue != buttonState) { 
    if(btnModeValue == 0){
      mapMode++; 
      if(mapMode > 6) 
        mapMode = 0; 
    }
    buttonState = btnModeValue;   
  }
  if (millis() - previousMillis > 1000/BlinkRate){
    mapLEDBlinkCount ++;
    previousMillis = millis();
    digitalWrite(MAP_LED_PIN, LOW);
    if (mapLEDBlinkCount <= (mapMode+1)*2) {
      mapLEDState = !mapLEDState;
      digitalWrite(MAP_LED_PIN, mapLEDState);
    } else if (mapLEDBlinkCount >= mapLEDBlinkLoopLength) {
      mapLEDBlinkCount = 0;   
    }
  }
  if(toggleDownValue == 0){
    leftPSValue  = analogRead(LEFT_PROX_SENSOR);
    rightPSValue = analogRead(RIGHT_PROX_SENSOR);
    freq2Value   = analogRead(GRAIN2_FREQ_CONTROL);
    analogWrite(SPH_LED_PIN, (leftPSValue+rightPSValue)/2);
    leftPSValue  = constrain(map(leftPSValue, psMin, psMax, 512, 1023), 512, 1023);
    rightPSValue = constrain(map(rightPSValue, psMin, psMax, 256, 1023), 256, 1023);
    syncValue    = rightPSValue;
    freq1Value   = 1024 - leftPSValue;
  }else if(toggleUpValue == 0){
    leftPSValue  = analogRead(LEFT_PROX_SENSOR);
    rightPSValue = analogRead(RIGHT_PROX_SENSOR);
    syncValue    = analogRead(SYNC_CONTROL);
    analogWrite(SPH_LED_PIN, (leftPSValue+rightPSValue)/2);
    leftPSValue  = constrain(map(leftPSValue, psMin, psMax, 512, 1023), 512, 1023);
    rightPSValue = constrain(map(rightPSValue, psMin, psMax, 512, 1023), 512, 1023);
    freq2Value   = 1024 - leftPSValue;
    freq1Value   = 1024 - rightPSValue;
  }else {
    syncValue    = analogRead(SYNC_CONTROL);
    freq2Value   = analogRead(GRAIN2_FREQ_CONTROL);
    freq1Value   = analogRead(GRAIN_FREQ_CONTROL);
    digitalWrite(SPH_LED_PIN, LOW);
  }
  switch (mapMode) {
    case 0:
      syncPhaseInc = MapPhaseInc(syncValue) / 4;
      break;
    case 1:
      syncPhaseInc = MapMidi(syncValue);
      break;
    case 2:
      syncPhaseInc = MapMajorDiatonic(syncValue);
      break;
    case 3:
      syncPhaseInc = MapMinorDiatonic(syncValue);
      break;
    case 4:
      syncPhaseInc = MapPentatonic(syncValue);
      break;
    case 5:
      syncPhaseInc = MapMajorPentatonic(syncValue);
      break;
    case 6:
      syncPhaseInc = MapMinorPentatonic(syncValue);
      break;
  }
  grainPhaseInc  = MapPhaseInc(freq1Value) / 2;
  grainDecay     = analogRead(GRAIN_DECAY_CONTROL) / 8;
  grain2PhaseInc = MapPhaseInc(freq2Value) / 2;
  grain2Decay    = analogRead(GRAIN2_DECAY_CONTROL) / 4;
  digitalWrite(FRQ_LED_PIN, PWM_VALUE);
}

SIGNAL(PWM_INTERRUPT){
  uint8_t value;
  uint16_t output;

  syncPhaseAcc += syncPhaseInc;
  if (syncPhaseAcc < syncPhaseInc) {
    // Time to start the next grain
    grainPhaseAcc = 0;
    grainAmp = 0x7fff;
    grain2PhaseAcc = 0;
    grain2Amp = 0x7fff;
    LED_PORT ^= 1 << LED_BIT; // Faster than using digitalWrite
  }

  // Increment the phase of the grain oscillators
  grainPhaseAcc += grainPhaseInc;
  grain2PhaseAcc += grain2PhaseInc;

  // Convert phase into a triangle wave
  value = (grainPhaseAcc >> 7) & 0xff;
  if (grainPhaseAcc & 0x8000) value = ~value;
  // Multiply by current grain amplitude to get sample
  output = value * (grainAmp >> 8);

  // Repeat for second grain
  value = (grain2PhaseAcc >> 7) & 0xff;
  if (grain2PhaseAcc & 0x8000) value = ~value;
  output += value * (grain2Amp >> 8);

  // Make the grain amplitudes decay by a factor every sample (exponential decay)
  grainAmp -= (grainAmp >> 8) * grainDecay;
  grain2Amp -= (grain2Amp >> 8) * grain2Decay;

  // Scale output to the available range, clipping if necessary
  output >>= 9;
  if (output > 255) output = 255;

  // Output to PWM (this is faster than using analogWrite)  
  PWM_VALUE = output;
}

void audioOn() {
  //ARDUINO MEGA Compatible
  TCCR3A = _BV(COM3C1) | _BV(WGM30);
  TCCR3B = _BV(CS30);
  TIMSK3 = _BV(TOIE3);
}

// MAPPINGS - START
// Smooth logarithmic mapping
uint16_t antilogTable[] = {
  64830,64132,63441,62757,62081,61413,60751,60097,59449,58809,58176,57549,56929,56316,55709,55109,
  54515,53928,53347,52773,52204,51642,51085,50535,49991,49452,48920,48393,47871,47356,46846,46341,
  45842,45348,44859,44376,43898,43425,42958,42495,42037,41584,41136,40693,40255,39821,39392,38968,
  38548,38133,37722,37316,36914,36516,36123,35734,35349,34968,34591,34219,33850,33486,33125,32768
};
uint16_t MapPhaseInc(uint16_t input) {
  return (antilogTable[input & 0x3f]) >> (input >> 6);
}

// Stepped chromatic mapping
uint16_t midiTable[] = {
  0,17,18,19,20,22,23,24,26,27,29,31,32,34,36,38,41,43,46,48,51,54,58,61,65,69,73,
  77,82,86,92,97,103,109,115,122,129,137,145,154,163,173,183,194,206,218,231,
  244,259,274,291,308,326,346,366,388,411,435,461,489,518,549,581,616,652,691,
  732,776,822,871,923,978,1036,1097,1163,1232,1305,1383,1465,1552,1644,1742,
  1845,1955,2071,2195,2325,2463,2610,2765,2930,3104,3288,3484,3691,3910,4143,
  4389,4650,4927,5220,5530,5859,6207,6577,6968,7382,7821,8286,8779,9301,9854,
  10440,11060,11718,12415,13153,13935,14764,15642,16572,17557,18601,19708,20879,
  22121,23436,24830,26306,27871
};
uint16_t MapMidi(uint16_t input) {
  return (midiTable[(1023-input) >> 3]);
}

// Stepped Pentatonic mapping
uint16_t pentatonicTable[54] = {
  0,19,22,26,29,32,38,43,51,58,65,77,86,103,115,129,154,173,206,231,259,308,346,
  411,461,518,616,691,822,923,1036,1232,1383,1644,1845,2071,2463,2765,3288,
  3691,4143,4927,5530,6577,7382,8286,9854,11060,13153,14764,16572,19708,22121,26306
};
uint16_t MapPentatonic(uint16_t input) {
  uint8_t value = (1023-input) / (1024/53);
  return (pentatonicTable[value]);
}

// Stepped major Diatonic mapping
uint16_t majorDiatonicTable[76] = {
  0,17,19,22,23,26,29,32,34,38,43,46,51,58,65,69,77,86,92,103,115,129,137,154,173,183,206,231,259,274,308,346,366,
  411,461,518,549,616,691,732,822,923,1036,1097,1232,1383,1465,1644,1845,2071,2195,2463,2765,2930,3288,
  3691,4143,4389,4927,5530,5859,6577,7382,8286,8779,9854,11060,11718,13153,14764,16572,17557,19708,22121,23436,26306
};
uint16_t MapMajorDiatonic(uint16_t input) {
  uint8_t value = (1023-input) / (1024/53);
  return (majorDiatonicTable[value]);
}

// Stepped minor Diatonic mapping
uint16_t minorDiatonicTable[76] = {
  0,17,19,20,23,26,27,31,34,38,41,46,51,54,61,69,77,82,92,103,109,122,137,154,163,183,206,218,244,274,308,326,366,
  411,435,489,549,616,652,732,822,871,978,1097,1232,1305,1465,1644,1742,1955,2195,2463,2610,2930,3288,
  3484,3910,4389,4927,5220,5859,6577,6968,7821,8779,9854,10440,11718,13153,13935,15642,17557,19708,20879,23436,26306
};
uint16_t MapMinorDiatonic(uint16_t input) {
  uint8_t value = (1023-input) / (1024/53);
  return (minorDiatonicTable[value]);
}

// Stepped major Pentatonic mapping
uint16_t majorPentatonicTable[55] = {
  0,17,19,22,26,29,34,38,43,51,58,69,77,86,103,115,137,154,173,206,231,274,308,346,
  411,461,549,616,691,822,923,1097,1232,1383,1644,1845,2195,2463,2765,3288,
  3691,4389,4927,5530,6577,7382,8779,9854,11060,13153,14764,17557,19708,22121,26306
};
uint16_t MapMajorPentatonic(uint16_t input) {
  uint8_t value = (1023-input) / (1024/53);
  return (majorPentatonicTable[value]);
}

// Stepped minor Pentatonic mapping
uint16_t minorPentatonicTable[55] = {
  0,17,20,23,26,31,34,41,46,51,61,69,82,92,103,122,137,163,183,206,244,274,326,366,
  411,489,549,652,732,822,978,1097,1305,1465,1644,1955,2195,2610,2930,3288,
  3910,4389,5220,5859,6577,7821,8779,10440,11718,13153,15642,17557,20879,23436,26306
};
uint16_t MapMinorPentatonic(uint16_t input) {
  uint8_t value = (1023-input) / (1024/53);
  return (minorPentatonicTable[value]);
}
// MAPPINGS - END
