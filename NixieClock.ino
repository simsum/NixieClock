//  Nixie Clock V2.0.1 by Sven Steinmeier - sven.steinmeier@gmail.com
//  2014-05-08


#include <NixieTube.h>
#include <Time.h>
#include <DS3232RTC.h>  //Arduino Pro Mini I2C: A4 (SDA) und A5 (SCL)
#include <Wire.h>
#include <Metro.h>


//Variablen für Echtzeituhr einrichten
int Jahr;
int Monat;
int Tag;
int Stunde;
int Minute;
int Sekunde;

// Secound Point Nixie Tube binking
// Create a variable to hold theled's current state
boolean SecondPointState = HIGH;

// Instanciate a metro object and set the interval to 1000 milliseconds (1.00 second).
Metro SecondPoint = Metro(1000);

// Instanciate a metro object and set the interval to 5000 milliseconds (5.00 second).
Metro ReadRTC = Metro(5000);

// Instanciate a metro object and set the interval to 10 milliseconds (0.01 second).
Metro MainProg = Metro(10);

tmElements_t tm;
unsigned long unixtime; //unsigned! long to fix the year 2038 problem

// NixieTube einrichten
NixieTube tube(11, 12, 13, 10, 4);       // pin_ds, pin_st. pin_sh, pin_oe(pwm pin is preferred), COUNT

// Anzeige Variablen
Color TubeBackColor[4] = {Red, Red, Red, Red};
Colon TubeColon[4]     = {Both, Both, Both, Both};
byte  TubeNumber[4]    = {8, 8, 8, 8};

// Helligkeit Tube
float maxHelligkeit    = 255;             // Maximale Helligkeit der Anzeige
float NachtHelligkeit  = 10;              // Nacht Helligkeit der Anzeige
float aktHelligkeit    = maxHelligkeit;   // Aktuelle Helligkeit der Anzeige

// Effektzeiten festlegen
int FadeOutTime        = 300;             // Fade Out Effekt beginnt 400 (4 Sek.)Umläufte bevor umgeschaltet wird
int FadeInTime         = 400;             // Fade In Effet in den ersten 250 Umläufen

// Anzeige Modus der Tube
unsigned int TubeModus          = 0;               // Auswahl der Anzeigeart
unsigned int TubeModusHilf      = 0;               // Hilfvariable
unsigned int TubeModusTime      = 1000;            // Umschaltzeit in Umläuften ca. 100 Umläufe je Sekunde
unsigned int TubeModusStop      = LOW;             // Automatische Umschaltung pausieren (Nachtmodus)

void setup()
{
  // Setup Serial connection
  /*
         Serial.begin(9600);
         Serial.println("Debug ein");

  */
  // Farbe und Display einrichten
  tube.setBrightness(aktHelligkeit);	 // Helligkeit der Anzeige einstellen
  for (int i = 0; i < 4; i ++) {
    tube.setBackgroundColor(i, TubeBackColor [i]);
    tube.setColon(i, TubeColon[i]);
    tube.setNumber(i, TubeNumber[i]);
  }
  tube.display();                          // Anzeige aktualisieren

  delay(250);                              // Pause
  /*
          Serial.print("Ende Setup");
          Serial.println();    */
}

void loop()
{
  // Second Point Nixie Tube binking 
  // (Metro.h! Lib)
  if (SecondPoint.check() == 1) { // check if the metro has passed its interval .
    if (SecondPointState == HIGH)  {
      SecondPointState = LOW;
    }
    else {
      SecondPointState = HIGH;
    }
  }

  // Get data from RTC
  // (Metro.h! Lib)
  if (ReadRTC.check() == 1) { // check if the metro has passed its interval .
    RTC.read(tm);
    unixtime = makeTime(tm);
    // Daylight saving time activ (false = standard time, true = daylight saving time +1)
    int DaylightSaving = DST(tmYearToCalendar(tm.Year), tm.Month, tm.Day, tm.Hour, tm.Minute);
    // DaylightSaving adjustment, RTC should work with standard time
    if (DaylightSaving) unixtime = unixtime + 3600;
    breakTime(unixtime, tm);
  }
  
  // (Metro.h! Lib)
  if (MainProg.check() == 1) { // check if the metro has passed its interval .
  
  Jahr      = tmYearToCalendar(tm.Year);
  Monat     = (tm.Month);
  Tag       = (tm.Day);
  Stunde    = (tm.Hour);
  Minute    = (tm.Minute);
  Sekunde   = (tm.Second);

  // Auswahl der verschiedenen Anzeigenmodi
  switch (TubeModus) {
    case 0:         // Uhrzeit auf Tube ausgeben
      TubeModusTime = 18000;                                    //Anzeigedauer vorgeben
      // Fade In Effekt
      FadeIn();
      // Farben eintragen
      TubeBackColor[0] = Blue;
      TubeBackColor[1] = Blue;
      TubeBackColor[2] = Blue;
      TubeBackColor[3] = Blue;
      // Doppelpunkte bestimmen
      TubeColon[0] = None;
      // Doppelpunkt wechselseitig blinken lassen
      if (SecondPointState == LOW)  TubeColon[1] = Lower;
      if (SecondPointState == HIGH) TubeColon[1] = Upper;
      TubeColon[2] = None;
      TubeColon[3] = None;
      // Uhrzeit eintragen
      TubeNumber[0] = Stunde / 10;
      TubeNumber[1] = Stunde;
      TubeNumber[2] = Minute / 10;
      TubeNumber[3] = Minute;
      // Fade Out Effekt
      //FadeOutTime = 400;
      FadeOut();
      break;

    case 1:         // Datum auf Tube ausgeben
      TubeModusTime = 600;                                    //Anzeigedauer vorgeben
      // Fade In Effekt
      FadeIn();

      // Farben eintragen
      TubeBackColor[0] = Magenta;
      TubeBackColor[1] = Magenta;
      TubeBackColor[2] = Magenta;
      TubeBackColor[3] = Magenta;
      // Doppelpunkte bestimmen
      TubeColon[0] =  None;
      TubeColon[1] =  Lower;
      TubeColon[2] =  None;
      TubeColon[3] =  Lower;
      // Datum eintragen
      TubeNumber[0] = Tag / 10;
      TubeNumber[1] = Tag;
      TubeNumber[2] = Monat / 10;
      TubeNumber[3] = Monat;

      break;

    case 2:         // Datum scrollen

      TubeModusTime = 155;
      Colon scrTubeColon[9];
      int scrTubeNumber[9];

      scrTubeColon[0]  = None;
      scrTubeColon[1]  = Lower;
      scrTubeColon[2]  = None;
      scrTubeColon[3]  = Lower;
      scrTubeColon[4]  = None;
      scrTubeColon[5]  = None;
      scrTubeColon[6]  = None;
      scrTubeColon[7]  = None;

      scrTubeNumber[0] = Tag / 10;
      scrTubeNumber[1] = Tag;
      scrTubeNumber[2] = Monat / 10;
      scrTubeNumber[3] = Monat;
      scrTubeNumber[4] = 2;
      scrTubeNumber[5] = 0;
      scrTubeNumber[6] = (Jahr - 2000) / 10;
      scrTubeNumber[7] = (Jahr - 2000);

      static float val1 = 0;
      static float val2 = 0;
      val2 = TubeModusTime;
      val1 += (4 / val2);
      for (int i = 0; i <= 3; i ++) {
        int x;
        x = val1 + i;
        TubeColon[i]  = scrTubeColon[x];
        TubeNumber[i] = scrTubeNumber[x];
      }

      break;

    case 3:         // Jahr auf Tube ausgeben
      TubeModusTime = 750;
      val1 = 0;                                 //Hilfvariable aus vorherigem case auf 0 setzen
      // Farben eintragen
      TubeBackColor[0] = Magenta;
      TubeBackColor[1] = Magenta;
      TubeBackColor[2] = Magenta;
      TubeBackColor[3] = Magenta;
      // Doppelpunkte bestimmen
      TubeColon[0] = None;
      TubeColon[1] = None;
      TubeColon[2] = None;
      TubeColon[3] = None;
      // Jahr eintragen
      TubeNumber[0] = 2;
      TubeNumber[1] = 0;
      TubeNumber[2] = (Jahr - 2000) / 10;
      TubeNumber[3] = (Jahr - 2000);
      // Fade Out Effekt
      FadeOut();
      break;

    default:         // wenn nix passt dann wieder die Uhrzeit anzeigen
      TubeModus = 0;
      break;
  }

  //Änderungen auf Tube anzeigen
  tube.setBrightness(aktHelligkeit);              // Helligkeit festlegen
  for (int i = 0; i <= 3; i ++) {                 // Schleife
    tube.setBackgroundColor(i, TubeBackColor [i]);
    tube.setColon(i, TubeColon[i]);
    tube.setNumber(i, TubeNumber[i]);
  }
  tube.display();

  //Umschaltung des Anzeige Modus Tagsüber automatischer Wechsel, Nachts nur Uhrzeit und gedimmt
  if (Stunde >= 23 or Stunde <= 6) {
    TubeModusStop = HIGH;
  }
  else {
    TubeModusStop = LOW;

  }

  // Tagmodus
  if (TubeModusStop == LOW) {                     // wenn automatischen Umschaltung erlaubt
    TubeModusHilf ++;
    if (TubeModusHilf > TubeModusTime) {       // Nach eingesteller Zeit nächste Anzeigeart auswählen
      TubeModus ++;
      TubeModusHilf = 0;                       // Sekundenzähler zurücksetzen
    }
  }

  // Nachtmodus
  if (TubeModusStop == HIGH) {                    // wenn automatischen Umschaltung aus
    TubeModus = 0;                              // nur Uhrzeit anzeigen
    aktHelligkeit = NachtHelligkeit;            // niedrige Helligkeit
  }
  }

}

// --------------------------------
// FadeIn und FadeOut Funktion
// --------------------------------

float Begrenzung0_max (float Wert)
{
  if (Wert > maxHelligkeit) Wert = maxHelligkeit;
  if (Wert < 0) Wert = 0;
  return Wert;
}

float FadeIn()
{
  float zwHelligkeit;
  if (TubeModusHilf <= TubeModusTime - FadeInTime) {                      // Fade In am Anfang
    zwHelligkeit = maxHelligkeit / FadeInTime;                            //
    aktHelligkeit = Begrenzung0_max(aktHelligkeit += zwHelligkeit);       // Wertbegrenzung 0 bis max.
  }
  return aktHelligkeit;
}

float FadeOut()
{
  float zwHelligkeit;
  if (TubeModusHilf > TubeModusTime - FadeOutTime) {                       // Fade Out am Ende
    zwHelligkeit = maxHelligkeit / (FadeOutTime - 100);                    // durch FadeOutTime - 100 wird erreicht das die Anzeige bereits eine Sekunde früher aus ist
    //Serial.print("zwHelligkeit: ");
    //Serial.println( zwHelligkeit);
    //Serial.print("aktHelligkeit: ");
    //Serial.println( aktHelligkeit);
    aktHelligkeit = Begrenzung0_max(aktHelligkeit -= zwHelligkeit);        // Wertbegrenzung 0 bis max.
  }
  return aktHelligkeit;
}

