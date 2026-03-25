document.addEventListener("DOMContentLoaded", () => {
  // Add click event listeners to the "Read More" buttons
  const readMoreButtons = document.querySelectorAll(".btn-primary")
  readMoreButtons.forEach((button) => {
    button.addEventListener("click", (event) => {
      event.preventDefault()
      alert("This feature will be implemented in a future module!")
    })
  })

  // Add click event listeners to the Quick Links buttons
  const quickLinkButtons = document.querySelectorAll("#quick-links .btn")
  quickLinkButtons.forEach((button) => {
    button.addEventListener("click", function (event) {
      event.preventDefault()
      alert(`You clicked: ${this.textContent}. This feature will be implemented soon!`)
    })
  })

  // Add click event listeners to the Brain Break buttons
  const brainBreakButtons = document.querySelectorAll("#brain-break .btn")
  brainBreakButtons.forEach((button) => {
    button.addEventListener("click", function (event) {
      event.preventDefault()
      alert(`You're about to play: ${this.textContent.trim()}. This game will be available soon!`)
    })
  })

  // Smooth scrolling for anchor links
  document.querySelectorAll('a[href^="#"]').forEach((anchor) => {
    anchor.addEventListener("click", function (e) {
      e.preventDefault()

      document.querySelector(this.getAttribute("href")).scrollIntoView({
        behavior: "smooth",
      })
    })
  })
})

