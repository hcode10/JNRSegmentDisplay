# JNRSegmentDisplay

Arduino library for driving JNR Puff LCD segment displays (0-100%) with letter support.

Specifically designed for the JNR puff device LCD screen.

## Installation

1. Clone or download this repository
2. Place it in your Arduino `libraries` folder
3. Restart Arduino IDE
4. Include the library: `#include <JNRSegmentDisplay.h>`

## Usage

### Basic Example

```cpp
#include <JNRSegmentDisplay.h>

// Constructor: p1, p2, p3, p4, p5 = physical pins 1-5 of your LCD
JNRSegmentDisplay lcd(45, 48, 0, 21, 47);

void setup() {
  lcd.begin();
  lcd.setValue(42);  // Display 0-100
}

void loop() {
  lcd.update();  // Required for multiplexing
}
```

### Available Methods

- `begin()` - Initialize the display
- `setValue(int v)` - Display numeric value (0-100)
- `setDisplay(char tens, char units)` - Display two characters (digits or letters)
- `setLetter(char letter)` - Display letter on units digit
- `setShowPercent(bool)` - Show/hide % icon
- `setShowBattery(bool)` - Show/hide battery icon
- `update()` - **MUST be called in loop()** for multiplexing
- `setSegmentOnTime(uint16_t us)` - Segment on time in microseconds
- `setPolarityInterval(uint16_t ms)` - Polarity inversion interval
- `enablePolarityInversion(bool)` - Enable/disable AC polarity inversion
- `getValue()` - Get current numeric value
- `getTensChar()` - Get tens digit character
- `getUnitsChar()` - Get units digit character

### Supported Characters

- **Digits:** 0-9
- **Letters:** A, B, C, D, E, F, G, H, I, J, L, N, O, P, Q, R, S, T, U, Y
- **Special:** `-`, `_`, (space)

### Wiring

Based on the original working code, the pin mapping is:
```
Physical Pin 1 -> Arduino 45
Physical Pin 2 -> Arduino 48
Physical Pin 3 -> Arduino 0
Physical Pin 4 -> Arduino 21
Physical Pin 5 -> Arduino 47
```

If your LCD has different wiring, permute the constructor parameters.

## Examples

The library includes 4 examples:
- `JNRSegmentDisplay_SimpleTest` - Basic 0-100 scroll test
- `JNRSegmentDisplay_Test` - Comprehensive test with all features
- `JNRSegmentDisplay_Letters` - Letter display test
- `JNRSegmentDisplay_PhysicalPins` - Physical pin ordering example

## Troubleshooting

If the display doesn't work:
1. Check your wiring
2. Try permuting the constructor parameters
3. Ensure `lcd.update()` is called in `loop()`
4. Test with `lcd.setValue(88)` - all segments should light up

## License

MIT License
