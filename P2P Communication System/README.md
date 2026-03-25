# P2P Communication System

A React + Node.js peer-to-peer communication application for local-network messaging and file sharing.

## Overview

- Real-time peer communication over LAN
- Peer-to-peer file sharing workflow
- Signaling server for peer connection setup
- Optional Electron entry point

## Tech Stack

- Frontend: React 18, styled-components, react-icons
- Peer-to-peer: peerjs
- Backend: Node.js, Express, peer (ExpressPeerServer), cors
- Optional desktop shell: Electron (`main.js`)

## Project Structure

```text
P2P Communication System/
|- src/                 # Frontend React source
|- public/              # Static frontend assets
|- server.js            # Express + PeerJS signaling server
|- main.js              # Electron entry point
|- package.json
|- package-lock.json
```

## Prerequisites

- Node.js 18+ recommended
- npm 9+ recommended

## Installation

From the project directory:

```bash
npm install
```

## Running the App

### Development (recommended)

Runs React (3000) and signaling server (3001):

```bash
npm run dev
```

### Frontend only

```bash
npm start
```

### Signaling server only

```bash
npm run start-server
```

### Production build + server

```bash
npm run build
npm run deploy
```

## Network Endpoints

- React app: `http://localhost:3000`
- Signaling server: `http://localhost:3001`
- PeerJS endpoint: `http://localhost:3001/peerjs/myapp`
- Health check: `http://localhost:3001/health`

The server logs all available local IPv4 addresses so peers on the same network can connect.

## Available Scripts

- `npm start`: Start React development server on port 3000.
- `npm run start-server`: Start Express + PeerJS server on port 3001.
- `npm run dev`: Run both frontend and backend concurrently.
- `npm run build`: Build production frontend assets.
- `npm test`: Run tests in watch mode.
- `npm run deploy`: Build and start server for production.

## How File Sharing Works (High Level)

1. A peer requests file metadata from connected peers.
2. Peers respond with available file lists.
3. A selected file is requested from a specific peer.
4. File data is transferred over peer connections and downloaded locally.

## Notes

- Designed primarily for same-network / LAN usage
- Ensure ports `3000` and `3001` are available
- Allow local Node.js/browser traffic if firewall blocks peer communication

## Future Improvements

- Add authentication and peer identity verification.
- Add transfer progress UI for large files.
- Add resumable chunked transfer retries.
- Add end-to-end encryption for all payloads by default.
