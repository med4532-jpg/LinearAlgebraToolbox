# 🧮 Interactive Matrix Toolbox (C & Raylib)

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
