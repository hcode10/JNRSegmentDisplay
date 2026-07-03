/*
 * LCDSegmentDisplay - Exemple avec ordre physique des broches
 * ------------------------------------------------------------
 * 
 * Cet exemple montre comment utiliser le constructeur avec l'ordre physique
 * des broches du LCD pour correspondre à votre câblage.
 * 
 * VOTRE CABLAGE (d'apres la photo) :
 *   Broche physique 1 du LCD -> Broche Arduino 45
 *   Broche physique 2 du LCD -> Broche Arduino 48
 *   Broche physique 3 du LCD -> Broche Arduino 0
 *   Broche physique 4 du LCD -> Broche Arduino 21
 *   Broche physique 5 du LCD -> Broche Arduino 47
 * 
 * APPPEL SIMPLE :
 *   JNRSegmentDisplay lcd(45, 48, 0, 21, 47);
 * 
 * Le constructeur mappe automatiquement :
 *   Broche physique 1 (45) -> PIN_19
 *   Broche physique 2 (48) -> PIN_20
 *   Broche physique 3 (0)  -> PIN_48
 *   Broche physique 4 (21) -> PIN_47
 *   Broche physique 5 (47) -> PIN_21
 * 
 * Si votre LCD a un brochage different, permutez simplement l'ordre des parametres.
 */

#include <JNRSegmentDisplay.h>

// Instanciation avec les broches PHYSIQUES dans l'ordre 1 a 5
// Pour votre LCD : pin1=45, pin2=48, pin3=0, pin4=21, pin5=47
JNRSegmentDisplay lcd(45, 48, 0, 21, 47);

void setup() {
  Serial.begin(115200);
  delay(500);
  
  lcd.begin();
  lcd.setValue(42);
  
  Serial.println("Test avec ordre physique des broches");
  Serial.println("Si l'affichage est incorrect, permutez les parametres du constructeur");
}

int value = 0;
unsigned long lastChange = 0;

void loop() {
  lcd.update();
  
  if (millis() - lastChange > 500) {
    lastChange = millis();
    value = (value > 100) ? 0 : value + 1;
    lcd.setValue(value);
    Serial.print("Valeur: ");
    Serial.print(value);
    Serial.println("%");
  }
}

// ============================================
// REFERENCE : Mapping par défaut avec physicalOrder=true
// ============================================
// Broche physique 1 -> PIN_19 (role internal)
// Broche physique 2 -> PIN_20 (role internal)
// Broche physique 3 -> PIN_48 (role internal)
// Broche physique 4 -> PIN_47 (role internal)
// Broche physique 5 -> PIN_21 (role internal)
//
// Si votre LCD a un mapping different, changez l'ordre des parametres :
// Exemple : JNRSegmentDisplay lcd(p5, p4, p3, p2, p1, true);
//          pour inverser complètement l'ordre
