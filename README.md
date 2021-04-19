
<h1 align="center" style="color:white; background-color:black">Garatéa 3</h1>
<h4 align="center">Our aerospace probes are attached to balloons filled with helium, carrying astrobiological experiments to the stratosphere, which is crucial so we can have a better understanding about how organisms can survive in space.</h4>

<p align="center">
	<a href="http://zenith.eesc.usp.br/">
    <img src="https://img.shields.io/badge/Zenith-Embarcados-black?style=for-the-badge"/>
    </a>
    <a href="https://eesc.usp.br/">
    <img src="https://img.shields.io/badge/Linked%20to-EESC--USP-black?style=for-the-badge"/>
    </a>
    <a href="https://github.com/zenitheesc/garatea3/blob/main/LICENSE">
    <img src="https://img.shields.io/github/license/zenitheesc/garatea3?style=for-the-badge"/>
    </a>
    <a href="https://github.com/zenitheesc/garatea2/issues">
    <img src="https://img.shields.io/github/issues/zenitheesc/garatea3?style=for-the-badge"/>
    </a>
    <a href="https://github.com/zenitheesc/garatea3/commits/main">
    <img src="https://img.shields.io/github/commit-activity/m/zenitheesc/garatea3?style=for-the-badge">
    </a>
    <a href="https://github.com/zenitheesc/garatea3/graphs/contributors">
    <img src="https://img.shields.io/github/contributors/zenitheesc/garatea3?style=for-the-badge"/>
    </a>
    <a href="https://github.com/zenitheesc/garatea3/commits/main">
    <img src="https://img.shields.io/github/last-commit/zenitheesc/garatea3?style=for-the-badge"/>
    </a>
    <a href="https://github.com/zenitheesc/garatea3/issues">
    <img src="https://img.shields.io/github/issues-raw/zenitheesc/garatea3?style=for-the-badge" />
    </a>
    <a href="https://github.com/zenitheesc/garatea3/pulls">
    <img src = "https://img.shields.io/github/issues-pr-raw/zenitheesc/garatea3?style=for-the-badge">
    </a>
</p>

<p align="center">
    <a href="#what-are-aerospace-probes">What are aerospace probes?</a> •
    <a href="#project">Project</a> •
    <a href="#details">Details</a> 
</p>

## What are aerospace probes?

 Our aerospace probes are balloons filled with helium, carrying astrobiological experiments to the stratosphere, which is crucial so we can have a better understanding about how organisms can survive in space.

Although they are part of a greater scientific experiment, the probes themselves are also fascinating from the point of view of embedded systems development. Our probes are equiped with microcontrolers, radios, sensors and cameras; every mission we aim to improve our systems so we can make the next mission even more efficient.

And, to sustain all our experiments and systems, we build reliable and reusable structures, that let us go to space multiple times. 

## Project

Garatéa 3 is our most awarded Space Probe, winning 3rd Place in Best Design in the **Global Space Balloon Challenge 2018**.	This Space Probe was also a highlight at **Campus Party**. 

We continue to use the Arduino Toolchain to develop software, but we grandly improve the hardware. Our mainboard was made with 2 Atmega microcontrollers (Master/Slave) communicating through I2C. The Master was connected to the most relevant acquisition sensors, such as GPS, pressure sensor, DHT, and barometer. The data read by the Master is sent to the Slave and the Slave receive sent them through LoRa and saved them in physical memory (EEPROM and SD card). The Slave also controlled the servomotor.

Our structure is based on a **hypercube**. The external connectors were built with white ABS filament and the supports were made with a PLA filament. The center cube is made with carbon fiber plates and the external cube with carbon fiber tubes connected by the 3D printed parts.
 
### Component List

- **Main Atmega:**
    - GPS (U-Blox NEO MAX-M8Q)
    - GPS (Adafruit Ultimate Breakout)
    - Barometer (BMP180)
    - DHT sensor
    - LM1117 Regulator 
    - 7805 Regulator
- **Slave Atmega:**
    - LoRa
    - Radiometrix
    - Servomotor
    - SD card
    - EEPROM


## Details
| Launch Date |Max Altitude|Experiments|Awards
|--|--|--|--|
| 22/04/2018 | 31,6 km |Experiments with yeasts, fungi and protozoa|3rd Place in Best Design (GSBC 2018)

| | | |
|:-------------------------:|:-------------------------:|:-------------------------:|
|<img width="1604" alt="" src="https://raw.githubusercontent.com/zenitheesc/zenith-website/develop/public/images/Projetos/SondasAeroespaciais/Garat%C3%A9a-III/photo9.webp"> |  <img width="1604" alt="" src="https://raw.githubusercontent.com/zenitheesc/zenith-website/develop/public/images/Projetos/SondasAeroespaciais/Garat%C3%A9a-III/photo2.webp">|<img width="1604" alt="" src="https://raw.githubusercontent.com/zenitheesc/zenith-website/develop/public/images/Projetos/SondasAeroespaciais/Garat%C3%A9a-III/photo3.webp">|
|<img width="1604" alt="" src="https://raw.githubusercontent.com/zenitheesc/zenith-website/develop/public/images/Projetos/SondasAeroespaciais/Garat%C3%A9a-III/photo4.webp">  | <img width="1604" alt="" src="https://raw.githubusercontent.com/zenitheesc/zenith-website/develop/public/images/Projetos/SondasAeroespaciais/Garat%C3%A9a-III/photo5.webp">| <img width="1604" alt="" src="https://raw.githubusercontent.com/zenitheesc/zenith-website/develop/public/images/Projetos/SondasAeroespaciais/Garat%C3%A9a-III/photo6.webp">| <img width="1604" alt="" src="https://raw.githubusercontent.com/zenitheesc/zenith-website/develop/public/images/Projetos/SondasAeroespaciais/Garat%C3%A9a-III/photo7.webp">|

---

<p align="center">
    <a href="http://zenith.eesc.usp.br">
    <img src="https://img.shields.io/badge/Check%20out-Zenith's Oficial Website-black?style=for-the-badge" />
    </a> 
    <a href="https://www.facebook.com/zenitheesc">
    <img src="https://img.shields.io/badge/Like%20us%20on-facebook-blue?style=for-the-badge"/>
    </a> 
    <a href="https://www.instagram.com/zenith_eesc/">
    <img src="https://img.shields.io/badge/Follow%20us%20on-Instagram-red?style=for-the-badge"/>
    </a>

</p>
<p align = "center">
<a href="zenith.eesc@gmail.com">zenith.eesc@gmail.com</a>
</p>
