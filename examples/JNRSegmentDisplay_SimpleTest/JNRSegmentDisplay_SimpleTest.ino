/*
 * LCDSegmentDisplay - Test Simple
 * -------------------------------
 * Exemple minimal pour tester rapidement la librairie
 * Affiche simplement les valeurs de 0 a 100 en boucle
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
//LCDSegmentDisplay lcd(48, 45, 0, 21, 47);  // p1 et p2 échangés
//LCDSegmentDisplay lcd(45, 0, 48, 21, 47);  // p2 et p3 échangés

void setup() {
  Serial.begin(115200);
  delay(500);
  
  lcd.begin();
  lcd.setValue(0);
  
  Serial.println("Test simple LCDSegmentDisplay");
  Serial.println("Defilement 0-100 toutes les 500ms");
}

int value = 0;
unsigned long lastChange = 0;

void loop() {
  // OBLIGATOIRE : Appeler update() en boucle pour le multiplexage
  lcd.update();
  
  // Changer de valeur toutes les 500ms
  if (millis() - lastChange > 500) {
    lastChange = millis();
    value++;
    if (value > 100) value = 0;
    lcd.setValue(value);
    Serial.print("Valeur: ");
    Serial.print(value);
    Serial.println("%");
  }
}
