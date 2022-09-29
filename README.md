# Serre_connectee

## Flash un nouveau programme 

**Débrancher l'alimentation** de la carte.

Utiliser le convertisseur USB to TTL : 
- GND -> GND
- RXD -> U0T
- TXD -> U0R

utiliser un **jumper** pour connecter IO0 et GND sur l'ESP32.

<div align="center">
<img src="./images/televerser.jpg" width="400">
</div>

Brancher l'**USB** au PC

Connecter l'**alimentation** de l'ESP32.

**Téléverser le programme sur l'ESP32 avec l'IDE arduino**

**Enlever le jumper** entre IO0 et GND

Débrancher l'alimentation et la rebrancher.

## liste des inspirations

sauvegarde des images dans ggdrive
https://github.com/gsampallo/esp32cam-gdrive

lire le spreadsheet
https://www.youtube.com/watch?v=0LoeaewIAdY

avoir l'heure depuis l'internet
https://randomnerdtutorials.com/esp32-date-time-ntp-client-server-arduino/

requêtes internet de base
https://randomnerdtutorials.com/esp32-http-get-post-arduino/#http-post

sleepmode
https://www.mischianti.org/2021/03/10/esp32-power-saving-modem-and-light-sleep-2/





