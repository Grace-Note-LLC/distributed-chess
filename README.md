# Junkfish

Junkfish is a distributed system of microcontrollers harvested from recycled smart devices working together to power a chess engine. Anyone can connect to the front-end web application and play a game against a variety of hardware products sourced from recycled IoT devices such as smart lightbulbs, switches, and cameras. We were inspired by the intersection of chess, distributed computing, and recycling, creating an engaging experience while promoting sustainability and reducing e-waste.

## Features
- **Frontend:** Built using TypeScript and React, providing an interactive chess interface.
- **Backend:** Developed in Golang for high performance, scalability, and efficient error handling.
- **Dispatcher:** Coordinates tasks across recycled devices using SPI interface on a Raspberry Pi.
- **Algorithm:** Uses Minimax with alpha-beta pruning for optimal performance.
- **Firmware:** Written in C to facilitate board evaluation and communication via SPI.

## Project Directory Structure
```
.
├── build
├── dispatcher
│   └── test
├── frontend
│   ├── dist
│   │   ├── assets
│   │   └── images
│   ├── node_modules
│   ├── public
│   │   └── images
│   └── src
│       ├── assets
│       ├── components
│       ├── pages
│       └── services
└── server
    └── handlers
```
- `frontend`: TypeScript and React-based front end of the project.
  - `dist`: Contains build files for assets and images.
  - `node_modules`: Includes all the dependencies for the front end.
  - `public`: Contains static images.
  - `src`: Source files for assets, components, pages, and services.
- `dispatcher`: Responsible for distributing computational tasks among recycled devices.
  - `test`: Contains testing files for dispatcher functionalities.
- `server`: Backend server code written in Golang.
  - `handlers`: Contains API functions for communication between front end and back end.

## What's Next for Junkfish
- **Beat Magnus Carlsen**: We plan to improve our chess engine until it can beat world champion-level players.