# Morse Code Translator

An interactive Morse Code Translator built on an Arduino-compatible board, featuring text input/output, auditory feedback, and an engaging game mode to practice Morse code skills.

---

## Table of Contents
- [About](#about)
- [Features](#features)
- [Hardware Components](#hardware-components)
- [Software Dependencies](#software-dependencies)
- [How It Works](#how-it-works)
- [Usage](#usage)
- [Future Improvements](#future-improvements)
- [Project Structure](#project-structure)
- [License](#license)
- [Author](#author)

---

## About
This project allows users to:
- Input Morse code via a button interface.
- Decode Morse code in real-time, with translations shown on an OLED screen.
- Play an interactive game mode, testing Morse code proficiency by decoding random words.
  
---

## Features
- Translate Morse code in real-time
- OLED display shows decoded text
- Buzzer gives auditory feedback (short and long beeps)
- Game mode to guess random words via Morse code
- LED feedback for correct/incorrect answers
- Written in Arduino C/C++
- Easy-to-use hardware interface

---

## Hardware Components
| Component                  | Description                                   |
|----------------------------|-----------------------------------------------|
| Arduino-compatible board   | Jade U1+ / Groundstudio                      |
| OLED display               | SSD1306                                      |
| Buttons (2)                | One for Morse input, one for Game Mode toggle |
| Buzzer                     | Provides Morse sound feedback                |
| LED                        | Indicates correct guess in Game Mode         |
| Breadboard and jumper wires| For circuit connections                      |

---

## Software Dependencies
- Adafruit_GFX.h
- Adafruit_SSD1306.h
- Wire.h

---

## How It Works

### Normal Mode (Default)
- Short button press (<500ms): Enters a dot `.`
- Long button press (≥500ms): Enters a dash `-`
- No input for 2 seconds: Automatically decodes the Morse sequence
- OLED display: Shows the decoded text
- Buzzer: Plays a short beep for dots and a long beep for dashes

### Game Mode
- Activated by pressing the Game Mode button
- A random target word appears on the OLED screen
- Input Morse code for each letter of the target word
- LED feedback:
  - Correct: LED blinks and a success message is displayed
  - Incorrect: Error message is displayed
- Toggle back to Normal Mode with the Game Mode button


---

## Future Improvements
- Implement letter deletion on input errors
- Add another LED for error notification
- Expand game mode with difficulty levels and time challenges

---

## Project Structure
