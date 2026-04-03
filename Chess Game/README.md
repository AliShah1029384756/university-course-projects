# Chess Game (Multiplayer)

![Course](https://img.shields.io/badge/Course-Computer%20Networks-0ea5e9)
![Type](https://img.shields.io/badge/Type-Real%20Time%20App-16a34a)

Real-time two-player chess implementation built to demonstrate networked gameplay, event-driven server communication, and rules validation.

## Problem Statement

Implement a playable multiplayer chess system where two remote users can connect, synchronize board state, and complete a valid game session.

## Solution Overview

This project uses Socket.IO for real-time communication and Chess.js for chess rule enforcement. The server coordinates move events while the frontend handles board interaction.

## Features

- Real-time move synchronization between players
- Chess rule validation via Chess.js
- WebSocket-based session updates
- Browser-based game interface

## Tech Stack

- Backend: Node.js, Socket.IO
- Rules Engine: Chess.js
- Frontend: HTML, CSS, JavaScript

## Setup

```bash
npm install
```

## Usage

```bash
npm start
```

Open the client and connect two players to test real-time gameplay.
Default port: `3000`.

## Project Structure

```text
Chess Game/
|- server.js
|- package.json
|- public/
|  |- index.html
|  |- js/script.js
|  \- ...
|- Commands.txt
```

## Limitations

- Focused on coursework requirements and local execution.
- Matchmaking/lobby and persistent game history are limited.

## Future Improvements

- Add room/lobby management for multiple games.
- Add reconnect handling and session recovery.
- Add move history export and replay mode.

## License

Currently portfolio and coursework use. Add an explicit LICENSE file for open reuse.

## Contact

- Maintainer: Syed Muhammad Ali Naqvi
- GitHub: https://github.com/AliShah1029384756
- LinkedIn: https://linkedin.com/in/ali-naqvi-1a9576331
