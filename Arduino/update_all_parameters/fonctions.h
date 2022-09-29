#ifndef _FONCTIONS_H_
#define _FONCTIONS_H_

#include <Arduino.h>

extern int DUREE_DEEPSLEEP;
extern int LED;
extern float MAX_HUMIDITY;
extern float MIN_HUMIDITY;
extern float MAX_TEMP;
extern float MIN_TEMP;
extern int FORCE_POMPE;
extern int ARROSAGE_AUTOMATIQUE;
extern int DUREE_AROSAGE;
extern int HEURE_AROSAGE;
extern int TIMELAPSE;
extern int FREQUENCE_TIMELAPSE;
extern int FORCE_PHOTO;

extern int redLed;
extern int whiteLed;
extern int fanPin;
extern int pumpPin;
extern int sensorPower;
extern int lightPin;
extern int dhtPin;

extern String GOOGLE_SCRIPT_ID;


extern void updateParameters();
extern void sendVariables(float temp, float humidity, float light);
extern int getHour();
#endif
