#include "JNRSegmentDisplay.h"

const uint8_t JNRSegmentDisplay::digitToSegments[10] = {
  0b0111111, 0b0000110, 0b1011011, 0b1001111, 0b1100110,
  0b1101101, 0b1111101, 0b0000111, 0b1111111, 0b1101111
};

uint8_t JNRSegmentDisplay::charToSegment(char c) {
  if (c >= 'a' && c <= 'z') c = c - 'a' + 'A';
  
  switch (c) {
    case '0': return 0b0111111;
    case '1': return 0b0000110;
    case '2': return 0b1011011;
    case '3': return 0b1001111;
    case '4': return 0b1100110;
    case '5': return 0b1101101;
    case '6': return 0b1111101;
    case '7': return 0b0000111;
    case '8': return 0b1111111;
    case '9': return 0b1101111;
    case 'A': return 0b1110111;
    case 'B': return 0b1111100;
    case 'C': return 0b0111001;
    case 'D': return 0b1011110;
    case 'E': return 0b1111001;
    case 'F': return 0b1110001;
    case 'G': return 0b1111101;
    case 'H': return 0b1110110;
    case 'I': return 0b0000110;
    case 'J': return 0b0000100;
    case 'L': return 0b0111000;
    case 'N': return 0b1010101;
    case 'O': return 0b0111111;
    case 'P': return 0b1110011;
    case 'Q': return 0b1110111;
    case 'R': return 0b1010000;
    case 'S': return 0b1101101;
    case 'T': return 0b1111000;
    case 'U': return 0b0111110;
    case 'Y': return 0b1101110;
    case '-': return 0b1000000;
    case ' ': return 0b0000000;
    case '_': return 0b0001000;
    default: return 0b0000000;
  }
}

JNRSegmentDisplay::LCDSegmentDisplay(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4, uint8_t p5) {
  pins[0] = p1;
  pins[1] = p2;
  pins[2] = p3;
  pins[3] = p4;
  pins[4] = p5;
  
  digitTens[0] = {pins[2], pins[1]};
  digitTens[1] = {pins[1], pins[2]};
  digitTens[2] = {pins[2], pins[3]};
  digitTens[3] = {pins[1], pins[3]};
  digitTens[4] = {pins[1], pins[4]};
  digitTens[5] = {pins[2], pins[4]};
  digitTens[6] = {pins[3], pins[4]};
  
  digitUnits[0] = {pins[1], pins[0]};
  digitUnits[1] = {pins[0], pins[1]};
  digitUnits[2] = {pins[2], pins[0]};
  digitUnits[3] = {pins[0], pins[2]};
  digitUnits[4] = {pins[3], pins[0]};
  digitUnits[5] = {pins[0], pins[3]};
  digitUnits[6] = {pins[0], pins[4]};
  
  segHundredTop = {pins[3], pins[2]};
  segHundredBottom = {pins[3], pins[1]};
  segPercent = {pins[4], pins[2]};
  segBattery = {pins[4], pins[0]};
}

void JNRSegmentDisplay::begin() { 
  allPinsHiZ(); 
  setValue(0); 
}

void JNRSegmentDisplay::setDisplay(char tens, char units) {
  tensChar = tens;
  unitsChar = units;
  useLetterMode = true;
  displayValue = -1;
  buildFrame();
  currentSegIndex = 0;
}

void JNRSegmentDisplay::setLetter(char letter) {
  setDisplay(tensChar, letter);
}

void JNRSegmentDisplay::allPinsHiZ() {
  for (uint8_t i = 0; i < 5; i++) {
    pinMode(pins[i], INPUT);
  }
}

void JNRSegmentDisplay::addSegmentsForDigit(SegPair* digitTable, uint8_t segMask) {
  for (uint8_t b = 0; b < 7; b++) {
    if (segMask & (1 << b)) {
      if (activeFrameCount < MAX_ACTIVE_SEGMENTS) {
        activeFrame[activeFrameCount++] = digitTable[b];
      }
    }
  }
}

void JNRSegmentDisplay::buildFrame() {
  activeFrameCount = 0;
  
  if (useLetterMode) {
    uint8_t tensMask = charToSegment(tensChar);
    uint8_t unitsMask = charToSegment(unitsChar);
    
    if (tensChar == '1' && unitsChar == '0') {
      activeFrame[activeFrameCount++] = segHundredTop;
      activeFrame[activeFrameCount++] = segHundredBottom;
      addSegmentsForDigit(digitTens, digitToSegments[0]);
      addSegmentsForDigit(digitUnits, digitToSegments[0]);
    } else {
      if (tensMask != 0) {
        addSegmentsForDigit(digitTens, tensMask);
      }
      if (unitsMask != 0) {
        addSegmentsForDigit(digitUnits, unitsMask);
      }
    }
  } else {
    int v = constrain(displayValue, 0, 100);
    tensChar = '0' + (v / 10);
    unitsChar = '0' + (v % 10);
    
    if (v == 100) {
      activeFrame[activeFrameCount++] = segHundredTop;
      activeFrame[activeFrameCount++] = segHundredBottom;
      addSegmentsForDigit(digitTens, digitToSegments[0]);
      addSegmentsForDigit(digitUnits, digitToSegments[0]);
    } else {
      addSegmentsForDigit(digitTens, digitToSegments[v / 10]);
      addSegmentsForDigit(digitUnits, digitToSegments[v % 10]);
    }
  }
  
  if (showPercent) {
    activeFrame[activeFrameCount++] = segPercent;
  }
  if (showBattery) {
    activeFrame[activeFrameCount++] = segBattery;
  }
}

void JNRSegmentDisplay::setValue(int v) {
  displayValue = constrain(v, 0, 100);
  useLetterMode = false;
  buildFrame();
  currentSegIndex = 0;
}

int JNRSegmentDisplay::getValue() const { return displayValue; }
char JNRSegmentDisplay::getTensChar() const { return tensChar; }
char JNRSegmentDisplay::getUnitsChar() const { return unitsChar; }

void JNRSegmentDisplay::setShowPercent(bool enable) {
  showPercent = enable;
  buildFrame();
}

void JNRSegmentDisplay::setShowBattery(bool enable) {
  showBattery = enable;
  buildFrame();
}

void JNRSegmentDisplay::setSegmentOnTime(uint16_t us) { segmentOnTime = us; }
void JNRSegmentDisplay::setPolarityInterval(uint16_t ms) { polarityInterval = ms; }
void JNRSegmentDisplay::enablePolarityInversion(bool enable) { polarityInversionEnabled = enable; }

void JNRSegmentDisplay::update() {
  if (activeFrameCount == 0) return;
  
  if (polarityInversionEnabled && (millis() - lastPolarityToggle > polarityInterval)) {
    lastPolarityToggle = millis();
    framePolarity = !framePolarity;
  }
  
  SegPair current = activeFrame[currentSegIndex];
  allPinsHiZ();
  
  uint8_t highPin = framePolarity ? current.com : current.seg;
  uint8_t lowPin = framePolarity ? current.seg : current.com;
  
  pinMode(highPin, OUTPUT);
  pinMode(lowPin, OUTPUT);
  digitalWrite(highPin, HIGH);
  digitalWrite(lowPin, LOW);
  delayMicroseconds(segmentOnTime);
  
  currentSegIndex = (currentSegIndex + 1) % activeFrameCount;
}
