"""
GUI Components for Transportation Optimization System

This module provides reusable UI components including buttons, input boxes,
scrollable panels, and table renderers with consistent styling and behavior.
"""

import pygame
from gui import styles
import time


class Button:
    """
    A clickable button with hover effects and customizable styling.
    
    Features:
    - Hover color change and scale effect
    - Click callback support
    - Rounded corners and shadows
    - Icon support (optional)
    """
    
    def __init__(self, x, y, width, height, text, callback=None, 
                 color=None, hover_color=None, text_color=None, font=None):
        """
        Initialize a Button.
        
        Args:
            x, y: Position coordinates
            width, height: Button dimensions
            text: Button label text
            callback: Function to call when clicked
            color: Normal button color (defaults to PRIMARY_BLUE)
            hover_color: Hover button color (defaults to PRIMARY_BLUE_HOVER)
            text_color: Text color (defaults to white)
            font: Font for text (defaults to FONT_NORMAL)
        """
        self.rect = pygame.Rect(x, y, width, height)
        self.original_rect = self.rect.copy()
        self.text = text
        self.callback = callback
        self.color = color or styles.PRIMARY_BLUE
        self.hover_color = hover_color or styles.PRIMARY_BLUE_HOVER
        self.text_color = text_color or styles.BUTTON_TEXT
        self.font = font or styles.FONT_NORMAL
        
        self.is_hovered = False
        self.is_pressed = False
        self.hover_scale = 1.0  # Current scale for smooth transition
        
    def handle_event(self, event):
        """
        Process mouse events for the button.
        
        Args:
            event: pygame.Event to process
            
        Returns:
            True if button was clicked, False otherwise
        """
        if event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
            if self.rect.collidepoint(event.pos):
                self.is_pressed = True
                
        elif event.type == pygame.MOUSEBUTTONUP and event.button == 1:
            if self.is_pressed and self.rect.collidepoint(event.pos):
                self.is_pressed = False
                if self.callback:
                    self.callback()
                return True
            self.is_pressed = False
            
        return False
    
    def update(self, mouse_pos):
        """
        Update button state based on mouse position.
        
        Args:
            mouse_pos: Current mouse position tuple (x, y)
        """
        was_hovered = self.is_hovered
        self.is_hovered = self.rect.collidepoint(mouse_pos)
        
        # Smooth hover scale transition
        target_scale = styles.HOVER_SCALE if self.is_hovered else 1.0
        self.hover_scale += (target_scale - self.hover_scale) * styles.TRANSITION_SPEED
        
        # Update rect size for hover effect
        if abs(self.hover_scale - 1.0) > 0.01:
            center = self.original_rect.center
            new_width = int(self.original_rect.width * self.hover_scale)
            new_height = int(self.original_rect.height * self.hover_scale)
            self.rect = pygame.Rect(0, 0, new_width, new_height)
            self.rect.center = center
    
    def draw(self, surface):
        """
        Draw the button on the given surface.
        
        Args:
            surface: pygame.Surface to draw on
        """
        # Determine current color
        current_color = self.hover_color if self.is_hovered else self.color
        
        # Draw shadow
        shadow_rect = self.rect.copy()
        shadow_rect.x += styles.SHADOW_OFFSET
        shadow_rect.y += styles.SHADOW_OFFSET
        styles.draw_rounded_rect(surface, shadow_rect, (0, 0, 0, 40), styles.BORDER_RADIUS_MEDIUM)
        
        # Draw button background
        styles.draw_rounded_rect(surface, self.rect, current_color, styles.BORDER_RADIUS_MEDIUM)
        
        # Draw text
        text_surface = self.font.render(self.text, True, self.text_color)
        text_rect = text_surface.get_rect(center=self.rect.center)
        surface.blit(text_surface, text_rect)


class InputBox:
    """
    A text input field with focus states and validation.
    
    Features:
    - Single-line and multi-line support
    - Focus highlighting
    - Cursor blinking
    - Text selection and clipboard support
    """
    
    def __init__(self, x, y, width, height, label="", default_text="", 
                 multiline=False, numeric_only=False, font=None):
        """
        Initialize an InputBox.
        
        Args:
            x, y: Position coordinates
            width, height: Input box dimensions
            label: Optional label text above input
            default_text: Initial text value
            multiline: Allow multiple lines of text
            numeric_only: Restrict input to numbers only
            font: Font for input text
        """
        self.rect = pygame.Rect(x, y, width, height)
        self.label = label
        self.text = default_text
        self.font = font or styles.FONT_NORMAL
        self.multiline = multiline
        self.numeric_only = numeric_only
        
        self.is_focused = False
        self.cursor_visible = True
        self.cursor_timer = 0
        self.cursor_pos = len(default_text)
        
        self.border_color = styles.INPUT_BORDER
        self.error_message = ""
        
    def handle_event(self, event):
        """
        Process keyboard and mouse events.
        
        Args:
            event: pygame.Event to process
        """
        if event.type == pygame.MOUSEBUTTONDOWN:
            # Check if click is inside input box
            self.is_focused = self.rect.collidepoint(event.pos)
            
        elif event.type == pygame.KEYDOWN and self.is_focused:
            if event.key == pygame.K_RETURN:
                if self.multiline:
                    self.text = self.text[:self.cursor_pos] + '\n' + self.text[self.cursor_pos:]
                    self.cursor_pos += 1
                else:
                    self.is_focused = False
                    
            elif event.key == pygame.K_BACKSPACE:
                if self.cursor_pos > 0:
                    self.text = self.text[:self.cursor_pos-1] + self.text[self.cursor_pos:]
                    self.cursor_pos -= 1
                    
            elif event.key == pygame.K_DELETE:
                if self.cursor_pos < len(self.text):
                    self.text = self.text[:self.cursor_pos] + self.text[self.cursor_pos+1:]
                    
            elif event.key == pygame.K_LEFT:
                self.cursor_pos = max(0, self.cursor_pos - 1)
                
            elif event.key == pygame.K_RIGHT:
                self.cursor_pos = min(len(self.text), self.cursor_pos + 1)
                
            elif event.key == pygame.K_HOME:
                self.cursor_pos = 0
                
            elif event.key == pygame.K_END:
                self.cursor_pos = len(self.text)
                
            elif event.unicode:
                # Filter input based on numeric_only flag
                if self.numeric_only and not (event.unicode.isdigit() or event.unicode in '.,- \n'):
                    return
                    
                self.text = self.text[:self.cursor_pos] + event.unicode + self.text[self.cursor_pos:]
                self.cursor_pos += 1
    
    def update(self):
        """Update cursor blink animation."""
        self.cursor_timer += 1
        if self.cursor_timer >= styles.CURSOR_BLINK_RATE / 16:  # Assuming ~60 FPS
            self.cursor_visible = not self.cursor_visible
            self.cursor_timer = 0
            
        # Update border color based on focus
        if self.is_focused:
            self.border_color = styles.INPUT_FOCUS
        elif self.error_message:
            self.border_color = styles.ERROR_RED
        else:
            self.border_color = styles.INPUT_BORDER
    
    def draw(self, surface):
        """
        Draw the input box on the given surface.
        
        Args:
            surface: pygame.Surface to draw on
        """
        # Draw label if present
        if self.label:
            label_surface = styles.FONT_SMALL.render(self.label, True, styles.TEXT_SECONDARY)
            surface.blit(label_surface, (self.rect.x, self.rect.y - 20))
        
        # Draw input box background
        styles.draw_rounded_rect(surface, self.rect, styles.INPUT_BG, 
                               styles.BORDER_RADIUS_SMALL, 
                               self.border_color, 2)
        
        # Draw text
        text_surface = self.font.render(self.text, True, styles.TEXT_PRIMARY)
        text_rect = text_surface.get_rect(midleft=(self.rect.x + styles.INPUT_PADDING, self.rect.centery))
        
        # Clip text to input box
        clip_rect = self.rect.inflate(-styles.INPUT_PADDING * 2, 0)
        surface.set_clip(clip_rect)
        surface.blit(text_surface, text_rect)
        surface.set_clip(None)
        
        # Draw cursor if focused
        if self.is_focused and self.cursor_visible:
            cursor_x = text_rect.x + self.font.size(self.text[:self.cursor_pos])[0]
            cursor_y = text_rect.y
            cursor_height = text_rect.height
            pygame.draw.line(surface, styles.TEXT_PRIMARY, 
                           (cursor_x, cursor_y), 
                           (cursor_x, cursor_y + cursor_height), 2)
        
        # Draw error message if present
        if self.error_message:
            error_surface = styles.FONT_TINY.render(self.error_message, True, styles.ERROR_RED)
            surface.blit(error_surface, (self.rect.x, self.rect.bottom + 4))
    
    def get_value(self):
        """Get the current text value."""
        return self.text
    
    def set_value(self, text):
        """Set the text value."""
        self.text = str(text)
        self.cursor_pos = len(self.text)
    
    def validate(self):
        """
        Validate input value.
        
        Returns:
            True if valid, False otherwise
        """
        if not self.text.strip():
            self.error_message = "Field cannot be empty"
            return False
            
        if self.numeric_only:
            try:
                float(self.text.replace(',', ''))
                self.error_message = ""
                return True
            except ValueError:
                self.error_message = "Must be a valid number"
                return False
        
        self.error_message = ""
        return True


class ScrollPanel:
    """
    A scrollable panel for displaying large amounts of content.
    
    Features:
    - Vertical scrolling with mouse wheel
    - Scrollbar display
    - Content clipping
    """
    
    def __init__(self, x, y, width, height, bg_color=None):
        """
        Initialize a ScrollPanel.
        
        Args:
            x, y: Position coordinates
            width, height: Panel dimensions
            bg_color: Background color (defaults to PANEL_BG)
        """
        self.rect = pygame.Rect(x, y, width, height)
        self.bg_color = bg_color or styles.PANEL_BG
        
        self.content_surface = pygame.Surface((width - 20, height), pygame.SRCALPHA)
        self.content_height = 0
        self.scroll_offset = 0
        self.max_scroll = 0
        
        self.scrollbar_rect = pygame.Rect(x + width - 15, y, 15, height)
        self.scrollbar_handle = pygame.Rect(x + width - 15, y, 15, 50)
        self.is_scrolling = False
        
    def handle_event(self, event):
        """
        Process scroll events.
        
        Args:
            event: pygame.Event to process
        """
        if event.type == pygame.MOUSEWHEEL:
            if self.rect.collidepoint(pygame.mouse.get_pos()):
                self.scroll_offset -= event.y * 20
                self.scroll_offset = styles.clamp(self.scroll_offset, 0, self.max_scroll)
                
        elif event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
            if self.scrollbar_handle.collidepoint(event.pos):
                self.is_scrolling = True
                
        elif event.type == pygame.MOUSEBUTTONUP and event.button == 1:
            self.is_scrolling = False
            
        elif event.type == pygame.MOUSEMOTION and self.is_scrolling:
            # Update scroll position based on mouse drag
            rel_y = event.pos[1] - self.scrollbar_rect.y
            scroll_ratio = rel_y / self.scrollbar_rect.height
            self.scroll_offset = int(scroll_ratio * self.max_scroll)
            self.scroll_offset = styles.clamp(self.scroll_offset, 0, self.max_scroll)
    
    def set_content_lines(self, lines, font=None):
        """
        Set content from a list of text lines.
        
        Args:
            lines: List of text strings
            font: Font to render text with (defaults to FONT_MONO)
        """
        font = font or styles.FONT_MONO_SMALL
        self.content_height = 0
        line_height = font.get_height() + 4
        
        # Calculate total content height
        total_height = len(lines) * line_height + styles.PADDING_MEDIUM
        
        # Create content surface
        self.content_surface = pygame.Surface((self.rect.width - 20, total_height), pygame.SRCALPHA)
        self.content_surface.fill(styles.WHITE)
        
        # Render each line
        y_offset = styles.PADDING_SMALL
        for line in lines:
            # Check if line is a separator (all equal signs or dashes)
            if line.strip() and all(c in '=-' for c in line.strip()):
                # Draw a horizontal line instead of rendering equal signs
                line_y = y_offset + line_height // 2
                pygame.draw.line(self.content_surface, styles.TEXT_SECONDARY,
                               (styles.PADDING_SMALL, line_y),
                               (self.rect.width - 40, line_y), 2)
            else:
                text_surface = font.render(line, True, styles.TEXT_PRIMARY)
                self.content_surface.blit(text_surface, (styles.PADDING_SMALL, y_offset))
            y_offset += line_height
        
        self.content_height = total_height
        self.max_scroll = max(0, self.content_height - self.rect.height)
        
        # Update scrollbar handle size and position
        if self.max_scroll > 0:
            handle_height = max(30, int((self.rect.height / self.content_height) * self.scrollbar_rect.height))
            self.scrollbar_handle.height = handle_height
        else:
            self.scrollbar_handle.height = self.scrollbar_rect.height
    
    def draw(self, surface):
        """
        Draw the scroll panel on the given surface.
        
        Args:
            surface: pygame.Surface to draw on
        """
        # Draw panel background
        styles.draw_rounded_rect(surface, self.rect, self.bg_color, 
                               styles.BORDER_RADIUS_MEDIUM, 
                               styles.PANEL_BORDER, 1)
        
        # Draw content with clipping
        surface.set_clip(self.rect)
        surface.blit(self.content_surface, 
                    (self.rect.x + styles.PADDING_SMALL, self.rect.y),
                    (0, self.scroll_offset, self.rect.width - 20, self.rect.height))
        surface.set_clip(None)
        
        # Draw scrollbar if needed
        if self.max_scroll > 0:
            # Scrollbar background
            pygame.draw.rect(surface, styles.MEDIUM_GRAY, self.scrollbar_rect, border_radius=4)
            
            # Scrollbar handle position
            handle_y = self.scrollbar_rect.y + int((self.scroll_offset / self.max_scroll) * 
                                                   (self.scrollbar_rect.height - self.scrollbar_handle.height))
            self.scrollbar_handle.y = handle_y
            
            # Scrollbar handle
            pygame.draw.rect(surface, styles.DARK_GRAY, self.scrollbar_handle, border_radius=4)


class TableRenderer:
    """
    Renders tabular data in a clean, aligned format.
    
    Features:
    - Automatic column width calculation
    - Header row styling
    - Cell padding and borders
    """
    
    def __init__(self, x, y, max_width, font=None):
        """
        Initialize a TableRenderer.
        
        Args:
            x, y: Position coordinates
            max_width: Maximum table width
            font: Font for table text
        """
        self.x = x
        self.y = y
        self.max_width = max_width
        self.font = font or styles.FONT_MONO_SMALL
        
    def draw(self, surface, data, headers=None):
        """
        Draw a table on the given surface.
        
        Args:
            surface: pygame.Surface to draw on
            data: 2D list of cell values
            headers: Optional list of header labels
            
        Returns:
            Height of the rendered table
        """
        if not data:
            return 0
        
        # Calculate column widths
        num_cols = len(data[0]) if data else 0
        col_widths = [0] * num_cols
        
        # Check headers
        if headers:
            for i, header in enumerate(headers):
                col_widths[i] = self.font.size(str(header))[0]
        
        # Check data rows
        for row in data:
            for i, cell in enumerate(row):
                cell_width = self.font.size(str(cell))[0]
                col_widths[i] = max(col_widths[i], cell_width)
        
        # Add padding to column widths
        col_widths = [w + styles.PADDING_MEDIUM for w in col_widths]
        
        cell_height = self.font.get_height() + styles.PADDING_SMALL
        current_y = self.y
        
        # Draw headers if present
        if headers:
            current_x = self.x
            for i, header in enumerate(headers):
                # Header background
                header_rect = pygame.Rect(current_x, current_y, col_widths[i], cell_height)
                pygame.draw.rect(surface, styles.PRIMARY_BLUE, header_rect)
                pygame.draw.rect(surface, styles.PANEL_BORDER, header_rect, 1)
                
                # Header text
                text_surface = self.font.render(str(header), True, styles.WHITE)
                text_rect = text_surface.get_rect(center=header_rect.center)
                surface.blit(text_surface, text_rect)
                
                current_x += col_widths[i]
            
            current_y += cell_height
        
        # Draw data rows
        for row_idx, row in enumerate(data):
            current_x = self.x
            
            # Alternate row colors
            row_color = styles.WHITE if row_idx % 2 == 0 else styles.LIGHT_GRAY
            
            for col_idx, cell in enumerate(row):
                # Cell background
                cell_rect = pygame.Rect(current_x, current_y, col_widths[col_idx], cell_height)
                pygame.draw.rect(surface, row_color, cell_rect)
                pygame.draw.rect(surface, styles.PANEL_BORDER, cell_rect, 1)
                
                # Cell text
                text_surface = self.font.render(str(cell), True, styles.TEXT_PRIMARY)
                text_rect = text_surface.get_rect(center=cell_rect.center)
                surface.blit(text_surface, text_rect)
                
                current_x += col_widths[col_idx]
            
            current_y += cell_height
        
        return current_y - self.y


class Label:
    """Simple text label component."""
    
    def __init__(self, x, y, text, font=None, color=None):
        """
        Initialize a Label.
        
        Args:
            x, y: Position coordinates
            text: Label text
            font: Font for text
            color: Text color
        """
        self.x = x
        self.y = y
        self.text = text
        self.font = font or styles.FONT_NORMAL
        self.color = color or styles.TEXT_PRIMARY
        
    def draw(self, surface):
        """Draw the label."""
        text_surface = self.font.render(self.text, True, self.color)
        surface.blit(text_surface, (self.x, self.y))
    
    def set_text(self, text):
        """Update label text."""
        self.text = text
