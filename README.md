# Advanced Wars

## clang-tidy

- Um `clang-tidy` für eine einzelne Datei (z.B. `main.cpp`) auszuführen:

   `clang-tidy src/game/main.cpp -header-filter="src/game/main\.cpp" -p build/`

- Um `clang-tidy` für das ganze Projekt auszuführen und die Warnings in die Datei `clang-tidy-warnings.txt` zu schreiben:

   `find src/game -name '*.cpp' -o -name '*.hpp' | xargs clang-tidy -header-filter="src/game/.*" -p build/ 1> clang-tidy-warnings.txt`
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

#### Falls Syntax errors

1. Erstelle .vscode/c_cpp_properties.json Datei
2. Füge die folgende JSON so oder so ähnlich ein:

```json
{
    "configurations": [
        {
            "name": "Fedora",
            "includePath": [
                "/usr/include",
                "/usr/include/SDL2"
            ],
            "defines": [],
            "intelliSenseMode": "linux-gcc-x64",
            "compilerPath": "/usr/bin/gcc",
            "cStandard": "c17",
            "cppStandard": "c++17"
        }
    ],
    "version": 4
}
```

## Build-Optionen

CMake kann mit verschiedenen Optionen konfiguriert werden:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release    # Release-Build
cmake .. -DCMAKE_BUILD_TYPE=Debug      # Debug-Build
```
