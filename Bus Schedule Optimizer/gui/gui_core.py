"""
GUI Core - Layout Management and Event Handling Utilities

This module provides layout management utilities and event handling helpers
for building structured, responsive interfaces.
"""

import pygame
from gui import styles


class LayoutManager:
    """
    Manages layout of UI components with automatic positioning and spacing.
    
    Supports vertical stacking, horizontal arrangement, and grid layouts.
    """
    
    @staticmethod
    def vertical_stack(components, start_x, start_y, spacing=None):
        """
        Arrange components vertically with consistent spacing.
        
        Args:
            components: List of components with .rect attribute
            start_x: Starting X position
            start_y: Starting Y position
            spacing: Vertical spacing between components (defaults to PADDING_MEDIUM)
            
        Returns:
            Total height used by all components
        """
        spacing = spacing or styles.PADDING_MEDIUM
        current_y = start_y
        
        for component in components:
            if hasattr(component, 'rect'):
                component.rect.x = start_x
                component.rect.y = current_y
                current_y += component.rect.height + spacing
            elif hasattr(component, 'y'):  # For labels
                component.y = current_y
                current_y += 30 + spacing  # Approximate height for labels
        
        return current_y - start_y
    
    @staticmethod
    def horizontal_stack(components, start_x, start_y, spacing=None):
        """
        Arrange components horizontally with consistent spacing.
        
        Args:
            components: List of components with .rect attribute
            start_x: Starting X position
            start_y: Starting Y position
            spacing: Horizontal spacing between components (defaults to PADDING_MEDIUM)
            
        Returns:
            Total width used by all components
        """
        spacing = spacing or styles.PADDING_MEDIUM
        current_x = start_x
        
        for component in components:
            if hasattr(component, 'rect'):
                component.rect.x = current_x
                component.rect.y = start_y
                current_x += component.rect.width + spacing
        
        return current_x - start_x
    
    @staticmethod
    def grid(components, start_x, start_y, cols, spacing_x=None, spacing_y=None):
        """
        Arrange components in a grid layout.
        
        Args:
            components: List of components with .rect attribute
            start_x: Starting X position
            start_y: Starting Y position
            cols: Number of columns
            spacing_x: Horizontal spacing (defaults to PADDING_MEDIUM)
            spacing_y: Vertical spacing (defaults to PADDING_MEDIUM)
            
        Returns:
            Tuple of (total_width, total_height)
        """
        spacing_x = spacing_x or styles.PADDING_MEDIUM
        spacing_y = spacing_y or styles.PADDING_MEDIUM
        
        max_width = 0
        current_y = start_y
        
        for i in range(0, len(components), cols):
            row = components[i:i+cols]
            current_x = start_x
            row_height = 0
            
            for component in row:
                if hasattr(component, 'rect'):
                    component.rect.x = current_x
                    component.rect.y = current_y
                    current_x += component.rect.width + spacing_x
                    row_height = max(row_height, component.rect.height)
            
            max_width = max(max_width, current_x - start_x)
            current_y += row_height + spacing_y
        
        return max_width, current_y - start_y
    
    @staticmethod
    def center_component(component, container_rect):
        """
        Center a component within a container.
        
        Args:
            component: Component with .rect attribute
            container_rect: pygame.Rect representing container bounds
        """
        if hasattr(component, 'rect'):
            component.rect.center = container_rect.center
    
    @staticmethod
    def align_right(component, container_rect, padding=None):
        """
        Align a component to the right of a container.
        
        Args:
            component: Component with .rect attribute
            container_rect: pygame.Rect representing container bounds
            padding: Right padding (defaults to PADDING_MEDIUM)
        """
        padding = padding or styles.PADDING_MEDIUM
        if hasattr(component, 'rect'):
            component.rect.right = container_rect.right - padding
            component.rect.centery = container_rect.centery


class EventDispatcher:
    """
    Centralized event handling for multiple components.
    
    Manages event distribution and prevents event conflicts.
    """
    
    def __init__(self):
        """Initialize the event dispatcher."""
        self.components = []
        self.focused_component = None
    
    def register(self, component):
        """
        Register a component for event handling.
        
        Args:
            component: Component with handle_event method
        """
        if component not in self.components:
            self.components.append(component)
    
    def unregister(self, component):
        """
        Unregister a component from event handling.
        
        Args:
            component: Component to remove
        """
        if component in self.components:
            self.components.remove(component)
    
    def dispatch(self, event):
        """
        Dispatch event to all registered components.
        
        Args:
            event: pygame.Event to dispatch
            
        Returns:
            True if event was handled, False otherwise
        """
        # Priority to focused component
        if self.focused_component and hasattr(self.focused_component, 'handle_event'):
            if self.focused_component.handle_event(event):
                return True
        
        # Dispatch to other components (reverse order for top-to-bottom)
        for component in reversed(self.components):
            if component != self.focused_component and hasattr(component, 'handle_event'):
                if component.handle_event(event):
                    return True
        
        return False
    
    def set_focus(self, component):
        """Set focused component."""
        self.focused_component = component
    
    def clear_focus(self):
        """Clear focused component."""
        self.focused_component = None


class Screen:
    """
    Base class for application screens.
    
    Provides template methods for initialization, event handling, update, and rendering.
    """
    
    def __init__(self, screen_manager):
        """
        Initialize a Screen.
        
        Args:
            screen_manager: Reference to the ScreenManager
        """
        self.screen_manager = screen_manager
        self.event_dispatcher = EventDispatcher()
        self.components = []
        
    def on_enter(self):
        """Called when screen becomes active. Override in subclasses."""
        pass
    
    def on_exit(self):
        """Called when screen is deactivated. Override in subclasses."""
        pass
    
    def handle_event(self, event):
        """
        Handle pygame events.
        
        Args:
            event: pygame.Event to process
            
        Returns:
            True if event was handled, False otherwise
        """
        return self.event_dispatcher.dispatch(event)
    
    def update(self, dt):
        """
        Update screen state.
        
        Args:
            dt: Delta time since last update (seconds)
        """
        mouse_pos = pygame.mouse.get_pos()
        for component in self.components:
            if hasattr(component, 'update'):
                # Check if update method accepts mouse_pos parameter
                import inspect
                sig = inspect.signature(component.update)
                if len(sig.parameters) > 0:  # Accepts parameters (Button)
                    component.update(mouse_pos)
                else:  # No parameters (InputBox, ScrollPanel)
                    component.update()
    
    def draw(self, surface):
        """
        Draw screen content.
        
        Args:
            surface: pygame.Surface to draw on
        """
        for component in self.components:
            if hasattr(component, 'draw'):
                component.draw(surface)


class ScreenManager:
    """
    Manages multiple screens and handles transitions.
    
    Maintains screen stack and handles screen switching.
    """
    
    def __init__(self):
        """Initialize the screen manager."""
        self.screens = {}
        self.screen_stack = []
        self.current_screen = None
    
    def register_screen(self, name, screen):
        """
        Register a screen with a name.
        
        Args:
            name: Unique screen identifier
            screen: Screen instance
        """
        self.screens[name] = screen
    
    def push_screen(self, name):
        """
        Push a screen onto the stack and make it active.
        
        Args:
            name: Name of registered screen
        """
        if self.current_screen:
            self.current_screen.on_exit()
            self.screen_stack.append(self.current_screen)
        
        if name in self.screens:
            self.current_screen = self.screens[name]
            self.current_screen.on_enter()
    
    def pop_screen(self):
        """Pop the current screen and return to previous."""
        if self.current_screen:
            self.current_screen.on_exit()
        
        if self.screen_stack:
            self.current_screen = self.screen_stack.pop()
            self.current_screen.on_enter()
        else:
            self.current_screen = None
    
    def switch_screen(self, name):
        """
        Switch to a screen, clearing the stack.
        
        Args:
            name: Name of registered screen
        """
        if self.current_screen:
            self.current_screen.on_exit()
        
        self.screen_stack.clear()
        
        if name in self.screens:
            self.current_screen = self.screens[name]
            self.current_screen.on_enter()
    
    def get_current_screen(self):
        """Get the currently active screen."""
        return self.current_screen
    
    def handle_event(self, event):
        """Delegate event to current screen."""
        if self.current_screen:
            return self.current_screen.handle_event(event)
        return False
    
    def update(self, dt):
        """Update current screen."""
        if self.current_screen:
            self.current_screen.update(dt)
    
    def draw(self, surface):
        """Draw current screen."""
        if self.current_screen:
            self.current_screen.draw(surface)


def draw_header(surface, title, width):
    """
    Draw the application header bar.
    
    Args:
        surface: pygame.Surface to draw on
        title: Header title text
        width: Header width
    """
    header_rect = pygame.Rect(0, 0, width, styles.HEADER_HEIGHT)
    
    # Header background with gradient effect
    pygame.draw.rect(surface, styles.PRIMARY_BLUE, header_rect)
    
    # Title text
    title_surface = styles.FONT_TITLE.render(title, True, styles.WHITE)
    title_rect = title_surface.get_rect(midleft=(styles.PADDING_LARGE, styles.HEADER_HEIGHT // 2))
    surface.blit(title_surface, title_rect)


def draw_status_bar(surface, text, y_pos, width, status_type="info"):
    """
    Draw a status bar with message.
    
    Args:
        surface: pygame.Surface to draw on
        text: Status message
        y_pos: Y position of status bar
        width: Status bar width
        status_type: Type of status ("info", "success", "warning", "error")
    """
    status_rect = pygame.Rect(0, y_pos, width, styles.STATUS_BAR_HEIGHT)
    
    # Choose color based on status type
    color_map = {
        "info": styles.INFO_BLUE,
        "success": styles.SUCCESS_GREEN,
        "warning": styles.WARNING_ORANGE,
        "error": styles.ERROR_RED
    }
    bg_color = color_map.get(status_type, styles.INFO_BLUE)
    
    pygame.draw.rect(surface, bg_color, status_rect)
    
    # Status text
    text_surface = styles.FONT_SMALL.render(text, True, styles.WHITE)
    text_rect = text_surface.get_rect(midleft=(styles.PADDING_MEDIUM, y_pos + styles.STATUS_BAR_HEIGHT // 2))
    surface.blit(text_surface, text_rect)


def create_panel(x, y, width, height, title=""):
    """
    Create a styled panel surface with optional title.
    
    Args:
        x, y: Panel position
        width, height: Panel dimensions
        title: Optional panel title
        
    Returns:
        pygame.Rect representing the panel area
    """
    panel_rect = pygame.Rect(x, y, width, height)
    return panel_rect


def parse_matrix_input(text):
    """
    Parse matrix input from text string.
    
    Supports formats:
    - CSV: "1,2,3\n4,5,6"
    - Space-separated: "1 2 3\n4 5 6"
    - Mixed
    
    Args:
        text: Input string
        
    Returns:
        2D list of numbers, or None if parsing fails
    """
    try:
        lines = text.strip().split('\n')
        matrix = []
        
        for line in lines:
            line = line.strip()
            if not line:
                continue
            
            # Try comma-separated first
            if ',' in line:
                row = [float(x.strip()) for x in line.split(',') if x.strip()]
            else:
                # Space-separated
                row = [float(x.strip()) for x in line.split() if x.strip()]
            
            if row:
                matrix.append(row)
        
        return matrix if matrix else None
        
    except (ValueError, AttributeError):
        return None


def format_matrix_output(matrix, headers=None, row_labels=None, precision=2):
    """
    Format matrix as aligned text for display.
    
    Args:
        matrix: 2D list of numbers
        headers: Optional column headers
        row_labels: Optional row labels
        precision: Decimal precision for formatting
        
    Returns:
        List of formatted text lines
    """
    if not matrix:
        return []
    
    lines = []
    
    # Convert to strings with precision
    str_matrix = [[f"{cell:.{precision}f}" if isinstance(cell, (int, float)) else str(cell) 
                   for cell in row] for row in matrix]
    
    # Calculate column widths
    col_widths = [0] * len(str_matrix[0])
    
    if headers:
        for i, header in enumerate(headers):
            col_widths[i] = len(str(header))
    
    for row in str_matrix:
        for i, cell in enumerate(row):
            col_widths[i] = max(col_widths[i], len(cell))
    
    if row_labels:
        label_width = max(len(str(label)) for label in row_labels)
    else:
        label_width = 0
    
    # Build header line
    if headers:
        header_line = " " * (label_width + 2) if row_labels else ""
        header_line += "  ".join(h.rjust(col_widths[i]) for i, h in enumerate(headers))
        lines.append(header_line)
        lines.append("-" * len(header_line))
    
    # Build data lines
    for i, row in enumerate(str_matrix):
        row_label = str(row_labels[i]).ljust(label_width) + "| " if row_labels and i < len(row_labels) else ""
        row_str = "  ".join(cell.rjust(col_widths[j]) for j, cell in enumerate(row))
        lines.append(row_label + row_str)
    
    return lines


def validate_matrix(matrix, expected_rows=None, expected_cols=None):
    """
    Validate matrix structure and dimensions.
    
    Args:
        matrix: 2D list to validate
        expected_rows: Expected number of rows (None to skip check)
        expected_cols: Expected number of columns (None to skip check)
        
    Returns:
        Tuple of (is_valid, error_message)
    """
    if not matrix:
        return False, "Matrix is empty"
    
    if not all(isinstance(row, list) for row in matrix):
        return False, "Invalid matrix structure"
    
    # Check rectangular shape
    col_count = len(matrix[0])
    if not all(len(row) == col_count for row in matrix):
        return False, "Matrix rows have different lengths"
    
    # Check numeric values
    for row in matrix:
        for val in row:
            if not isinstance(val, (int, float)):
                return False, f"Non-numeric value found: {val}"
    
    # Check dimensions
    if expected_rows is not None and len(matrix) != expected_rows:
        return False, f"Expected {expected_rows} rows, got {len(matrix)}"
    
    if expected_cols is not None and col_count != expected_cols:
        return False, f"Expected {expected_cols} columns, got {col_count}"
    
    return True, ""
