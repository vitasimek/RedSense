# RedSense

### To be able to use new geda symbols create/edit gafrc file:
gedit ~/.gEDA/gafrc 
### And add the following line:
(component-library "Repos/RedSense/geda/symbols")

### Using Visual Studio Code for Arduino projects
The file arduino.json is used a project file. Only one sketch could be used and so, rename the one that is needed.
arduino.json -> arduino.receiver.json   // This deactivates receiver.ino sketch.
arduino.sender.json -> arduino.json     // This activates sender.ino sketch.
Stupid, stupid, stupid!

https://medium.com/home-wireless/use-visual-studio-code-for-arduino-2d0cf4c1760b