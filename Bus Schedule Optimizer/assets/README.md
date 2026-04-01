# Assets Directory

This directory contains icons and fonts for the Transportation Optimization System.

## Icons

Due to the text-based nature of this scaffold, icon files are not included. 
To add icons:

1. Add PNG or SVG files for:
   - `bus_icon.png` - For transportation theme
   - `route_icon.png` - For route visualization
   - `pin_icon.png` - For location markers
   - `exit_icon.png` - For exit button
   - `fullscreen_icon.png` - For fullscreen toggle
   - `windowed_icon.png` - For windowed toggle

2. Load icons in your screens using:
   ```python
   import pygame
   icon = pygame.image.load('assets/icons/bus_icon.png')
   ```

## Fonts

The application currently uses system fonts (Segoe UI, Consolas).
To add custom fonts:

1. Add TTF files to `assets/fonts/`
2. Update `gui/styles.py` to load custom fonts:
   ```python
   FONT_CUSTOM = pygame.font.Font('assets/fonts/CustomFont.ttf', 16)
   ```

## Recommended Icon Sources

- [Flaticon](https://www.flaticon.com/) - Free icons with attribution
- [Font Awesome](https://fontawesome.com/) - Icon library
- [Material Icons](https://fonts.google.com/icons) - Google's icon set
- Custom design using tools like Inkscape or Figma

## Size Guidelines

- Button icons: 24×24 or 32×32 pixels
- Header icons: 48×48 or 64×64 pixels
- Background patterns: Tileable textures
