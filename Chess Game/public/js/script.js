// Initialize socket connection
const socket = io()
const chess = new Chess()

// DOM elements
const lobbyContainer = document.getElementById("lobbyContainer")
const gameContainer = document.getElementById("gameContainer")
const boardElement = document.querySelector(".chessboard")
const statusElement = document.querySelector(".status")
const userCountElement = document.querySelector(".user-count")
const turnPopupElement = document.querySelector(".turn-popup")
const messageElement = document.querySelector(".message")
const usernameInput = document.getElementById("username")
const roomNameInput = document.getElementById("roomName")
const createRoomButton = document.getElementById("createRoomButton")
const joinRoomButton = document.getElementById("joinRoomButton")
const roomsList = document.getElementById("roomsList")
const usersList = document.getElementById("usersList")
const chatForm = document.getElementById("chatForm")
const chatInput = document.getElementById("chatInput")
const chatMessages = document.querySelector(".chat-messages")
const whiteTimer = document.getElementById("whiteTimer")
const blackTimer = document.getElementById("blackTimer")
const whitePlayerName = document.getElementById("whitePlayerName")
const blackPlayerName = document.getElementById("blackPlayerName")
const gameStatus = document.getElementById("gameStatus")
const resignButton = document.getElementById("resignButton")
const offerDrawButton = document.getElementById("offerDrawButton")

// Game state variables
let draggedPiece = null
let sourceSquare = null
let playerRole = null
let currentRoom = null
let username = null
let whiteTimeRemaining = 600 // 10 minutes in seconds
let blackTimeRemaining = 600
let timerInterval = null
let isTimerRunning = false

// Event listeners
createRoomButton.addEventListener("click", () => {
  const roomName = roomNameInput.value.trim()
  username = usernameInput.value.trim()
  if (roomName && username) {
    socket.emit("createRoom", { roomName, username })
    currentRoom = roomName
  } else {
    alert("Please enter both username and room name")
  }
})

joinRoomButton.addEventListener("click", () => {
  const roomName = roomNameInput.value.trim()
  username = usernameInput.value.trim()
  if (roomName && username) {
    socket.emit("joinRoom", { roomName, username })
    currentRoom = roomName
  } else {
    alert("Please enter both username and room name")
  }
})

chatForm.addEventListener("submit", (e) => {
  e.preventDefault()
  const message = chatInput.value.trim()
  if (message) {
    socket.emit("sendMessage", { message, username, roomName: currentRoom })
    chatInput.value = ""
  }
})

resignButton.addEventListener("click", () => {
  if (confirm("Are you sure you want to resign?")) {
    socket.emit("resign", { roomName: currentRoom })
  }
})

offerDrawButton.addEventListener("click", () => {
  socket.emit("offerDraw", { roomName: currentRoom })
})

// Helper function to show game container and hide lobby
function showGameContainer() {
  lobbyContainer.style.display = "none"
  gameContainer.style.display = "grid"
}

// Helper function to format time
function formatTime(seconds) {
  const minutes = Math.floor(seconds / 60)
  const remainingSeconds = seconds % 60
  return `${minutes}:${remainingSeconds.toString().padStart(2, "0")}`
}

// Start the game timer
function startTimer() {
  if (timerInterval) {
    clearInterval(timerInterval)
  }

  isTimerRunning = true
  timerInterval = setInterval(() => {
    if (chess.turn() === "w") {
      whiteTimeRemaining--
      whiteTimer.textContent = formatTime(whiteTimeRemaining)

      if (whiteTimeRemaining <= 0) {
        clearInterval(timerInterval)
        socket.emit("timeOut", { color: "w", roomName: currentRoom })
      }
    } else {
      blackTimeRemaining--
      blackTimer.textContent = formatTime(blackTimeRemaining)

      if (blackTimeRemaining <= 0) {
        clearInterval(timerInterval)
        socket.emit("timeOut", { color: "b", roomName: currentRoom })
      }
    }
  }, 1000)
}

// Stop the game timer
function stopTimer() {
  if (timerInterval) {
    clearInterval(timerInterval)
    isTimerRunning = false
  }
}

// Reset the game timer
function resetTimer() {
  whiteTimeRemaining = 600
  blackTimeRemaining = 600
  whiteTimer.textContent = formatTime(whiteTimeRemaining)
  blackTimer.textContent = formatTime(blackTimeRemaining)
  stopTimer()
}

// Chess piece unicode mapping
const getPieceUnicode = (piece) => {
  const unicodePieces = {
    p: "\u2659",
    r: "\u2656",
    n: "\u2658",
    b: "\u2657",
    q: "\u2655",
    k: "\u2654",
  }
  return unicodePieces[piece.type]
}

// Handle chess move
const handleMove = (source, target) => {
  const move = {
    from: `${String.fromCharCode(97 + source.col)}${8 - source.row}`,
    to: `${String.fromCharCode(97 + target.col)}${8 - target.row}`,
    promotion: "q", // Always promote to queen for simplicity
  }
  socket.emit("move", move, currentRoom)
}

// Render chess board
const renderBoard = () => {
  const board = chess.board()
  boardElement.innerHTML = ""

  board.forEach((row, rowIndex) => {
    row.forEach((square, squareIndex) => {
      const squareElement = document.createElement("div")
      squareElement.classList.add("square", (rowIndex + squareIndex) % 2 === 0 ? "light" : "dark")

      squareElement.dataset.row = rowIndex
      squareElement.dataset.col = squareIndex

      if (square) {
        const pieceElement = document.createElement("div")
        pieceElement.classList.add("piece", square.color === "w" ? "white" : "black")
        pieceElement.innerText = getPieceUnicode(square)
        pieceElement.draggable = true

        pieceElement.addEventListener("dragstart", (e) => {
          if (
            (square.color === "w" && playerRole === "w" && chess.turn() === "w") ||
            (square.color === "b" && playerRole === "b" && chess.turn() === "b")
          ) {
            draggedPiece = pieceElement
            sourceSquare = { row: rowIndex, col: squareIndex }
            showPossibleMoves(rowIndex, squareIndex)
          } else {
            e.preventDefault()
          }
        })

        pieceElement.addEventListener("dragend", () => {
          draggedPiece = null
          sourceSquare = null
          const squares = document.querySelectorAll(".square")
          squares.forEach((square) => square.classList.remove("possible-move"))
        })

        squareElement.appendChild(pieceElement)
      }

      squareElement.addEventListener("dragover", (e) => {
        e.preventDefault()
      })

      squareElement.addEventListener("drop", (e) => {
        e.preventDefault()
        if (draggedPiece) {
          const targetSquare = {
            row: Number.parseInt(squareElement.dataset.row),
            col: Number.parseInt(squareElement.dataset.col),
          }
          handleMove(sourceSquare, targetSquare)
        }
      })

      // Add click-based move support for mobile
      squareElement.addEventListener("click", () => {
        if (sourceSquare) {
          // Second click - attempt to move
          const targetSquare = {
            row: Number.parseInt(squareElement.dataset.row),
            col: Number.parseInt(squareElement.dataset.col),
          }
          handleMove(sourceSquare, targetSquare)
          sourceSquare = null
          const squares = document.querySelectorAll(".square")
          squares.forEach((square) => square.classList.remove("possible-move"))
        } else if (
          square &&
          ((square.color === "w" && playerRole === "w" && chess.turn() === "w") ||
            (square.color === "b" && playerRole === "b" && chess.turn() === "b"))
        ) {
          // First click - select piece
          sourceSquare = { row: rowIndex, col: squareIndex }
          showPossibleMoves(rowIndex, squareIndex)
        }
      })

      boardElement.appendChild(squareElement)
    })
  })

  // Update game status
  updateGameStatus()

  // Show turn popup
  turnPopupElement.textContent = `${chess.turn() === "w" ? "White's" : "Black's"} turn`
  turnPopupElement.classList.add("visible")
  setTimeout(() => turnPopupElement.classList.remove("visible"), 2000)

  // Flip board for black player
  if (playerRole === "b") {
    boardElement.classList.add("flipped")
  } else {
    boardElement.classList.remove("flipped")
  }
}

// Update game status
function updateGameStatus() {
  if (chess.in_checkmate()) {
    statusElement.textContent = "Checkmate"
    gameStatus.textContent = `${chess.turn() === "w" ? "Black" : "White"} wins`
    stopTimer()
  } else if (chess.in_check()) {
    statusElement.textContent = "Check"
    gameStatus.textContent = `${chess.turn() === "w" ? "White" : "Black"} in check`
  } else if (chess.in_draw()) {
    statusElement.textContent = "Draw"
    gameStatus.textContent = "Game ended in draw"
    stopTimer()
  } else if (chess.in_stalemate()) {
    statusElement.textContent = "Stalemate"
    gameStatus.textContent = "Game ended in stalemate"
    stopTimer()
  } else if (chess.in_threefold_repetition()) {
    statusElement.textContent = "Threefold Repetition"
    gameStatus.textContent = "Game ended in draw (repetition)"
    stopTimer()
  } else if (chess.insufficient_material()) {
    statusElement.textContent = "Insufficient Material"
    gameStatus.textContent = "Game ended in draw (insufficient material)"
    stopTimer()
  } else {
    statusElement.textContent = ""
    gameStatus.textContent = `${chess.turn() === "w" ? "White" : "Black"} to move`
  }
}

// Show possible moves for a piece
const showPossibleMoves = (row, col) => {
  const moves = chess.moves({
    square: `${String.fromCharCode(97 + col)}${8 - row}`,
    verbose: true,
  })

  const squares = document.querySelectorAll(".square")
  squares.forEach((square) => {
    square.classList.remove("possible-move")
  })

  moves.forEach((move) => {
    const targetSquare = document.querySelector(
      `.square[data-row="${8 - move.to[1]}"][data-col="${move.to.charCodeAt(0) - 97}"]`,
    )
    if (targetSquare) {
      targetSquare.classList.add("possible-move")
    }
  })
}

// Socket event handlers
socket.on("roomsList", (rooms) => {
  if (rooms.length === 0) {
    roomsList.innerHTML = "<li class='room-item'>No rooms available</li>"
    return
  }

  roomsList.innerHTML = ""
  rooms.forEach((room) => {
    const li = document.createElement("li")
    li.classList.add("room-item")
    li.innerHTML = `
      ${room.name} (${room.players}/2)
      ${room.players < 2 ? `<button class="room-join-btn" data-room="${room.name}">Join</button>` : ""}
    `
    roomsList.appendChild(li)
  })

  // Add event listeners to join buttons
  document.querySelectorAll(".room-join-btn").forEach((btn) => {
    btn.addEventListener("click", () => {
      const roomName = btn.getAttribute("data-room")
      username = usernameInput.value.trim()
      if (username) {
        socket.emit("joinRoom", { roomName, username })
        currentRoom = roomName
      } else {
        alert("Please enter a username")
      }
    })
  })
})

socket.on("roomJoined", () => {
  showGameContainer()
})

socket.on("PlayerRole", (role) => {
  playerRole = role
  renderBoard()

  if (role === "w" || role === "b") {
    if (!isTimerRunning && chess.history().length > 0) {
      startTimer()
    }
  }
})

socket.on("gameStart", () => {
  resetTimer()
  startTimer()
})

socket.on("spectator", () => {
  playerRole = null
  renderBoard()
})

socket.on("boardState", (fen) => {
  chess.load(fen)
  renderBoard()
})

socket.on("move", (move) => {
  chess.move(move)
  renderBoard()
})

socket.on("userCount", (count) => {
  userCountElement.textContent = `Online users: ${count}`
})

socket.on("updateUsers", (users) => {
  usersList.innerHTML = ""
  users.forEach((user) => {
    const userElement = document.createElement("li")
    userElement.textContent = `${user.username} (${user.role})`
    usersList.appendChild(userElement)

    // Update player names in the board header
    if (user.role === "White") {
      whitePlayerName.textContent = user.username
    } else if (user.role === "Black") {
      blackPlayerName.textContent = user.username
    }
  })
})

socket.on("updateTimers", ({ white, black }) => {
  whiteTimeRemaining = white
  blackTimeRemaining = black
  whiteTimer.textContent = formatTime(whiteTimeRemaining)
  blackTimer.textContent = formatTime(blackTimeRemaining)
})

socket.on("error", (message) => {
  messageElement.textContent = message
  setTimeout(() => (messageElement.textContent = ""), 3000)
})

socket.on("opponentJoined", () => {
  messageElement.textContent = "Opponent joined the room"
  setTimeout(() => (messageElement.textContent = ""), 3000)
})

socket.on("opponentLeft", (color) => {
  messageElement.textContent = `${color === "white" ? "White" : "Black"} player left the game`
  setTimeout(() => (messageElement.textContent = ""), 3000)
  stopTimer()
})

socket.on("gameOver", ({ reason, winner }) => {
  stopTimer()
  if (winner) {
    gameStatus.textContent = `Game over: ${winner} wins by ${reason}`
  } else {
    gameStatus.textContent = `Game over: ${reason}`
  }
  messageElement.textContent = gameStatus.textContent
})

socket.on("drawOffered", () => {
  if (confirm("Your opponent has offered a draw. Do you accept?")) {
    socket.emit("acceptDraw", { roomName: currentRoom })
  } else {
    socket.emit("declineDraw", { roomName: currentRoom })
  }
})

socket.on("drawResponse", (accepted) => {
  if (accepted) {
    messageElement.textContent = "Draw offer accepted. Game ended in a draw."
    gameStatus.textContent = "Game ended in draw (agreement)"
    stopTimer()
  } else {
    messageElement.textContent = "Draw offer declined."
    setTimeout(() => (messageElement.textContent = ""), 3000)
  }
})

socket.on("chatMessage", ({ message, username, role }) => {
  const messageElement = document.createElement("div")
  messageElement.classList.add("chat-message")
  messageElement.innerHTML = `<strong>${username} (${role}):</strong> ${message}`
  chatMessages.appendChild(messageElement)
  chatMessages.scrollTop = chatMessages.scrollHeight
})

// Request available rooms on connection
socket.on("connect", () => {
  socket.emit("getRooms")
})

// Periodically request room updates
setInterval(() => {
  if (lobbyContainer.style.display !== "none") {
    socket.emit("getRooms")
  }
}, 5000)

// Initialize empty board
renderBoard()
