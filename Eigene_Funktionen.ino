


int WochenTagBerechnen(int Jahr, int Monat, int Tag)   //  Wochentag aus dem Datum errechnen
{
  int WoTag;
  int WochenTag;  

  WoTag = ((((5 * Jahr) + Monatsziffer(Monat)) / 4) + Tag - 1) % 7;  //  Zwischenergebnis (0=Son, 1=Mon, usw.)
  if (WoTag < 1)                                                     //  Endergebnis in der richtigen Folge (1=Mon, 2=Die,... 7=Son)
  {
    WochenTag = 7;
  }
  else
  {
    WochenTag = WoTag;
  }
  return WochenTag;
}

int Monatsziffer (int Monat)                         //  Monatskennziffer festlegen (wird u.a. zur Wochentagsberechnung benötigt)
{
  int Ergebnis;

  switch (Monat)  
  {
  case 1:                                          // Monatziffer festlegen (Januar)
    Ergebnis = 23;
    break;     
  case 2:                                          // Monatziffer festlegen (Februar)
    Ergebnis = 7;
    break;
  case 3:                                          // Monatziffer festlegen (März)
    Ergebnis = 8;
    break;     
  case 4:                                          // Monatziffer festlegen (April)
    Ergebnis = 20;
    break;
  case 5:                                          // Monatziffer festlegen (Mai)
    Ergebnis = 0;
    break;     
  case 6:                                          // Monatziffer festlegen (Juni)
    Ergebnis = 12;
    break;
  case 7:                                          // Monatziffer festlegen (Juli)
    Ergebnis = 20;
    break;     
  case 8:                                          // Monatziffer festlegen (August)
    Ergebnis = 4;
    break;
  case 9:                                          // Monatziffer festlegen (September)
    Ergebnis = 16;
    break;     
  case 10:                                         // Monatziffer festlegen (Oktober)
    Ergebnis = 24;
    break;    
  case 11:                                         // Monatziffer festlegen (November)
    Ergebnis = 8;
    break;     
  case 12:                                         // Monatziffer festlegen (Dezember)
    Ergebnis = 16;
    break;    

  } 
  return Ergebnis;
}

int Osterkennzahl(int Jahr)                          // Osterkennzahl berechnen (wird für weitere Berechnungen notwendig)
{
  int a;    //Hilfvariable
  int b;    //Hilfvariable
  int c;    //Hilfvariable
  int e;    //Osterkennzahl
  a = (Jahr % 19 * 19 + 24) % 30;
  b = 120 + a - (a / 27);
  c = (b + (Jahr * 5 / 4) - (Jahr / 2100)) % 7;
  e = b - c;
  return e;
}

int TagNummer (int Jahr, int Monat, int Tag)      // Tag Nummer berechnen ( 05.01.2012 = 5 .... 31.12.2012 = 366)
{
  int sj = SchaltjahrAktiv (Jahr);
  int TagNr = Tag + ((489 * Monat) / 16) - (7 + Monat) / (10 * (2 - sj)) -30;
  return TagNr;
}


int BeginnTagSommerzeit (int Jahr)                  // Tag im März - Beginn Sommerzeit (02:00 Uhr)
{
  int e = Osterkennzahl (Jahr);
  int Tag;
  Tag = 25 + (e + 2) % 7;
  return Tag;
} 

int EndeTagSommerzeit (int Jahr)                   // Tag im Oktober - Ende Sommerzeit (03:00 Uhr)
{
  int e = Osterkennzahl (Jahr);
  int Tag;
  Tag = 25 + (e + 5) % 7;
  return Tag;
} 

int SchaltjahrAktiv (int Jahr)                     // Schaltjahr aktiv (0 = Nein, 1 = Ja)
{
  int sj = (1/(1 + Jahr % 4)) - (1/(1 + Jahr % 100)) + (1/(1 + Jahr % 400));
  return sj;
} 


int SommerzeitAktiv (int Jahr, int Monat, int Tag, int Stunde, int Minute)      // Sommerzeit aktiv aktiv (0 = Nein, 1 = Ja)
{
  int aktiv;
  long beginnZW  = TagNummer(Jahr, 3, BeginnTagSommerzeit (Jahr));              // Hilfsvariable
  long beginn    = (beginnZW *1440) + 120;                                      // Beginn Sommerzeit in "Minuten"

  long endeZW    = TagNummer(Jahr, 10, EndeTagSommerzeit (Jahr));               // Hilfsvariable
  long ende      = (endeZW * 1440) + 180;                                       // Ende Sommerzeit in "Minuten"

  long aktuellZW = TagNummer(Jahr, Monat, Tag);                                 // Hilfsvariable
  long aktuell   = (aktuellZW * 1440) + (Stunde * 60) + Minute;                 // Minuten im Jahr

  if (aktuell >= beginn && aktuell <= ende) {
    aktiv = true;
  }
  else {
    aktiv = false;
  }

  return aktiv;
}



