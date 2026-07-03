/*
 * JNRSegmentDisplay - Test avec Lettres
 * --------------------------------------
 * Exemple d'utilisation des lettres sur l'affichage LCD
 * 
 * ATTENTION: Seuls certains caracteres sont affichables sur un display 7-segments :
 *   Chiffres: 0-9
 *   Lettres:  A, B, C, D, E, F, G, H, I, J, L, N, O, P, Q, R, S, T, U, Y
 *   Speciaux: -,  (espace), _
 * 
 * Le digit des dizaines peut etre un chiffre OU une lettre.
 * Pour afficher "A5" ou "9B" ou "HELLO" (si l'affichage le permet)
 * 
 * CABLAGE POUR VOTRE LCD (d'apres la photo) :
 *   Broche physique 1 -> Arduino 45
 *   Broche physique 2 -> Arduino 48
 *   Broche physique 3 -> Arduino 0
 *   Broche physique 4 -> Arduino 21
 *   Broche physique 5 -> Arduino 47
 */

#include <JNRSegmentDisplay.h>

// Instanciation avec les broches PHYSIQUES dans l'ordre 1 a 5
// ATTENTION : Broche physique 3 = Arduino 0 (pas 20 !)
JNRSegmentDisplay lcd(45, 48, 0, 21, 47);

void setup() {
  Serial.begin(115200);
  delay(500);
  
  lcd.begin();
  
  Serial.println("=== Test Lettres JNRSegmentDisplay ===");
  Serial.println("Test des differentes combinaisons chiffre + lettre");
}

unsigned long lastChange = 0;
int testIndex = 0;

// Liste de tests : {tens, units, description}
const struct TestCase {
  char tens;
  char units;
  const char* desc;
  bool is100;  // true to display as 100 using setValue()
} testCases[] = {
  {'0', 'A', "0A", false},
  {'1', 'B', "1B", false},
  {'2', 'C', "2C", false},
  {'3', 'D', "3D", false},
  {'4', 'E', "4E", false},
  {'5', 'F', "5F", false},
  {'6', 'G', "6G", false},
  {'7', 'H', "7H", false},
  {'8', 'I', "8I", false},
  {'9', 'J', "9J", false},
  {'A', 'B', "AB", false},
  {'B', 'C', "BC", false},
  {'H', 'I', "HI", false},
  {'L', 'O', "LO", false},
  {'P', 'A', "PA", false},
  {'T', 'E', "TE", false},
  {'U', 'P', "UP", false},
  {'-', '-', "--", false},
  {' ', ' ', "  ", false},
  {'0', '0', "00", false},
  {'1', '0', "10", false},  // Affiche 10 (pas 100)
  {'0', '0', "100", true}   // Pour afficher 100, utilise setValue(100)
};

const int numTests = sizeof(testCases) / sizeof(testCases[0]);

void loop() {
  // OBLIGATOIRE : Appeler update() en boucle pour le multiplexage
  lcd.update();
  lcd.setShowBattery(false);
  
  if (millis() - lastChange > 2000) {
    lastChange = millis();
    
    const TestCase& tc = testCases[testIndex];
    
    // Special case: display 100
    if (tc.is100) {
      lcd.setValue(100);
      Serial.print("Valeur numerique: 100");
    } else {
      // Normal letter/digit display
      lcd.setDisplay(tc.tens, tc.units);
      Serial.print("Affiche: '");
      Serial.print(tc.tens);
      Serial.print(tc.units);
      Serial.print("' (");
      Serial.print(tc.desc);
      Serial.print(")");
    }
    Serial.println();
    
    testIndex = (testIndex + 1) % numTests;
  }
}
