#include <MIDI.h>

//Définition du nombre de boutons, de leurs Pin et de la pin de leur Led
#define PIN_BOUTON_1 3
#define PIN_LED_1 8
#define NB_BOUTONS 4

int pinBouton[NB_BOUTONS];  //Je crée le Tableau des pin de branchement de mes boutons
int etatBouton[NB_BOUTONS]; //Je crée le tableau de flags d'état de mes boutons
int pinLed[NB_BOUTONS]; //Je crée le Tableau des pin de branchement des LEDs
int etatLed[NB_BOUTONS];  //Je crée le tableau d'état des Leds
int randNumber =0;
bool flag=0;

MIDI_CREATE_DEFAULT_INSTANCE();


//Prise des référentiels de temps
#define BPM 55

//Calcul pour l'horloge midii
unsigned long microParTick = (unsigned long) (1e6 / (BPM * 24.0 / 60.0)) ;
unsigned long dernier_tick = 0UL ;
long compteurdelamort = 1 ;


void setup()
{
  //Initialisation des boutons et des Leds
  for (int i = 0 ; i < NB_BOUTONS ; i++)
  {
    pinBouton[i] = PIN_BOUTON_1+i;  //J'initialise toutes mes pin à partir de la broche 5
    etatBouton[i] = HIGH;  //J'initialise tous mes boutons en HIGH (branchement en Pullup
    pinMode(pinBouton[i], INPUT_PULLUP); //J'initialise toutes mes pins de bouton comme des entrées

    etatLed[i] = LOW;  //J'initialise toutes mes LEDs éteintes
    pinLed[i] = PIN_LED_1+i;  //J'initialise toutes mes pin à partir de la broche 5
    pinMode(pinLed[i], OUTPUT); //J'initialise toutes mes pins de bouton comme des entrées
  }
  

  //Initialisation MIDI
  MIDI.begin();
  MIDI.sendStop();
  //Serial.begin(9600);
  delay(4000);
  MIDI.sendStart();
}

void loop()
{
  
if ((micros () - dernier_tick) >= microParTick)
  {
    dernier_tick += microParTick ; // schedule next tick
    MIDI.sendClock();
    
    if (compteurdelamort == 24 )
    {
      flag=0;
      //Réinitation des messages
      for (int i = 0 ; i < NB_BOUTONS ; i++)
      {
        etatBouton[i] = HIGH;
        digitalWrite(pinLed[i], LOW);
        delayMicroseconds(5000);
      }
      //tirage d'une PIN au hasard et Allumage de la LED
      randNumber = random(0, NB_BOUTONS);
      digitalWrite(pinLed[randNumber],HIGH);
      compteurdelamort=0;
    }
    compteurdelamort++;
   
  }

  //Lecture du bouton, extinction et envoi du message
  etatBouton[randNumber] = digitalRead(pinBouton[randNumber]);
  if (etatBouton[randNumber]==LOW && flag==0)
  {
    digitalWrite(pinLed[randNumber], LOW);
    randNumber=randNumber*12;
    MIDI.sendNoteOn(12+randNumber, 127, 1);
    MIDI.sendNoteOff(12+randNumber, 0, 1);
    flag=1;
  }

  /*
  //Envoie de l'horloge MIDI
   
  if (referenceClock >= interval)
  {
    referenceClock = referenceClock - interval;  //réinitialisation du compteur
    MIDI.sendClock();   
  }

   //A chaque seconde (60bmp)
  if (referenceTempo >= PULSATION) 
  {
    referenceTempo = referenceTempo - PULSATION;  //réinitialisation du compteur
    flag=0;
    //Réinitation des messages
   for (int i = 0 ; i < NB_BOUTONS ; i++)
      {
       etatBouton[i] = HIGH;
       digitalWrite(pinLed[i], LOW);
      }
    //tirage d'une PIN au hasard et Allumage de la LED
    randNumber = random(0, NB_BOUTONS);
    digitalWrite(pinLed[randNumber],HIGH);
  }

  //Lecture du bouton, extinction et envoi du message
  etatBouton[randNumber] = digitalRead(pinBouton[randNumber]);
  if (etatBouton[randNumber]==LOW && flag==0)
  {
    digitalWrite(pinLed[randNumber], LOW);
    MIDI.sendNoteOn(10+randNumber, 127, 1);
    MIDI.sendNoteOff(410+randNumber, 0, 1);
    flag=1;
  }
  */

}
