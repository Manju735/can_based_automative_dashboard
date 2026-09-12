<div align="center">

# 🚗 CAN-Based Automotive Dashboard

**A 3-ECU instrument cluster that talks over CAN bus — just like a real car.**

![Language](https://img.shields.io/badge/Language-C-blue?style=for-the-badge&logo=c)
![Platform](https://img.shields.io/badge/MCU-PIC18-orange?style=for-the-badge&logo=microchip)
![Protocol](https://img.shields.io/badge/Protocol-CAN%20Bus-yellow?style=for-the-badge)
![IDE](https://img.shields.io/badge/IDE-MPLAB%20X-red?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Active-brightgreen?style=for-the-badge)

<br/>

> A multi-node embedded systems project where three PIC18 microcontrollers exchange live **speed, gear, RPM, and turn-indicator** data over a shared **CAN bus**, and a dedicated dashboard ECU renders it on a 16x2 character LCD — complete with turn-signal LEDs and a warning buzzer.

</div>

---

## 📌 Table of Contents

- [About the Project](#-about-the-project)
- [System Architecture](#-system-architecture)
- [How It Works](#-how-it-works)
- [CAN Message Map](#-can-message-map)
- [Hardware](#-hardware)
- [Project Structure](#-project-structure)
- [Getting Started](#-getting-started)
  - [Prerequisites](#prerequisites)
  - [Build & Flash](#build--flash)
- [Wiring the CAN Bus](#-wiring-the-can-bus)
- [Concepts Used](#-concepts-used)
- [Roadmap](#-roadmap)
- [Author](#-author)

---

## 🧠 About the Project

This project simulates a real automotive instrument cluster using **three independent PIC18 ECUs** communicating over the microcontroller's built-in **ECAN (Enhanced CAN) module**.

- **ECU1** reads a potentiometer (speed) and a 4-key keypad (gear), and transmits both over CAN
- **ECU2** reads a potentiometer (RPM) and a 4-key keypad (turn indicators), and transmits both over CAN
- **ECU3** listens to the bus, decodes every message by ID, and drives a **16x2 LCD dashboard**, **turn-signal LEDs**, and a **buzzer**
- All nodes share standard 11-bit CAN identifiers and run at CAN timing tuned for an 8 MHz oscillator
- Built entirely in bare-metal C using the Microchip **XC8** compiler — no RTOS, no external libraries

---

## 🏗️ System Architecture

```
   ┌────────────────┐        ┌────────────────┐
   │      ECU1       │        │      ECU2       │
   │  Speed + Gear    │        │  RPM + Indicator │
   │    (Sender)      │        │    (Sender)      │
   └────────┬─────────┘        └────────┬─────────┘
            │                           │
            │        CAN BUS            │
            └─────────────┬─────────────┘
                          │
                 ┌────────┴─────────┐
                 │       ECU3        │
                 │  Dashboard Node   │
                 │    (Receiver)     │
                 │                   │
                 │  📟 16x2 LCD       │
                 │  💡 Indicator LEDs │
                 │  🔊 Buzzer         │
                 └───────────────────┘
```

---

## ⚙️ How It Works

```
ECU1 / ECU2                         ECU3
────────────                        ─────────────────────
Read sensor (ADC / keypad)          Poll CAN receive buffer
        │                                   │
        ▼                                   ▼
Build CAN frame (ID + data)   ──►   Identify message by CAN ID
        │                                   │
        ▼                                   ▼
Transmit on shared bus               Dispatch to handler:
                                      • handle_speed_data()
                                      • handle_gear_data()
                                      • handle_rpm_data()
                                      • handle_indicator_data()
                                             │
                                             ▼
                                   Update LCD / LEDs / Buzzer
```

**Timer0** on ECU3 drives the blink timing used for turn-signal flashing and the reverse-gear buzzer.

---

## 📡 CAN Message Map

| Signal | Message ID | Producer | Consumer |
|--------|:----------:|----------|----------|
| Speed | `0x10` | ECU1 | ECU3 |
| Gear | `0x20` | ECU1 | ECU3 |
| RPM | `0x30` | ECU2 | ECU3 |
| Engine Temp *(reserved, not yet implemented)* | `0x40` | — | — |
| Turn Indicator | `0x50` | ECU2 | ECU3 |

---

## 🔧 Hardware

| Component | Details |
|-----------|---------|
| **MCU** | Microchip PIC18 (XC8 / MPLAB X, ECAN module) |
| **CAN Transceiver** | e.g. MCP2551 / TJA1050 (external, one per node) |
| **Speed / RPM Input** | Potentiometer → ADC channel `AN4` |
| **Gear / Indicator Input** | 4-button digital keypad on `PORTC` |
| **Display** | 16x2 character LCD (CLCD), driven by ECU3 |
| **Indicator LEDs** | `PORTB` |
| **Buzzer** | `RE0` |
| **CAN Timing** | 8 MHz oscillator (`BRGCON1/2/3`) |

---

## 📁 Project Structure

```
can_based_automative_dashboard/
│
├── ECU1/                       # Speed + Gear sender node
│   ├── main.c                   # Main loop — reads sensors, transmits CAN frames
│   ├── sensor.c / ecu1_sensor.h # Speed (ADC) + gear (keypad) logic
│   ├── can.c / can.h            # ECAN driver — init, transmit, receive
│   ├── adc.c / adc.h            # ADC driver
│   ├── digital_keypad.c / .h    # Keypad read logic
│   ├── uart.c / uart.h          # UART driver (optional debug output)
│   └── msg_id.h                 # Shared CAN message ID definitions
│
├── ECU2/                       # RPM + Indicator sender node
│   ├── main.c
│   ├── ecu2_sensor.c / .h       # RPM (ADC) + indicator (keypad) logic
│   ├── can.c / can.h
│   ├── adc.c / adc.h
│   ├── digital_keypad.c / .h
│   ├── uart.c / uart.h
│   └── msg_id.h
│
├── ECU3/                       # Dashboard / display node
│   ├── main.c                   # Inits CLCD, LEDs, buzzer, CAN, timer
│   ├── msg_handler.c / .h       # Decodes CAN frames, updates LCD/LEDs/buzzer
│   ├── can.c / can.h
│   ├── clcd.c / clcd.h          # 16x2 character LCD driver
│   ├── timer0.c / timer0.h      # Blink timing for indicators & buzzer
│   ├── isr.c / isr.h            # Interrupt service routines
│   ├── uart.c / uart.h
│   └── msg_id.h
│
└── README.md
```

---

## 🚀 Getting Started

### Prerequisites

- [MPLAB X IDE](https://www.microchip.com/mplab/mplab-x-ide)
- [XC8 Compiler](https://www.microchip.com/mplab/compilers)
- 3x PIC18-series development boards
- 3x CAN transceivers (e.g. MCP2551) + a shared CAN bus with 120 Ω termination at both ends
- PICkit (or compatible) programmer/debugger

### Build & Flash

```bash
# 1. Open each ECU folder as a separate MPLAB X project
#    (ECU1, ECU2, ECU3), or create three new projects and
#    add the corresponding source/header files from each folder.

# 2. Select your target PIC18 device and programmer in each project.

# 3. Build and flash:
ECU1  →  Board 1   (Speed + Gear sender)
ECU2  →  Board 2   (RPM + Indicator sender)
ECU3  →  Board 3   (Dashboard receiver)

# 4. Wire all three boards to a shared CAN bus (see below), power up,
#    and watch the LCD update live as you turn the pots / press keys.
```

---

## 🔌 Wiring the CAN Bus

| Signal | Pin (all ECUs) |
|--------|:---------------:|
| CAN TX | `RB2` |
| CAN RX | `RB3` |

```
   ECU1 ──┐
          ├── CAN_H ────────────── CAN_H ──┐
   ECU2 ──┤                                 ├── ECU3
          └── CAN_L ────────────── CAN_L ──┘
      120Ω                              120Ω
   (termination)                    (termination)
```

Each node needs its own CAN transceiver (TX/RX from the PIC ↔ CAN_H/CAN_L on the bus); the bus itself needs 120 Ω resistors at each physical end.

---

## 💡 Concepts Used

| Concept | Applied In |
|---------|------------|
| **CAN Protocol (ECAN module)** | Message framing, standard IDs, TX/RX buffers |
| **ADC** | Reading potentiometers for speed & RPM simulation |
| **Digital I/O / Keypad Scanning** | Gear selection & turn-indicator input |
| **Character LCD Interfacing** | Real-time dashboard display on ECU3 |
| **Timers & Interrupts** | Blink timing for indicators and buzzer |
| **Modular Embedded C** | Shared `msg_id.h`, per-node drivers (`can.c`, `adc.c`, etc.) |

---

## 🗺️ Roadmap

- [ ] Implement `ENG_TEMP_MSG_ID` (0x40) with an actual temperature sensor
- [ ] Add CAN acceptance filtering instead of accepting all standard IDs
- [ ] Add UART debug logging (driver present, currently disabled)
- [ ] Add a CAN bus fault / bus-off indicator on the dashboard

---

## 👤 Author

**H Manjunatha**

- 🐙 GitHub: [@Manju735](https://github.com/Manju735)

---

<div align="center">

⭐ If you found this project helpful, give it a **star** on GitHub!

</div>
