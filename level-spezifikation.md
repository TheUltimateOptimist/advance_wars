# Levelspezifikation

Das Level wird ueber 2 Datasets in der HDF5-Datei repraesentiert.  
Ein Dataset in XML Format, das die Level Metadaten (Breite, Hoehe, Dataset ID Tiles Array) angibt.  
Ein weiters Dataset, das letztlich ein array<uint8_t> ist, welches die Levelmap ueber ein Array von Tile IDs definiert.

## 1. XML Dataset mit Level Metadaten
```xml
<?xml version="1.0" encoding="ASCII"?>
<level>
    <width>20</width> <!-- Breite des Levels -->
    <height>20</height> <!-- Hoehe des Levels -->
    <mapid>levelmap</mapid> <!-- Dataset ID, unter der das Tiles Array gespeichert ist -->
</level>
```

## 2. Tiles Array
Das Tiles Array wird als array<uint8_t> in einem Dataset in der HDF5-Datei gespeichert.  
Die Laenge des Arrays ist Breite X Hoehe (kann man aus XML Leveldefinition entnehmen).  
Die einzelnen Werte in dem Array sind die IDs des entsprechenden Gebaudes/Unit/Terrains.  
Diese IDs sollten in der Spielspezifikation festgelegt sein.  
D.h. wenn man die Levelmap rendern will, liest man das Tiles Array und kann die IDs des TIles Arrays
dann den entsprechenden Gebaueden/Terrains/Units durch die Spielspezifikation zuordnen.  
Wichtig ist noch zu beachten, dass die Werte des Tiles Array Modulo 80 gerechnet werden muessen, um 
die korrekte ID zu berechnen.  
Angenommen ein Gebaude hat die ID 10. Dann wuerde es in dem Tiles Array als 10 gespeichert werden, wenn es neutral ist,
als 80 + 10 = 90 wenn es Spieler 1 gehoert und als 80 + 80 + 10 = 170 wenn es Spieler 2 gehoert.  
Das ist letztlich der Grund weshalb die Werte des Tiles Arrays immer Modulo 80 gerechnet werden muessen.  
So erhaelt man immer die gleiche ID, egal ob das Gebaeude neutral ist oder Spieler 1 bzw. 2 gehoert.  
Um zu ueberpruefen ob das Gebaeude neutral ist oder Spieler 1 bzw. 2 gehoert muss man wie folgt vorgehen:  
Ist der Wert im Tiles Array < 80 => neutral  
Ist der Wert               >= 80 => Spieler 1
Ist der Wert              >= 160 => Spieler 2

## Laden des Levels angesichts der Spezifikation
Um das Level zu laden, liest man das XML Dataset aus der HDF5-Datei und erhaelt so die Metadaten des Levels.  
Diesen Metadaten kann man die Levelbreite, Levelhoehe, und Dataset ID des Tiles Array entnehmen.  
Mit diesen Informationen kann man dann das Tiles Array aus der HDF5-Datei lesen.  
Aus den Werten im Tiles Array berechnet man die IDs der Gebaeude/Terrains/Units (Berechnung durch Modulo 80).  
Die IDs kann man mit Hilfe der Spielspezifikation dann korrekt zu ordnen und die Levelmap rendern.

