#ifndef JNR_SEGMENT_DISPLAY_H
#define JNR_SEGMENT_DISPLAY_H

#include <Arduino.h>

struct SegPair {
  uint8_t seg;
  uint8_t com;
};

class JNRSegmentDisplay {
public:
  JNRSegmentDisplay(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4, uint8_t p5);
  
  void begin();
  void setValue(int value);
  void setDisplay(char tens, char units);
  void setLetter(char letter);
  void setShowPercent(bool enable);
  void setShowBattery(bool enable);
  void update();
  void setSegmentOnTime(uint16_t us);
  void setPolarityInterval(uint16_t ms);
  void enablePolarityInversion(bool enable);
  int getValue() const;
  char getTensChar() const;
  char getUnitsChar() const;

private:
  uint8_t pins[5];
  
  SegPair digitTens[7];
  SegPair digitUnits[7];
  SegPair segHundredTop;
  SegPair segHundredBottom;
  SegPair segPercent;
  SegPair segBattery;
  
  bool showPercent = true;
  bool showBattery = true;
  bool polarityInversionEnabled = false;
  bool framePolarity = false;
  int displayValue = 0;
  uint16_t segmentOnTime = 300;
  uint16_t polarityInterval = 10;
  
  static const uint8_t MAX_ACTIVE_SEGMENTS = 20;
  SegPair activeFrame[MAX_ACTIVE_SEGMENTS];
  uint8_t activeFrameCount = 0;
  uint8_t currentSegIndex = 0;
  unsigned long lastPolarityToggle = 0;
  
  char tensChar = '0';
  char unitsChar = '0';
  bool useLetterMode = false;
  
  static const uint8_t digitToSegments[10];
  static uint8_t charToSegment(char c);
  
  void allPinsHiZ();
  void buildFrame();
  void addSegmentsForDigit(SegPair* digitTable, uint8_t segMask);
};

#endif