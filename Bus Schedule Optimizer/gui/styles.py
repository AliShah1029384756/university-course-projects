"""
GUI Styling Constants and Theme Configuration

This module defines all visual styling for the Transportation Optimization System,
including colors, fonts, padding, and dimension constants.
"""

import pygame

# Initialize Pygame font system
pygame.font.init()

# ============================================================================
# COLOR PALETTE - Modern, transportation-themed
# ============================================================================

# Base colors
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
LIGHT_GRAY = (240, 240, 240)
MEDIUM_GRAY = (200, 200, 200)
DARK_GRAY = (100, 100, 100)

# Accent colors
PRIMARY_BLUE = (41, 128, 185)      # Professional blue
PRIMARY_BLUE_HOVER = (52, 152, 219)  # Lighter blue for hover
SECONDARY_YELLOW = (241, 196, 15)   # Transportation yellow
SECONDARY_YELLOW_HOVER = (243, 156, 18)

# Status colors
SUCCESS_GREEN = (46, 204, 113)
WARNING_ORANGE = (230, 126, 34)
ERROR_RED = (231, 76, 60)
INFO_BLUE = (52, 152, 219)

# UI Element colors
PANEL_BG = (250, 250, 250)
PANEL_BORDER = (200, 200, 200)
INPUT_BG = WHITE
INPUT_BORDER = (180, 180, 180)
INPUT_FOCUS = PRIMARY_BLUE
BUTTON_TEXT = WHITE
TEXT_PRIMARY = (33, 33, 33)
TEXT_SECONDARY = (100, 100, 100)
SHADOW = (0, 0, 0, 30)  # Semi-transparent shadow

# ============================================================================
# FONTS
# ============================================================================

# Font sizes
FONT_SIZE_HUGE = 48
FONT_SIZE_LARGE = 32
FONT_SIZE_TITLE = 28
FONT_SIZE_SUBTITLE = 20
FONT_SIZE_NORMAL = 16
FONT_SIZE_SMALL = 14
FONT_SIZE_TINY = 12

# Font objects (using default system fonts)
try:
    FONT_HUGE = pygame.font.SysFont('segoeui', FONT_SIZE_HUGE)
    FONT_LARGE = pygame.font.SysFont('segoeui', FONT_SIZE_LARGE)
    FONT_TITLE = pygame.font.SysFont('segoeui', FONT_SIZE_TITLE, bold=True)
    FONT_SUBTITLE = pygame.font.SysFont('segoeui', FONT_SIZE_SUBTITLE, bold=True)
    FONT_NORMAL = pygame.font.SysFont('segoeui', FONT_SIZE_NORMAL)
    FONT_SMALL = pygame.font.SysFont('segoeui', FONT_SIZE_SMALL)
    FONT_TINY = pygame.font.SysFont('segoeui', FONT_SIZE_TINY)
    FONT_MONO = pygame.font.SysFont('consolas', FONT_SIZE_NORMAL)  # For tableaus
    FONT_MONO_SMALL = pygame.font.SysFont('consolas', FONT_SIZE_SMALL)
except:
    # Fallback to default font if Segoe UI not available
    FONT_HUGE = pygame.font.Font(None, FONT_SIZE_HUGE)
    FONT_LARGE = pygame.font.Font(None, FONT_SIZE_LARGE)
    FONT_TITLE = pygame.font.Font(None, FONT_SIZE_TITLE)
    FONT_SUBTITLE = pygame.font.Font(None, FONT_SIZE_SUBTITLE)
    FONT_NORMAL = pygame.font.Font(None, FONT_SIZE_NORMAL)
    FONT_SMALL = pygame.font.Font(None, FONT_SIZE_SMALL)
    FONT_TINY = pygame.font.Font(None, FONT_SIZE_TINY)
    FONT_MONO = pygame.font.Font(None, FONT_SIZE_NORMAL)
    FONT_MONO_SMALL = pygame.font.Font(None, FONT_SIZE_SMALL)

# ============================================================================
# LAYOUT & SPACING
# ============================================================================

# Padding and margins
PADDING_TINY = 4
PADDING_SMALL = 8
PADDING_MEDIUM = 12
PADDING_LARGE = 16
PADDING_XLARGE = 24
PADDING_HUGE = 32

# Border radius for rounded rectangles
BORDER_RADIUS_SMALL = 4
BORDER_RADIUS_MEDIUM = 8
BORDER_RADIUS_LARGE = 12

# Shadows
SHADOW_OFFSET = 2
SHADOW_BLUR = 4

# ============================================================================
# COMPONENT DIMENSIONS
# ============================================================================

# Buttons
BUTTON_HEIGHT = 45
BUTTON_WIDTH_SMALL = 100
BUTTON_WIDTH_MEDIUM = 150
BUTTON_WIDTH_LARGE = 200
BUTTON_PADDING = PADDING_MEDIUM

# Input boxes
INPUT_HEIGHT = 35
INPUT_WIDTH_SMALL = 100
INPUT_WIDTH_MEDIUM = 200
INPUT_WIDTH_LARGE = 300
INPUT_PADDING = PADDING_SMALL

# Panels
PANEL_MIN_WIDTH = 300
PANEL_MIN_HEIGHT = 200

# Header
HEADER_HEIGHT = 70
STATUS_BAR_HEIGHT = 30

# ============================================================================
# SCREEN DIMENSIONS
# ============================================================================

SCREEN_WIDTH = 1280
SCREEN_HEIGHT = 720

# ============================================================================
# ANIMATION & INTERACTION
# ============================================================================

HOVER_SCALE = 1.05  # Scale factor for button hover effect
TRANSITION_SPEED = 0.15  # Speed for smooth transitions
CURSOR_BLINK_RATE = 500  # Milliseconds for input cursor blink

# ============================================================================
# HELPER FUNCTIONS
# ============================================================================

def get_shadow_surface(width, height, offset=SHADOW_OFFSET):
    """
    Create a semi-transparent shadow surface.
    
    Args:
        width: Shadow width
        height: Shadow height
        offset: Shadow offset from main element
        
    Returns:
        pygame.Surface with alpha transparency
    """
    shadow = pygame.Surface((width, height), pygame.SRCALPHA)
    shadow.fill(SHADOW)
    return shadow


def draw_rounded_rect(surface, rect, color, radius=BORDER_RADIUS_MEDIUM, border_color=None, border_width=0):
    """
    Draw a rounded rectangle on the given surface.
    
    Args:
        surface: pygame.Surface to draw on
        rect: pygame.Rect defining position and size
        color: Fill color (R, G, B) or (R, G, B, A)
        radius: Corner radius
        border_color: Optional border color
        border_width: Border width if border_color is specified
    """
    pygame.draw.rect(surface, color, rect, border_radius=radius)
    if border_color and border_width > 0:
        pygame.draw.rect(surface, border_color, rect, width=border_width, border_radius=radius)


def interpolate_color(color1, color2, factor):
    """
    Interpolate between two colors.
    
    Args:
        color1: Starting color (R, G, B)
        color2: Ending color (R, G, B)
        factor: Interpolation factor (0.0 to 1.0)
        
    Returns:
        Interpolated color tuple
    """
    r = int(color1[0] + (color2[0] - color1[0]) * factor)
    g = int(color1[1] + (color2[1] - color1[1]) * factor)
    b = int(color1[2] + (color2[2] - color1[2]) * factor)
    return (r, g, b)


def get_text_surface(text, font=FONT_NORMAL, color=TEXT_PRIMARY, antialias=True):
    """
    Render text to a surface.
    
    Args:
        text: Text string to render
        font: pygame.Font object
        color: Text color
        antialias: Use antialiasing
        
    Returns:
        pygame.Surface with rendered text
    """
    return font.render(str(text), antialias, color)


def clamp(value, min_value, max_value):
    """Clamp a value between min and max."""
    return max(min_value, min(value, max_value))
