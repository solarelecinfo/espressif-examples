#include <SPI.h>
#include <MFRC522.h>

#define LED_PIN 7
#define RST_PIN 9
#define SS_PIN 10

const int keySize = 4;
MFRC522 mfrc522(SS_PIN, RST_PIN);
// NUID de la carte précédemment détectée
byte nuidPICC[keySize];
// Code autorisé
byte secretCode[keySize] = {81, 227, 243, 93};//code en entier decimal

void setup() {
  Serial.begin(9600);
  while (!Serial);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(LED_PIN, OUTPUT);

  Serial.println("Scan d'une carte MIFARE Classic...");
}

void loop() {
  // Aucune nouvelle carte
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Impossible de lire le numéro de série
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Vérifier la carte et récupérer son NUID
  if (isValidCard()) {
    for (byte i = 0; i < keySize; i++) {
      nuidPICC[i] = mfrc522.uid.uidByte[i];
    }

     // Affichage du NUID de la carte 
    Serial.println("NUID:");
    printDec(nuidPICC, keySize);
    // Comparaison NUID / code autorisé
    if (compareArrays( nuidPICC,secretCode,keySize,keySize)) {
      Serial.println("\n Carte autorisee");
      digitalWrite(LED_PIN, HIGH);
      delay(1000);
      digitalWrite(LED_PIN, LOW);
    } else {
      Serial.println("\n Carte non autorisee");
      digitalWrite(LED_PIN, LOW);
    }
  } else {
    // Carte non valide
    for (byte i = 0; i < keySize; i++) {
      nuidPICC[i] = 0;
    }
  }

  // Arrêt de la carte
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

/*
* Vérifie que la carte est une MIFARE Classic
* et affiche son NUID.
*/
int isValidCard() {
  MFRC522::PICC_Type piccType =
    mfrc522.PICC_GetType(mfrc522.uid.sak);

  Serial.print("PICC type : ");
  Serial.println(mfrc522.PICC_GetTypeName(piccType));

  // Vérifier le type de carte
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {

    Serial.println("Carte non compatible MIFARE Classic");
    return 0;
  }

  Serial.println("Carte MIFARE Classic detectee");
  Serial.print("NUID nouveau: ");
  printDec(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();
  return 1;
}

/*
* Affichage du tableau en decimal
*/
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(' ');
    Serial.print(buffer[i], DEC);
  }
}

/*
* Compare deux tableaux
*/
int compareArrays(byte* arr1,byte* arr2,int size1,int size2) {
  if (size1 != size2) {
    return 0;
  }
  for (int i = 0; i < size1; i++) {
    if (arr1[i] != arr2[i]) {
      return 0;
    }
  }
  return 1;
}