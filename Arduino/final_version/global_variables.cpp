#include "fonctions.h"

int DUREE_DEEPSLEEP;
int LED;
float MAX_HUMIDITY;
float MIN_HUMIDITY;
float MAX_TEMP;
float MIN_TEMP;
int FORCE_POMPE;
int ARROSAGE_AUTOMATIQUE;
int DUREE_AROSAGE;
int HEURE_AROSAGE;
int TIMELAPSE;
int FREQUENCE_TIMELAPSE;
int FORCE_PHOTO;
int LED_PHOTO;

int redLed = 33;
int whiteLed = 4;
int pumpPin = 13;
int fanPin = 12;
int sensorPower = 2;
int lightPin = 14;
int dhtPin = 15;

String GOOGLE_SCRIPT_ID = "AKfycbwszFNGswHs3zNiTl-HkM1p3AgxD0_O8TumCzW3lgq2pPJVpEhR-lWcySEVYnKy1w";    // change Gscript ID
