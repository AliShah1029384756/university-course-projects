const http = require("http")
const fs = require("fs")
const path = require("path")
const { Chess } = require("chess.js")
const socketIo = require("socket.io")

const PORT = process.env.PORT || 3000

// Create HTTP server
const server = http.createServer((req, res) => {
  // Handle static file requests
  const filePath = path.join(__dirname, "public", req.url === "/" ? "index.html" : req.url)

  // Get file extension
  const extname = path.extname(filePath)
  let contentType = "text/html"

  // Set content type based on file extension
  switch (extname) {
    case ".js":
      contentType = "text/javascript"
      break
    case ".css":
      contentType = "text/css"
      break
    case ".json":
      contentType = "application/json"
      break
    case ".png":
      contentType = "image/png"
      break
    case ".jpg":
      contentType = "image/jpg"
      break
  }

  // Read file
  fs.readFile(filePath, (err, content) => {
    if (err) {
      if (err.code === "ENOENT") {
        // Page not found
        fs.readFile(path.join(__dirname, "public", "404.html"), (err, content) => {
          res.writeHead(404, { "Content-Type": "text/html" })
          res.end(content, "utf8")
        })
      } else {
        // Server error
        res.writeHead(500)
        res.end(`Server Error: ${err.code}`)
      }
    } else {
      // Success
      res.writeHead(200, { "Content-Type": contentType })
      res.end(content, "utf8")
    }
  })
})

// Initialize Socket.io
const io = socketIo(server)

// Game rooms storage
const rooms = {}

// Socket.io connection handling
io.on("connection", (socket) => {
  console.log("User connected")

  // Send list of available rooms
  socket.on("getRooms", () => {
    const roomsList = Object.keys(rooms).map((roomName) => {
      const room = rooms[roomName]
      const playerCount = (room.players.white ? 1 : 0) + (room.players.black ? 1 : 0)
      return {
        name: roomName,
        players: playerCount,
      }
    })
    socket.emit("roomsList", roomsList)
  })

  socket.on("createRoom", ({ roomName, username }) => {
    if (rooms[roomName]) {
      socket.emit("error", "Room already exists")
      return
    }

    rooms[roomName] = {
      chess: new Chess(),
      players: {
        white: null,
        black: null,
      },
      spectators: [],
      timers: {
        white: 600, // 10 minutes in seconds
        black: 600,
      },
      gameStarted: false,
      drawOffers: {},
    }

    joinRoom(socket, roomName, username)
  })

  socket.on("joinRoom", ({ roomName, username }) => {
    if (!rooms[roomName]) {
      socket.emit("error", "Room does not exist")
      return
    }

    joinRoom(socket, roomName, username)
  })

  socket.on("move", (move, roomName) => {
    try {
      const room = rooms[roomName]
      if (!room) return

      const chess = room.chess
      const playerRole = getPlayerRole(room, socket.id)

      if (chess.turn() === "w" && playerRole !== "white") return
      if (chess.turn() === "b" && playerRole !== "black") return

      const result = chess.move(move)

      if (result) {
        io.to(roomName).emit("move", move)
        io.to(roomName).emit("boardState", chess.fen())

        // Start game timer if this is the first move
        if (!room.gameStarted && chess.history().length === 1) {
          room.gameStarted = true
          io.to(roomName).emit("gameStart")
        }

        // Check for game over conditions
        if (chess.game_over()) {
          let reason = ""
          let winner = null

          if (chess.in_checkmate()) {
            reason = "checkmate"
            winner = chess.turn() === "w" ? "Black" : "White"
          } else if (chess.in_draw()) {
            reason = "draw"
          } else if (chess.in_stalemate()) {
            reason = "stalemate"
          } else if (chess.in_threefold_repetition()) {
            reason = "threefold repetition"
          } else if (chess.insufficient_material()) {
            reason = "insufficient material"
          }

          io.to(roomName).emit("gameOver", { reason, winner })
        }
      } else {
        socket.emit("invalidMove", move)
      }
    } catch (err) {
      console.log("Invalid Move", err)
    }
  })

  socket.on("resign", ({ roomName }) => {
    const room = rooms[roomName]
    if (!room) return

    const playerRole = getPlayerRole(room, socket.id)
    if (!playerRole || (playerRole !== "white" && playerRole !== "black")) return

    const winner = playerRole === "white" ? "Black" : "White"
    io.to(roomName).emit("gameOver", { reason: "resignation", winner })
  })

  socket.on("offerDraw", ({ roomName }) => {
    const room = rooms[roomName]
    if (!room) return

    const playerRole = getPlayerRole(room, socket.id)
    if (!playerRole || (playerRole !== "white" && playerRole !== "black")) return

    const opponent = playerRole === "white" ? room.players.black : room.players.white
    if (!opponent) return

    room.drawOffers[playerRole] = true

    // If both players have offered a draw, end the game
    if (room.drawOffers.white && room.drawOffers.black) {
      io.to(roomName).emit("gameOver", { reason: "draw agreement" })
      return
    }

    // Otherwise, notify the opponent
    io.to(opponent.id).emit("drawOffered")
  })

  socket.on("acceptDraw", ({ roomName }) => {
    const room = rooms[roomName]
    if (!room) return

    io.to(roomName).emit("drawResponse", true)
    io.to(roomName).emit("gameOver", { reason: "draw agreement" })
  })

  socket.on("declineDraw", ({ roomName }) => {
    const room = rooms[roomName]
    if (!room) return

    const playerRole = getPlayerRole(room, socket.id)
    if (playerRole === "white") {
      room.drawOffers.black = false
    } else if (playerRole === "black") {
      room.drawOffers.white = false
    }

    io.to(roomName).emit("drawResponse", false)
  })

  socket.on("timeOut", ({ color, roomName }) => {
    const room = rooms[roomName]
    if (!room) return

    const winner = color === "w" ? "Black" : "White"
    io.to(roomName).emit("gameOver", { reason: "timeout", winner })
  })

  socket.on("spectatorResponse", (play, roomName) => {
    try {
      if (!rooms[roomName]) return

      if (play) {
        const room = rooms[roomName]
        if (!room.players.white) {
          room.players.white = {
            id: socket.id,
            username: socket.username,
          }
          socket.emit("PlayerRole", "w")
        } else if (!room.players.black) {
          room.players.black = {
            id: socket.id,
            username: socket.username,
          }
          socket.emit("PlayerRole", "b")
        }
        room.spectators = room.spectators.filter((spectator) => spectator.id !== socket.id)
        io.to(roomName).emit("updateUsers", getUsersInRoom(roomName))
      }
    } catch (err) {
      console.log("Spectator Can't Move", err)
    }
  })

  socket.on("disconnect", () => {
    console.log("User disconnected")
    for (const roomName in rooms) {
      const room = rooms[roomName]
      const playerRole = getPlayerRole(room, socket.id)
      if (playerRole) {
        if (playerRole === "white") {
          room.players.white = null
          if (room.players.black) {
            io.to(roomName).emit("opponentLeft", "white")
          }
        } else if (playerRole === "black") {
          room.players.black = null
          if (room.players.white) {
            io.to(roomName).emit("opponentLeft", "black")
          }
        }
      } else {
        room.spectators = room.spectators.filter((spectator) => spectator.id !== socket.id)
      }

      if (!room.players.white && !room.players.black && room.spectators.length === 0) {
        delete rooms[roomName]
      } else {
        io.to(roomName).emit("updateUsers", getUsersInRoom(roomName))
      }
    }
  })

  socket.on("sendMessage", ({ message, username, roomName }) => {
    if (!rooms[roomName] || !username) return
    const room = rooms[roomName]
    const role = getPlayerRole(room, socket.id) || "Spectator"
    io.to(roomName).emit("chatMessage", { message, username, role })
  })
})

// Helper functions
const joinRoom = (socket, roomName, username) => {
  socket.join(roomName)
  socket.username = username
  const room = rooms[roomName]
  let role = null

  if (!room.players.white) {
    room.players.white = { id: socket.id, username }
    role = "w"
  } else if (!room.players.black) {
    room.players.black = { id: socket.id, username }
    role = "b"

    // Notify white player that opponent has joined
    if (room.players.white) {
      io.to(room.players.white.id).emit("opponentJoined")
    }
  } else {
    room.spectators.push({ id: socket.id, username })
    role = "spectator"
  }

  socket.emit("roomJoined")
  socket.emit("PlayerRole", role)
  socket.emit("boardState", room.chess.fen())
  socket.emit("updateTimers", { white: room.timers.white, black: room.timers.black })
  io.to(roomName).emit("updateUsers", getUsersInRoom(roomName))
  io.to(roomName).emit(
    "userCount",
    Object.keys(room.players).filter((k) => room.players[k]).length + room.spectators.length,
  )

  // Start game if both players are present
  if (room.players.white && room.players.black && !room.gameStarted && room.chess.history().length === 0) {
    room.gameStarted = true
    io.to(roomName).emit("gameStart")
  }
}

const getPlayerRole = (room, socketId) => {
  if (room.players.white && room.players.white.id === socketId) return "white"
  if (room.players.black && room.players.black.id === socketId) return "black"
  return null
}

const getUsersInRoom = (roomName) => {
  const room = rooms[roomName]
  if (!room) return []

  const users = []
  if (room.players.white) users.push({ username: room.players.white.username, role: "White" })
  if (room.players.black) users.push({ username: room.players.black.username, role: "Black" })
  room.spectators.forEach((spectator) => users.push({ username: spectator.username, role: "Spectator" }))

  return users
}

// Start server
server.listen(PORT, () => {
  console.log(`Server running on port ${PORT}`)
})
