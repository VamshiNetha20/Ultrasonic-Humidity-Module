Smart Humidifier Code - ATmega328P
==================================

Author: G. Vamshi Krishna

This code controls an ultrasonic humidifier using an ATmega328P microcontroller.
The system accepts control input from either a push button or an HC-05 Bluetooth module and provides visual feedback using a status LED.

Features:
---------
- Push button toggles humidifier ON/OFF (connected to PD2, INT0)
- Bluetooth commands '1' and '0' received via HC-05 control ON/OFF (PD0/PD1)
- Status LED indicates active state (ON = LED ON)
- Uses interrupts for both external input and serial reception

Pin Connections:
----------------
- PD2 (INT0)   → Push Button Input
- PD3          → MOSFET Gate (controls humidifier)
- PD4          → Status LED
- PD0 (RXD)    ← HC-05 TX
- PD1 (TXD)    → HC-05 RX (via 1k resistor)

Flashing Instructions:
----------------------
- Use a USBasp or Arduino-as-ISP via the 6-pin ISP header
- Compile and flash with AVR-GCC or Arduino IDE (as C sketch)

Note:
-----
- No separate UART header is used; all serial communication is handled exclusively by HC-05.