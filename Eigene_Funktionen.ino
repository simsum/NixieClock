

//Die Funktion DAY_OF_WEEK berechnet den Wochentag aus dem Eingangsdatum.
int DAY_OF_WEEK(int Year, int Month, int Day)   
{
  int WeDay;
  int Weekday;  
 
  WeDay = ((((5 * Year) + Monatsziffer(Month)) / 4) + Day - 1) % 7;  //Zwischenergabnis (0=Son, 1=Mon, usw.)
  if (WeDay < 1)                                                     //Endergebnis in der richtigen Folge (1=Mon, 2=Die,... 7=Son)
  {
    Weekday = 7;
  }
  else
  {
    Weekday = WeDay;
  }
  return Weekday;
}

int Monatsziffer (int Monat)                         //Monatskennziffer festlegen (wird u.a. zur Wochentagsberechnung benötigt)
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

//Die Funktion LEAP_YEAR testet, ob das Eingangsjahr ein Schaltjahr ist und gibt gegebenenfalls TRUE aus.
//Der Test hat Gültigkeit für den Zeitrum 1970 - 2099. 
int LEAP_YEAR (int Year)                     // Schaltjahr aktiv (1 = Ja)
{
  int sj = (1/(1 + Year % 4)) - (1/(1 + Year % 100)) + (1/(1 + Year % 400));
  return sj;
} 

//Die Funktion DST überprüft, ob im Augenblick Sommerzeit herrscht, oder nicht.
//Die Sommerzeit wird aufgrund von UTC (Weltzeit) berechnet. 
bool DST (int Year, int Month, int Day, int Hour, int Minute)       // Sommerzeit aktiv (TRUE wenn Sommerzeit)
{                                                              
  bool active;
  long StartDSInM   = (DAY_OF_YEAR(Year, 3, BeginnTagSommerzeit (Year)) *1440) + 120;  // Beginn Sommerzeit in Minuten                                      // Beginn Sommerzeit in "Minuten"
  long EndDSInM     = (DAY_OF_YEAR(Year, 10, EndeTagSommerzeit (Year)) * 1440) + 180;  // Ende Sommerzeit in Minuten                                     // Ende Sommerzeit in "Minuten"
  long NowInMinute  = (DAY_OF_YEAR(Year, Month, Day) * 1440) + (Hour * 60) + Minute;   // aktuelle Minuten im Jahr
  
  active = NowInMinute >= StartDSInM && NowInMinute <= EndDSInM;                       // Ausgang TRUE wenn Zeitraum innerhalb Sommerzeit
  return active;
}
  
//Die Funktion DAY_OF_YEAR berechnet den Tag des Jahres aus dem Eingangsdatum. 

int DAY_OF_YEAR (int Year, int Month, int Day)      // ( 05.01.2012 = 5 .... 31.12.2012 = 366)
{
  int sj = LEAP_YEAR (Year);
  int DayNbr = Day + 489 * Month / 16 - (7 + Month) / 10 * (2 - sj) -30;
  return DayNbr;
} 

// noch zu testende Funktionen !!!

//Die Funktion SUN_MIDDAY berechnet abhängig vom Tagesdatum zu welcher Tageszeit die Sonne exakt im Süden steht. 
//Die Berechnung erfolgt in UTC (Weltzeit)  
float SUN_MIDDAY (float Lon, int Year, int Month, int Day)
{
  float T = DAY_OF_YEAR(Year, Month, Day);
  float OFFSET = -0.1752 * sin(0.033430 * T + 0.5474) - 0.1340 * sin(0.018234 * T - 0.1939);
  float SunMidday = (12.0 - OFFSET - Lon * 0.0666666666666);
  return SunMidday; 
  
} 
