/*
 * LCDSegmentDisplay - Test complet
 * --------------------------------
 * Code de test pour la librairie LCDSegmentDisplay
 * Teste toutes les fonctionnalités : affichage 0-100, icônes %, batterie, polarité
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
JNRSegmentDisplay lcd(45, 48, 0, 21, 47);

// Variables pour le test
unsigned long lastValueChange = 0;
unsigned long lastPercentToggle = 0;
unsigned long lastBatteryToggle = 0;
unsigned long lastPolarityToggle = 0;

const unsigned long VALUE_CHANGE_DELAY = 1000;    // 1 seconde par valeur
const unsigned long ICON_TOGGLE_DELAY = 5000;    // 5 secondes pour basculer les icônes
const unsigned long POLARITY_TOGGLE_DELAY = 15000; // 15 secondes pour tester la polarité

int currentValue = 0;
bool showPercentState = true;
bool showBatteryState = true;
bool polarityEnabled = false;

enum TestMode {
  MODE_SCROLL,      // Défilement 0-100
  MODE_ICONS,       // Test des icônes
  MODE_POLARITY,    // Test de la polarité
  MODE_SPECIAL      // Valeurs spéciales (0, 100, etc.)
};

TestMode currentMode = MODE_SCROLL;

void setup() {
  Serial.begin(115200);
  delay(500);
  
  lcd.begin();
  lcd.setValue(0);
  
  Serial.println("=== Test LCDSegmentDisplay ===");
  Serial.println("Mode : SCROLL (0-100)");
  Serial.println("Les valeurs changent toutes les 1 seconde");
  Serial.println("Icones % et batterie basculent toutes les 5 secondes");
  Serial.println("Polarite bascule toutes les 15 secondes");
}

void loop() {
  // Mise à jour OBLIGATOIRE du display (multiplexage)
  lcd.update();
  
  unsigned long currentTime = millis();
  
  // ============================================
  // Mode 1 : Défilement des valeurs 0-100
  // ============================================
  if (currentMode == MODE_SCROLL) {
    if (currentTime - lastValueChange > VALUE_CHANGE_DELAY) {
      lastValueChange = currentTime;
      currentValue++;
      if (currentValue > 100) currentValue = 0;
      lcd.setValue(currentValue);
      Serial.print("Affiche: ");
      Serial.print(currentValue);
      Serial.println("%");
    }
    
    // Basculer les icônes périodiquement
    if (currentTime - lastPercentToggle > ICON_TOGGLE_DELAY) {
      lastPercentToggle = currentTime;
      showPercentState = !showPercentState;
      lcd.setShowPercent(showPercentState);
      Serial.print("Icone % : ");
      Serial.println(showPercentState ? "ON" : "OFF");
    }
    
    if (currentTime - lastBatteryToggle > ICON_TOGGLE_DELAY + 1000) {
      lastBatteryToggle = currentTime;
      showBatteryState = !showBatteryState;
      lcd.setShowBattery(showBatteryState);
      Serial.print("Icone batterie : ");
      Serial.println(showBatteryState ? "ON" : "OFF");
    }
    
    // Basculer vers le mode test polarité après 30 secondes
    if (currentTime > 30000 && currentMode == MODE_SCROLL) {
      currentMode = MODE_POLARITY;
      Serial.println("\n=== Mode : POLARITY TEST ===");
      Serial.println("Test de l'inversion de polarite (evite le ghosting)");
      lcd.setValue(50);
      lcd.enablePolarityInversion(true);
      polarityEnabled = true;
      lastPolarityToggle = currentTime;
    }
  }
  
  // ============================================
  // Mode 2 : Test de la polarité
  // ============================================
  else if (currentMode == MODE_POLARITY) {
    lcd.enablePolarityInversion(true);
    lcd.setSegmentOnTime(300);
    lcd.setPolarityInterval(10);  // Inversion toutes les 10ms (~50Hz)
    
    // Faire clignoter la valeur pour visualiser l'effet
    if (currentTime - lastValueChange > 2000) {
      lastValueChange = currentTime;
      currentValue = (currentValue == 50) ? 75 : 50;
      lcd.setValue(currentValue);
      Serial.print("Polarite active - Valeur: ");
      Serial.println(currentValue);
    }
    
    // Basculer vers le mode spécial après 20 secondes
    if (currentTime > 50000 && currentMode == MODE_POLARITY) {
      currentMode = MODE_SPECIAL;
      Serial.println("\n=== Mode : SPECIAL VALUES ===");
      Serial.println("Test des valeurs speciales : 0, 100, 1, 99, etc.");
      lcd.enablePolarityInversion(false);
      lcd.setShowPercent(true);
      lcd.setShowBattery(true);
      currentValue = 0;
      lastValueChange = currentTime;
    }
  }
  
  // ============================================
  // Mode 3 : Valeurs spéciales
  // ============================================
  else if (currentMode == MODE_SPECIAL) {
    lcd.enablePolarityInversion(false);
    
    // Liste des valeurs spéciales à tester
    const int specialValues[] = {0, 1, 5, 10, 15, 20, 25, 30, 50, 75, 99, 100};
    const int numValues = 12;
    
    if (currentTime - lastValueChange > 1500) {
      lastValueChange = currentTime;
      static int specialIndex = 0;
      currentValue = specialValues[specialIndex];
      lcd.setValue(currentValue);
      Serial.print("Valeur speciale: ");
      Serial.println(currentValue);
      specialIndex = (specialIndex + 1) % numValues;
    }
    
    // Basculer les icônes
    if (currentTime - lastPercentToggle > 3000) {
      lastPercentToggle = currentTime;
      showPercentState = !showPercentState;
      lcd.setShowPercent(showPercentState);
    }
    
    if (currentTime - lastBatteryToggle > 3500) {
      lastBatteryToggle = currentTime;
      showBatteryState = !showBatteryState;
      lcd.setShowBattery(showBatteryState);
    }
    
    // Retour au mode scroll après 40 secondes
    if (currentTime > 90000) {
      currentMode = MODE_SCROLL;
      Serial.println("\n=== Retour au Mode : SCROLL ===");
      lcd.setValue(0);
      currentValue = 0;
      lastValueChange = currentTime;
    }
  }
}
