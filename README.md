# 🧮 Interactive Linear Algebra Toolbox (C & Raylib)
 
Eine interaktive Desktop-Anwendung zur Arbeit mit **Matrizen und Vektoren**, entwickelt in **reinem C** und visualisiert mit der **Raylib**-Grafikbibliothek. Das Tool bietet eine benutzerfreundliche Oberfläche zur Eingabe, Bearbeitung und Echtzeit-Berechnung komplexer linearer Algebra-Probleme.
 
![C](https://img.shields.io/badge/Language-C-blue.svg)
![Raylib](https://img.shields.io/badge/GUI-Raylib-red.svg)
![Build](https://img.shields.io/badge/Build-gcc-green.svg)
 
---
 
## 🚀 Features
 
Die Toolbox ist modular aufgebaut und bietet verschiedene spezialisierte Registerkarten (Tabs):
 
### 1. ➕➖✖️ Grundrechenarten (Rechnen-Tab)
* **Addition & Subtraktion** zweier Matrizen.
* **Matrix-Multiplikation** mit automatischer Dimensionsüberprüfung.
* **Schnelles Transponieren (T):** Spiegelt beide Eingabematrizen gleichzeitig an ihrer Hauptdiagonale über eine dedizierte Schaltfläche.
### 2. 🔍 Erweiterte Matrix-Operationen
* **Determinante (DET):** Berechnung für quadratische Matrizen.
* **Inverse (INV):** Berechnung und direkte visuelle Ausgabe der Kehrmatrix ($A^{-1}$).
* **Rang (RANK):** Bestimmung des Rangs einer Matrix.
* **Eigenwerte (EIG):** Numerische Berechnung der Eigenwerte.
### 3. 📐 Lineare Gleichungssysteme (Gauß-Verfahren)
* Vollständige Lösung von Gleichungssystemen mittels **Gauß-Verfahren**.
* Trennung von Koeffizientenmatrix ($A$) und dem Ergebnisvektor ($b$) im UI.
### 4. 💾 Dateiverwaltung (Öffnen / Speichern)
* **Sichern:** Speichert die aktuelle einfache Matrix und berechnete Ergebnisse formatiert in einer Textdatei ab.
* **Laden:** Liest gespeicherte Matrizen direkt wieder in die Benutzeroberfläche ein (unterstützt für die Tabs Determinante, Inverse, Rang und Eigenwerte).
---
 
## 🛠️ Installations- & Bauanleitung
 
Wähle die passende Anleitung für dein Betriebssystem, um das Projekt zu bauen und zu starten.
 
### 🐧 Linux (Ubuntu/Debian)
 
1. **Abhängigkeiten installieren:**
```bash
   sudo apt-get update
   sudo apt-get install libraylib-dev gcc make
```
 
2. **Projekt kompilieren:**
```bash
   gcc *.c -o matrixtoolbox $(pkg-config --libs --cflags raylib) -lm
```
 
3. **Anwendung starten:**
```bash
   ./matrixtoolbox
```
 
---
 
### 🍎 macOS
 
1. **Homebrew installieren** (falls noch nicht vorhanden):
```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```
 
2. **Abhängigkeiten installieren:**
```bash
   brew install raylib gcc make
```
 
3. **Projekt kompilieren:**
```bash
   gcc *.c -o matrixtoolbox $(pkg-config --libs --cflags raylib) -lm
```
   > Falls `pkg-config` die Raylib-Pfade nicht findet, kannst du sie auf Apple-Silicon-Macs manuell ergänzen:
   > ```bash
   > gcc *.c -o matrixtoolbox -I/opt/homebrew/include -L/opt/homebrew/lib -lraylib -framework Cocoa -framework IOKit -framework CoreVideo -framework OpenGL
   > ```
 
4. **Anwendung starten:**
```bash
   ./matrixtoolbox
```
 
---
 
### 🪟 Windows
 
Unter Windows empfiehlt sich **MSYS2**, da sich darüber Raylib und GCC am einfachsten installieren lassen.
 
1. **MSYS2 installieren:**
   Lade MSYS2 von [msys2.org](https://www.msys2.org/) herunter und installiere es.
2. **MSYS2-MinGW64-Terminal öffnen** (nicht das normale MSYS2-Terminal!) und Abhängigkeiten installieren:
```bash
   pacman -Syu
   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-raylib mingw-w64-x86_64-make
```
 
3. **Projekt kompilieren** (im Projektordner, ebenfalls im MSYS2-MinGW64-Terminal):
```bash
   gcc *.c -o matrixtoolbox.exe -lraylib -lopengl32 -lgdi32 -lwinmm
```
 
4. **Anwendung starten:**
```bash
   ./matrixtoolbox.exe
```
 
   > **Hinweis:** Falls du die `.exe` stattdessen per Doppelklick im Windows-Explorer startest, achte darauf, dass `daten_*.txt`-Dateien (aus der Öffnen/Speichern-Funktion) dann in demselben Ordner wie die `.exe` abgelegt werden.
 
   > **Alternative:** Wer lieber mit Visual Studio arbeitet, kann Raylib auch über [vcpkg](https://vcpkg.io/) installieren (`vcpkg install raylib`) und das Projekt als Visual-Studio-Projekt einbinden.
 
