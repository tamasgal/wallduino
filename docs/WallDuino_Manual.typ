#set page(paper: "a5", margin: 1.5cm)
#set text(font: "Athelas", size: 10pt)
#set par(justify: true)

#align(center)[
  #text(size: 20pt, weight: "bold")[WallDuino]
  #v(0.3cm)
  #text(size: 14pt)[Open Source Ersatzboard für motorisierte WallWizard TV-Wandhalterungen]
  #v(0.1cm)
  #text(size: 12pt)[Open-source replacement board for Motorized WallWizard TV Wall Mounts]
  #v(0.5cm)
  #text(size: 12pt)[Tamas Gal - https://www.tamasgal.com]
  #v(0.5cm)
  #image(
    "images/Wallduino.png",
    width: 5cm
  )
  #v(0.3cm)
  #text(size: 9pt)[Bedienungsanleitung / User Manual]
  #v(0.5cm)
  #text(size: 8pt)[Custom DIY Mainboard Replacement]
]

#pagebreak()

// German Version
#align(center)[
  #text(size: 16pt, weight: "bold")[Bedienungsanleitung]
  #v(0.5cm)
]

== Einleitung

Dieses Handbuch beschreibt die Verwendung des *WallDuino*, einer selbstgebauten DIY-Platine, die als Ersatz für die originale WallWizard-Hauptplatine (P/N: 417-S3-001-00 CLO SYSTEMS 2010.12.25) dient. 

Der WallDuino ist *vollständig kompatibel* mit dem originalen Layout und der originalen Fernbedienung des WallWizard-Systems. Die Platine wurde entwickelt, um alle Funktionen der Originalplatine zu ersetzen und bietet zusätzlich die Möglichkeit zur Anpassung der Firmware.

#box(fill: rgb("#fff3cd"), inset: 10pt, radius: 4pt)[
  *Open Source Projekt:* Der WallDuino ist ein Open-Source-Projekt. Die Firmware und alle Designdateien finden Sie im offiziellen Git-Repository: \
  #link("https://github.com/tamasgal/wallduino")
]

== Übersicht

Der WallDuino steuert eine motorisierte TV-Wandhalterung, die es Ihnen ermöglicht, Ihren Fernseher horizontal nach links und rechts zu bewegen. Die Steuerung erfolgt bequem über die kompatible Fernbedienung.

== Fernbedienung

Die Fernbedienung verfügt über folgende Tasten:

#table(
  columns: (auto, 1fr),
  stroke: 0.5pt,
  [*Taste*], [*Funktion*],
  [◄ Links], [Bewegt die Halterung nach links],
  [► Rechts], [Bewegt die Halterung nach rechts],
  [■ Stop], [Stoppt die Bewegung sofort],
  [Preset 1], [Positionsspeicher 1],
  [Preset 2], [Positionsspeicher 2],
)

== Bedienung

=== Grundlegende Bewegung

*Links/Rechts bewegen:*
- Drücken Sie die Taste ◄ (Links) oder ► (Rechts)
- Die Halterung bewegt sich in die gewählte Richtung
- Die Bewegung stoppt automatisch bei Erreichen der Endposition

*Bewegung stoppen:*
- Drücken Sie die ■ (Stop)-Taste, um die Bewegung jederzeit zu unterbrechen

=== Preset-Positionen

Die Preset-Tasten ermöglichen es Ihnen, zwei Lieblingspositionen zu speichern und schnell anzufahren.

*Position anfahren:*
- Drücken Sie *einmal* auf Preset 1 oder Preset 2
- Die Halterung fährt automatisch zur gespeicherten Position

*Position speichern:*
- Bewegen Sie die Halterung zur gewünschten Position
- Drücken Sie *dreimal schnell hintereinander* auf Preset 1 oder Preset 2
- Die aktuelle Position ist nun gespeichert

#box(fill: rgb("#e8f4f8"), inset: 10pt, radius: 4pt)[
  *Hinweis:* Die Preset-Positionen bleiben auch nach dem Ausschalten gespeichert.
]

== Sicherheitshinweise

- Achten Sie darauf, dass sich keine Gegenstände oder Personen im Bewegungsbereich befinden
- Verwenden Sie die Stop-Taste bei unerwarteten Hindernissen
- Prüfen Sie regelmäßig die sichere Montage der Halterung
- Bei Fehlfunktionen trennen Sie das Gerät vom Strom und kontaktieren Sie den Kundendienst

== Firmware-Anpassung

Ein besonderer Vorteil des WallDuino ist die Möglichkeit, die Firmware nach eigenen Wünschen anzupassen. Dies ermöglicht es Ihnen:

*Zusätzliche Fernbedienungsbefehle hinzufügen:*
- Ungenutzte Tasten bestehender TV-Fernbedienungen verwenden
- Beliebige andere Fernbedienungen einlernen
- Eigene Befehle und Funktionen programmieren

*Anpassungsmöglichkeiten:*
- IR-Codes neuer Fernbedienungen hinzufügen
- Bewegungsgeschwindigkeit anpassen
- Zusätzliche Preset-Positionen definieren
- Spezielle Bewegungsabläufe programmieren

Die vollständige Firmware sowie Anleitungen zur Anpassung finden Sie im offiziellen Repository: \
#link("https://github.com/tamasgal/wallduino")

#pagebreak()

// English Version
#align(center)[
  #text(size: 16pt, weight: "bold")[User Manual]
  #v(0.5cm)
]

== Introduction

This manual describes the use of the *WallDuino*, a custom DIY board that serves as a replacement for the original WallWizard mainboard (P/N: 417-S3-001-00 CLO SYSTEMS 2010.12.25).

The WallDuino is *fully compatible* with the original layout and the original remote control of the WallWizard system. The board was designed to replace all functions of the original board and additionally offers the possibility to customize the firmware.

#box(fill: rgb("#fff3cd"), inset: 10pt, radius: 4pt)[
  *Open Source Project:* The WallDuino is an open source project. The firmware and all design files can be found in the official Git repository: \
  #link("https://github.com/tamasgal/wallduino")
]

== Overview

The WallDuino controls a motorized TV wall mount that allows you to move your television horizontally to the left and right. Control is conveniently managed via the compatible remote control.

== Remote Control

The remote control features the following buttons:

#table(
  columns: (auto, 1fr),
  stroke: 0.5pt,
  [*Button*], [*Function*],
  [◄ Left], [Moves the mount to the left],
  [► Right], [Moves the mount to the right],
  [■ Stop], [Stops the movement immediately],
  [Preset 1], [Position memory 1],
  [Preset 2], [Position memory 2],
)

== Operation

=== Basic Movement

*Moving Left/Right:*
- Press the ◄ (Left) or ► (Right) button
- The mount will move in the selected direction
- Movement stops automatically when reaching the end position

*Stopping Movement:*
- Press the ■ (Stop) button to interrupt movement at any time

=== Preset Positions

The preset buttons allow you to save two favorite positions and navigate to them quickly.

*Navigate to Position:*
- Press Preset 1 or Preset 2 *once*
- The mount will automatically move to the saved position

*Save Position:*
- Move the mount to the desired position
- Press Preset 1 or Preset 2 *three times quickly in succession*
- The current position is now saved

#box(fill: rgb("#e8f4f8"), inset: 10pt, radius: 4pt)[
  *Note:* Preset positions remain saved even after powering off.
]

== Safety Instructions

- Ensure no objects or persons are in the movement range
- Use the Stop button if unexpected obstacles appear
- Regularly check the secure mounting of the bracket
- In case of malfunction, disconnect the device from power and contact customer service

== Firmware Customization

A special advantage of the WallDuino is the ability to customize the firmware according to your own preferences. This allows you to:

*Add Additional Remote Commands:*
- Use unused buttons from existing TV remotes
- Learn any other remote controls
- Program custom commands and functions

*Customization Options:*
- Add IR codes from new remote controls
- Adjust movement speed
- Define additional preset positions
- Program special movement sequences

The complete firmware and instructions for customization can be found in the official repository: \
#link("https://github.com/tamasgal/wallduino")

#pagebreak()

// Appendix
#align(center)[
  #text(size: 16pt, weight: "bold")[Anhang / Appendix]
  #v(0.5cm)
]

== PCB-Dokumentation / PCB Documentation

Die folgenden Seiten enthalten detaillierte technische Zeichnungen und Schaltpläne der WallDuino-Platine.

The following pages contain detailed technical drawings and schematics of the WallDuino board.

#pagebreak()

=== v1.0 (Rev A)

// Schematic
// #align(center, text(size: 12pt, weight: "bold")[Schaltplan / Schematic])
// #v(0.5cm)
#rotate(90deg, origin: center, reflow: true)[
  #image("schematics/WallDuino v1.pdf", width: 95%, height: 95%)
]

#pagebreak()

// PCB Layer - Front with Silkscreen
#align(center, text(size: 12pt, weight: "bold")[Vorderseite mit Beschriftung / Front Copper with Silkscreen])
#v(0.5cm)
#image("pcb/F.Cu.pdf", width: 100%)

#pagebreak()

// PCB Layer - Front without Silkscreen
#align(center, text(size: 12pt, weight: "bold")[Vorderseite ohne Beschriftung / Front Copper without Silkscreen])
#v(0.5cm)
#image("pcb/F.Cu No Silkscreen.pdf", width: 100%)

#pagebreak()

// PCB Layer - Back with Silkscreen
#align(center, text(size: 12pt, weight: "bold")[Rückseite mit Beschriftung / Back Copper with Silkscreen])
#v(0.5cm)
#image("pcb/B.Cu.pdf", width: 100%)

#pagebreak()

// PCB Layer - Back without Silkscreen
#align(center, text(size: 12pt, weight: "bold")[Rückseite ohne Beschriftung / Back Copper without Silkscreen])
#v(0.5cm)
#image("pcb/B.Cu No Silkscreen.pdf", width: 100%)

#pagebreak()

=== v1.0 (Rev B)

// #align(center, text(size: 12pt, weight: "bold")[Schaltplan / Schematic])
// #v(0.5cm)
#rotate(90deg, origin: center, reflow: true)[
  #image("schematics/WallDuino v1 Rev B.pdf", width: 95%, height: 95%, fit: "contain")
]

#pagebreak()

// PCB Layer - Front with Silkscreen
#align(center, text(size: 12pt, weight: "bold")[Vorderseite mit Beschriftung / Front Copper with Silkscreen])
#v(0.5cm)
#image("pcb/F.Cu v1.0 Rev B.pdf", width: 100%)

#pagebreak()

// PCB Layer - Front without Silkscreen
#align(center, text(size: 12pt, weight: "bold")[Vorderseite ohne Beschriftung / Front Copper without Silkscreen])
#v(0.5cm)
#image("pcb/F.Cu v1.0 Rev B No Silkscreen.pdf", width: 100%)

#pagebreak()

// PCB Layer - Back with Silkscreen
#align(center, text(size: 12pt, weight: "bold")[Rückseite mit Beschriftung / Back Copper with Silkscreen])
#v(0.5cm)
#image("pcb/B.Cu v1.0 Rev B.pdf", width: 100%)

#pagebreak()

// PCB Layer - Back without Silkscreen
#align(center, text(size: 12pt, weight: "bold")[Rückseite ohne Beschriftung / Back Copper without Silkscreen])
#v(0.5cm)
#image("pcb/B.Cu v1.0 Rev B No Silkscreen.pdf", width: 100%)

