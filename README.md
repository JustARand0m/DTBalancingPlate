# Balancing Plate
## Aufbau
-> Todo
## ...Todo
-> Todo Beschreibung

#Raspberry Pi config
## UART
first the serial interface has to be enabled,
type:
```
raspi-config
```
into the shell of the raspberry pi.
In there go to `Interfacing Options` and than to 
`Serial`, than enable this option. <br/><br/>
Now the terminal needs to be disconnected of serial0 on boot
```
sudo vim /boot/cmdline.txt
```
delete the line `console=serial0,115200`
(older versions can have AMA0 here)
<br/><br/>
(Optional): <br/>
Um zu testen ob UART geht,
TX und RX pin des Raspberry pis verbinden, dann
minicom installieren und ausfuehren:
```
sudo apt install minicom
minicom -b 9600 -o -D /dev/serial0
```
Falls man nun Buchstaben tippt sollten diese auch erkannt werden.
Falls UART noch nicht funktioniert, sollte die Tastatureingabe nicht erkannt 
werden.

