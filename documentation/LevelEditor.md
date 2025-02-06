# Level Editor Doku

## System Architecture
The LevelEditor UI is built of many classes that inherit from a QWidget.  
The Composition of these classes forms the Application.  
Appart from main.cpp and the QWidget classes, level editor also makes use of  
the SpriteProvider and EventHandler class.  
The SpriteProvider is initialized at the beginning of the program in the main function
and can later be used to retrieve any sprite (QPixmap) by tile id from anywhere in the application.  
The EventHandler class is the heart of the application. It allows any Class to emit or receive events from anywhere in the program.  
To receive Events from EventHandler a class needs to inherit from EventHandler and overwrite the event method for the event it wants to handle.  
The available event methods, as well as the info on which class emits which event method and which class overwrites which event method can be seen in the below diagramm.  
The formless diagramm below demonstrates how the classes form the Application.  
For more details on the individual classes, take a look at the documentation in their header files.
![Formless System Architecture Diagramm](./architecture.svg)

## Level Specification

The level is stored in two datasets named 'metadata', 'tilesarray' in the level hdf5 file (level.h5).  
The metadata dataset is in XML format. It provides the level height, width and name.  
The tilesarray dataset is a giant array<uint8_t> which defines the level map through an array of tile ids.

### 1. metadata dataset (XML)
```xml
<?xml version="1.0" encoding="ASCII"?>
<level>
    <width>20</width> <!-- Breite des Levels -->
    <height>20</height> <!-- Hoehe des Levels -->
    <name>Geiles Level<name><!-- Name des Levels -->
</level>
```

### 2. tilesarray dataset
The length of the tiles array is level width x level height (see level metadata).  
From the individual values inside the tiles array you can determine the IDs of the buildings and terrains.  
0 - 29 are terrain IDs => Mapping see enum in src/game/Tile.hpp  
30-49 are undefined  
50-79  are building/faction IDs:   
50 => faction ID 0, building ID 0  
51 => faction ID 0, building ID 1  
...  
55 => faction ID 1, building ID 0  
56 => faction ID 1, building ID 1  
57 => faction ID 1, building ID 2  
...  

In General:  
Let t be a value from the tiles array,    
in case t < 30: terrain ID = t  
in case t >= 50: faction ID = (t - 50) / 5   building ID = (t - 50) % 5  
t is defined as t = terrain ID for terrains  
and t = 50 + 5*faction Id + building ID for buildings 

Which faction id belongs to which faction and which building id belongs to
which building can be found in the enum inside src/game/building.hpp
