# Advanced Wars

## Build-Anleitung

### Linux/MacOS

1. Repository klonen:
   ```bash
   git clone https://github.com/username/project.git
   cd project
   ```

2. Build-Verzeichnis erstellen:
   ```bash
   mkdir build && cd build
   ```

3. CMake konfigurieren und Build ausführen:
   ```bash
   cmake ..
   cmake --build .
   ```

### Windows

1. Repository klonen:
   ```powershell
   git clone https://github.com/username/project.git
   cd project
   ```

2. Build-Verzeichnis erstellen:
   ```powershell
   mkdir build
   cd build
   ```

3. CMake konfigurieren und Build ausführen:
   ```powershell
   cmake ..
   cmake --build . --config Release
   ```

### Visual Studio

1. Repository klonen
2. Visual Studio öffnen
3. "Ordner öffnen" wählen und zum Projektverzeichnis navigieren
4. Visual Studio erkennt automatisch das CMake-Projekt
5. Build über "Build All" ausführen

## Build-Optionen

CMake kann mit verschiedenen Optionen konfiguriert werden:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release    # Release-Build
cmake .. -DCMAKE_BUILD_TYPE=Debug      # Debug-Build
```

## Prerequisites for Leveleditor
- QT 6: ```sudo apt install qt6-base-dev```