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
    <name>Geiles Level<name><!-- Name des Levels -->
</level>
```

## 2. Tiles Array
Das Tiles Array wird als array<uint8_t> in einem Dataset in der HDF5-Datei gespeichert.  
Die Laenge des Arrays ist Breite X Hoehe (kann man aus XML Leveldefinition entnehmen).  
Aus den einzelnen Werte in dem Array lassen sich die IDs der entsprechenden Gebaudes/Terrains ableiten.  
0 - 29 sind Terrain IDs => Zuordnung siehe entsprechendes Enum in Tile.hpp  
30-49 sind undefiniert  
50-79  sind Gebaeude IDs:  
50 => Faction ID 0, Gebaeude ID 0  
51 => Faction ID 0, Gebaeude ID 1  
...  
55 => Faction ID 1, Gebaeude ID 0  
56 => Faction ID 1, Gebaeude ID 1  
57 => Faction ID 1, Gebaeude ID 2  
...  
Allgemein:  
Sei t ein Wert im Tiles Array, dann gillt  
falls t < 30: Terrain ID = t  
falls t >= 50: Faction ID = (t - 50) / 5   Gebaeude ID = (t - 50) % 5  
t wird ermittelt mit entweder t = Terrain ID fuer Terrains  
oder t = 50 + 5*Faction Id + Gebaeude ID fuer Gebaeude