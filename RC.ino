// C++ code
//
/*
  Carica e scarica di un condensatore
  creato 30/04/2022
  by Andrea Colonna
*/
#include <phyphoxBle.h>      // libreria per la comunicazione tramite bluetooth con phyphox
  char board_name[] = "rosmini6"; // per cambiare il nome della scheda che usa il BLE: non usare al cun carattere speciale e nessun spazio
  #define ROSSO 22     
  #define BLU 24     
  #define VERDE 23
  #define LED_PWR 25

  const int Sensor = A0;             //Pin di lettura del voltaggio
  int Supply = 8;              //Pin di carica e scarica del condensatore
  float first_difference_float, fourth_difference_float;   // variabili di comodo per il tempo
  unsigned long tempo_iniziale;
  unsigned long tempo;
  float tensione;
  float resistenza = 10000.0;
  float capacita = 0.0001;
  float tau;
  int passi = 100;

void setup() {
  PhyphoxBLE::minConInterval = 6; //6 = 7.5ms       // impostazioni per il bluetooth
  PhyphoxBLE::maxConInterval = 6; //6 = 7.5ms
  PhyphoxBLE::slaveLatency = 0; //
  PhyphoxBLE::timeout = 10; //10 = 100ms 
  
  pinMode(ROSSO, OUTPUT);  // dichiarazione dei led della scheda
  pinMode(BLU, OUTPUT);
  pinMode(VERDE, OUTPUT);

  digitalWrite(ROSSO, LOW);    // 
  digitalWrite(VERDE, HIGH);
  digitalWrite(BLU, HIGH);
  
 // Serial.begin(9600);  // inizializza il monitor seriale nel caso si utilizza phyphox il monitor seriale deve essere escluso
 
  pinMode(Supply, OUTPUT);      //Dichiarazione del pin 8 di carica e scarica
  digitalWrite(Supply, LOW);    //voltaggio 0 V sul pin di carica
  tau = resistenza * capacita;
  PhyphoxBLE::start(board_name); // Il nome della scheda può essere cambiato all'inizio del programma, attiva la conversazione bluetooth
  PhyphoxBLE::configHandler = &receivedData; // routine per mandare i comandi sulla scheda
  delay(1000);
}

void loop() {
 if (PhyphoxBLE::currentConnections > 0) {
    digitalWrite(ROSSO, HIGH);
    digitalWrite(VERDE, LOW);
    digitalWrite(BLU, HIGH);
  } else {
    digitalWrite(ROSSO, LOW);
    digitalWrite(VERDE, HIGH);
    digitalWrite(BLU, HIGH);
  }
CaricaScarica();  
}

void receivedData() { // see Phyphox Arduino Library example
  if (PhyphoxBLE::currentConnections == 1) {
     initial_time = millis();  // se se la connessione è attiva prendi il tempo di inizio esperimento
  }
}

void CaricaScarica(){
  // *************Carico del condensatore
 digitalWrite(Supply,HIGH);   // sto caricando il condensatore
 tempo_iniziale = micros();
 // 
 for (int i = 0; i < passi; i ++) {       // campiono il tempo e il voltaggio in 100 passi
  tempo = micros();
  tensione = analogRead(A0)*3.3/1023.;
  first_difference_float = (tempo - tempo_iniziale )/1000000.0;
   PhyphoxBLE::write(first_difference_float, tensione);
   //Serial.print(tensione);
  //Serial.print(" ");
  //Serial.println((tempo - tempo_iniziale )/1000000.0);
  delay(int(tau*100));
  }
  //******** Scarico del condensatore
 digitalWrite(Supply, LOW);       // scarico il condensatore
 tempo_iniziale = micros();
  //
  for (int i = 0; i < passi; i ++) {       // campiono il tempo e il voltaggio in 100 passi
  tempo = micros();
  tensione = analogRead(A0)*3.3/1023.;
  first_difference_float = (tempo - tempo_iniziale )/1000000.0;
  PhyphoxBLE::write(first_difference_float, tensione);
   //Serial.print(tensione);
  //Serial.print(" ");
  //Serial.println((tempo - tempo_iniziale )/1000000.0);
  delay(int(tau*100));
  }
 }
