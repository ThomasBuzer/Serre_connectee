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

int redLed = 33;
int whiteLed = 4;
int pumpPin = 13;
int fanPin = 12;
int sensorPower = 2;
int lightPin = 14;
int dhtPin = 15;

String GOOGLE_SCRIPT_ID = "AKfycbx-9i9Zu6WyBMAfQey_Hle-L-dqW94WVCNc_f8sVzF-vi08NC7xyscnGzp_XL6DOj6w";    // change Gscript ID
