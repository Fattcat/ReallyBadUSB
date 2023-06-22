#include <SD.h>
#include <SPI.h>

//        CONNECTION
// SD modul -> Arduino Pro Micro

//   VCC   ->   VCC (5V)
//   GND   ->   GND
//   MISO  ->   14
//   MOSI  ->   16
//   SCK   ->   1
//   CS    ->   10


const int chipSelect = 10;

const int bufferSize = 512;  // Veľkosť buffera pre prijímanie fotiek

File photoFile;

void setup() {

  Serial.begin(115200);

  delay(100);

  while (!Serial) {

    ;  // Čakať na spojenie so sériovým portom

  }

  if (!SD.begin(chipSelect)) {

    Serial.println("Chyba pri inicializácii SD karty!");

    return;

  }

  Serial.println("SD karta inicializovaná.");

  receivePhotos();

}

void loop() {

  // Žiadne ďalšie operácie v loop

}

void receivePhotos() {

  char buffer[bufferSize];

  while (Serial.available()) {

    Serial.read();  // Zbaviť sa ľubovoľných prebytočných dát

  }

  while (true) {

    int bytesRead = Serial.readBytesUntil('\n', buffer, bufferSize);

    if (bytesRead <= 0) {

      break;  // Koniec prijímania fotiek

    }

    if (!photoFile) {

      String fileName = "photo" + String(millis()) + ".jpg";

      photoFile = SD.open(fileName, FILE_WRITE);

    }

    if (photoFile) {

      photoFile.write((const uint8_t*)buffer, bytesRead);

    }

  }

  if (photoFile) {

    photoFile.close();

    Serial.println("Fotky boli uložené na SD kartu.");

  } else {

    Serial.println("Chyba pri otváraní súboru na SD karte.");

  }

}

