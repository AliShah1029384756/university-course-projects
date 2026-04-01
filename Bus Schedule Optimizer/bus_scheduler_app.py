"""
Transportation Optimization System - Main Application

Entry point for the Pygame-based GUI application solving OR problems:
- Linear Programming (Simplex)
- Assignment Problem (Hungarian)
- Transportation Problem (NW Corner, Least-Cost, MODI)
"""

import pygame
import sys
import traceback
from gui import styles
from gui.components import Button, InputBox, ScrollPanel, Label
from gui.gui_core import (ScreenManager, Screen, LayoutManager, 
                          draw_header, draw_status_bar, parse_matrix_input,
                          format_matrix_output, validate_matrix)
from solvers.route_optimizer_simplex import SimplexSolver
from solvers.driver_assignment_hungarian import AssignmentSolver
from solvers.bus_distribution_modi import TransportationSolver


class MainMenuScreen(Screen):
    """
    Main menu screen with problem type selection.
    
    Shows three big tiles for Simplex, Assignment, and Transportation,
    plus an Exit button.
    """
    
    def __init__(self, screen_manager):
        super().__init__(screen_manager)
        self.title = "Bus Scheduling Optimizer"
        self.status_message = "Ready"
        self.status_type = "info"
        
    def on_enter(self):
        """Initialize menu components."""
        self.components = []
        
        # Calculate tile positions (3 columns)
        tile_width = 300
        tile_height = 200
        spacing = 40
        start_x = (styles.SCREEN_WIDTH - (3 * tile_width + 2 * spacing)) // 2
        start_y = 250
        
        # Problem type buttons
        simplex_btn = Button(
            start_x, start_y, tile_width, tile_height,
            "Route Optimization\n(Linear Programming)",
            callback=lambda: self.screen_manager.switch_screen('simplex'),
            color=styles.PRIMARY_BLUE,
            font=styles.FONT_SUBTITLE
        )
        
        assignment_btn = Button(
            start_x + tile_width + spacing, start_y, tile_width, tile_height,
            "Driver Assignment\n(Hungarian Algorithm)",
            callback=lambda: self.screen_manager.switch_screen('assignment'),
            color=styles.PRIMARY_BLUE,
            font=styles.FONT_SUBTITLE
        )
        
        transport_btn = Button(
            start_x + 2 * (tile_width + spacing), start_y, tile_width, tile_height,
            "Bus Distribution\n(Transportation)",
            callback=lambda: self.screen_manager.switch_screen('transportation'),
            color=styles.PRIMARY_BLUE,
            font=styles.FONT_SUBTITLE
        )
        
        # Exit button at bottom
        exit_btn = Button(
            styles.SCREEN_WIDTH // 2 - 100, start_y + tile_height + 100,
            200, 50,
            "Exit",
            callback=self._exit_app,
            color=styles.ERROR_RED,
            font=styles.FONT_NORMAL
        )
        
        self.components = [simplex_btn, assignment_btn, transport_btn, exit_btn]
        
        for component in self.components:
            self.event_dispatcher.register(component)
    
    def _exit_app(self):
        """Exit the application."""
        pygame.quit()
        sys.exit()
    
    def draw(self, surface):
        """Draw the main menu."""
        surface.fill(styles.LIGHT_GRAY)
        
        # Header
        draw_header(surface, self.title, styles.SCREEN_WIDTH)
        
        # Subtitle
        subtitle = styles.FONT_LARGE.render("Select Optimization Method", True, styles.TEXT_PRIMARY)
        subtitle_rect = subtitle.get_rect(center=(styles.SCREEN_WIDTH // 2, 150))
        surface.blit(subtitle, subtitle_rect)
        
        # Description
        desc = styles.FONT_NORMAL.render(
            "Optimize bus routes, driver assignments, and resource allocation",
            True, styles.TEXT_SECONDARY
        )
        desc_rect = desc.get_rect(center=(styles.SCREEN_WIDTH // 2, 200))
        surface.blit(desc, desc_rect)
        
        # Draw components
        super().draw(surface)
        
        # Status bar
        draw_status_bar(surface, self.status_message, 
                       styles.SCREEN_HEIGHT - styles.STATUS_BAR_HEIGHT,
                       styles.SCREEN_WIDTH, self.status_type)


class SimplexScreen(Screen):
    """Screen for Linear Programming / Simplex problem."""
    
    def __init__(self, screen_manager):
        super().__init__(screen_manager)
        self.solver = None
        self.status_message = "Enter problem data or load example"
        self.status_type = "info"
        
    def on_enter(self):
        """Initialize Simplex screen components."""
        self.components = []
        
        # Input panel (left side)
        panel_x = 20
        panel_y = styles.HEADER_HEIGHT + 20
        panel_width = 500
        
        # Input fields
        self.obj_coef_input = InputBox(
            panel_x, panel_y, panel_width, 35,
            label="Objective Coefficients (c):",
            default_text="3, 5",
            font=styles.FONT_SMALL
        )
        
        self.constraint_matrix_input = InputBox(
            panel_x, panel_y + 70, panel_width, 100,
            label="Constraint Matrix (A) - one row per line:",
            default_text="1, 0\n0, 2\n3, 2",
            multiline=True,
            font=styles.FONT_SMALL
        )
        
        self.rhs_input = InputBox(
            panel_x, panel_y + 200, panel_width, 35,
            label="Right-Hand Side (b):",
            default_text="4, 12, 18",
            font=styles.FONT_SMALL
        )
        
        self.constraint_types_input = InputBox(
            panel_x, panel_y + 270, panel_width, 35,
            label="Constraint Types (<=, >=, =):",
            default_text="<=, <=, <=",
            font=styles.FONT_SMALL
        )
        
        self.prob_type_input = InputBox(
            panel_x, panel_y + 340, panel_width, 35,
            label="Problem Type:",
            default_text="max",
            font=styles.FONT_SMALL
        )
        
        # Control buttons
        solve_btn = Button(
            panel_x, panel_y + 400, 150, 40,
            "Solve",
            callback=self._solve,
            color=styles.SUCCESS_GREEN
        )
        
        example_btn = Button(
            panel_x + 160, panel_y + 400, 150, 40,
            "Load Example",
            callback=self._load_example
        )
        
        clear_btn = Button(
            panel_x + 320, panel_y + 400, 150, 40,
            "Clear",
            callback=self._clear,
            color=styles.WARNING_ORANGE
        )
        
        # Back button
        back_btn = Button(
            panel_x, panel_y + 450, 150, 40,
            "← Back",
            callback=lambda: self.screen_manager.switch_screen('main'),
            color=styles.DARK_GRAY,
            font=styles.FONT_NORMAL
        )
        
        # Output panel (right side)
        output_x = panel_x + panel_width + 40
        output_y = panel_y
        output_width = styles.SCREEN_WIDTH - output_x - 40
        output_height = styles.SCREEN_HEIGHT - output_y - 60
        
        self.output_panel = ScrollPanel(output_x, output_y, output_width, output_height)
        self.output_panel.set_content_lines(["Solution will appear here..."], styles.FONT_MONO_SMALL)
        
        self.components = [
            back_btn,
            self.obj_coef_input,
            self.constraint_matrix_input,
            self.rhs_input,
            self.constraint_types_input,
            self.prob_type_input,
            solve_btn,
            example_btn,
            clear_btn,
            self.output_panel
        ]
        
        for component in self.components:
            self.event_dispatcher.register(component)
    
    def _load_example(self):
        """Load an example problem."""
        self.obj_coef_input.set_value("3, 5")
        self.constraint_matrix_input.set_value("1, 0\n0, 2\n3, 2")
        self.rhs_input.set_value("4, 12, 18")
        self.constraint_types_input.set_value("<=, <=, <=")
        self.prob_type_input.set_value("max")
        self.status_message = "Example loaded"
        self.status_type = "success"
    
    def _clear(self):
        """Clear all inputs."""
        self.obj_coef_input.set_value("")
        self.constraint_matrix_input.set_value("")
        self.rhs_input.set_value("")
        self.constraint_types_input.set_value("")
        self.prob_type_input.set_value("max")
        self.output_panel.set_content_lines(["Solution will appear here..."], styles.FONT_MONO_SMALL)
        self.status_message = "Cleared"
        self.status_type = "info"
    
    def _solve(self):
        """Solve the LP problem."""
        try:
            # Parse inputs
            c = [float(x.strip()) for x in self.obj_coef_input.get_value().split(',')]
            A_text = self.constraint_matrix_input.get_value()
            A = parse_matrix_input(A_text)
            b = [float(x.strip()) for x in self.rhs_input.get_value().split(',')]
            constraint_types = [x.strip() for x in self.constraint_types_input.get_value().split(',')]
            prob_type = self.prob_type_input.get_value().strip()
            
            if A is None:
                raise ValueError("Invalid constraint matrix format")
            
            # Validate dimensions
            if len(A) != len(b):
                raise ValueError(f"Matrix has {len(A)} rows but RHS has {len(b)} values")
            
            if len(A) != len(constraint_types):
                raise ValueError(f"Need {len(A)} constraint types, got {len(constraint_types)}")
            
            # Solve
            self.status_message = "Solving..."
            self.status_type = "info"
            
            solver = SimplexSolver(c, A, b, constraint_types, prob_type)
            solution, optimal_value, status, steps = solver.solve('bigm')
            
            # Display solution
            self.output_panel.set_content_lines(steps, styles.FONT_MONO_SMALL)
            self.status_message = f"Solution found: {status}"
            self.status_type = "success" if status == 'optimal' else "warning"
            
        except Exception as e:
            error_lines = [
                "ERROR:",
                "",
                str(e),
                "",
                "Please check your inputs and try again."
            ]
            self.output_panel.set_content_lines(error_lines, styles.FONT_MONO_SMALL)
            self.status_message = f"Error: {str(e)}"
            self.status_type = "error"
    
    def draw(self, surface):
        """Draw the simplex solver screen."""
        surface.fill(styles.LIGHT_GRAY)
        
        # Header
        draw_header(surface, "Route Optimization - Linear Programming (Simplex)", styles.SCREEN_WIDTH)
        
        # Draw components
        super().draw(surface)
        
        # Status bar
        draw_status_bar(surface, self.status_message,
                       styles.SCREEN_HEIGHT - styles.STATUS_BAR_HEIGHT,
                       styles.SCREEN_WIDTH, self.status_type)


class AssignmentScreen(Screen):
    """Screen for Assignment Problem."""
    
    def __init__(self, screen_manager):
        super().__init__(screen_manager)
        self.solver = None
        self.status_message = "Enter cost matrix or load example"
        self.status_type = "info"
        
    def on_enter(self):
        """Initialize Assignment screen components."""
        self.components = []
        
        # Input panel
        panel_x = 20
        panel_y = styles.HEADER_HEIGHT + 20
        panel_width = 500
        
        self.cost_matrix_input = InputBox(
            panel_x, panel_y, panel_width, 150,
            label="Cost Matrix (rows=workers, cols=tasks):",
            default_text="9, 2, 7, 8\n6, 4, 3, 7\n5, 8, 1, 8\n7, 6, 9, 4",
            multiline=True,
            font=styles.FONT_SMALL
        )
        
        # Control buttons
        solve_btn = Button(
            panel_x, panel_y + 180, 150, 40,
            "Solve",
            callback=self._solve,
            color=styles.SUCCESS_GREEN
        )
        
        example_btn = Button(
            panel_x + 160, panel_y + 180, 150, 40,
            "Load Example",
            callback=self._load_example
        )
        
        clear_btn = Button(
            panel_x + 320, panel_y + 180, 150, 40,
            "Clear",
            callback=self._clear,
            color=styles.WARNING_ORANGE
        )
        
        # Back button
        back_btn = Button(
            panel_x, panel_y + 230, 150, 40,
            "← Back",
            callback=lambda: self.screen_manager.switch_screen('main'),
            color=styles.DARK_GRAY,
            font=styles.FONT_NORMAL
        )
        
        # Output panel
        output_x = panel_x + panel_width + 40
        output_y = panel_y
        output_width = styles.SCREEN_WIDTH - output_x - 40
        output_height = styles.SCREEN_HEIGHT - output_y - 60
        
        self.output_panel = ScrollPanel(output_x, output_y, output_width, output_height)
        self.output_panel.set_content_lines(["Solution will appear here..."], styles.FONT_MONO_SMALL)
        
        self.components = [
            back_btn,
            self.cost_matrix_input,
            solve_btn,
            example_btn,
            clear_btn,
            self.output_panel
        ]
        
        for component in self.components:
            self.event_dispatcher.register(component)
    
    def _load_example(self):
        """Load example assignment problem."""
        self.cost_matrix_input.set_value("9, 2, 7, 8\n6, 4, 3, 7\n5, 8, 1, 8\n7, 6, 9, 4")
        self.status_message = "Example loaded"
        self.status_type = "success"
    
    def _clear(self):
        """Clear inputs."""
        self.cost_matrix_input.set_value("")
        self.output_panel.set_content_lines(["Solution will appear here..."], styles.FONT_MONO_SMALL)
        self.status_message = "Cleared"
        self.status_type = "info"
    
    def _solve(self):
        """Solve the assignment problem."""
        try:
            # Parse cost matrix
            cost_matrix = parse_matrix_input(self.cost_matrix_input.get_value())
            
            if cost_matrix is None:
                raise ValueError("Invalid matrix format")
            
            # Validate
            is_valid, error_msg = validate_matrix(cost_matrix)
            if not is_valid:
                raise ValueError(error_msg)
            
            # Solve
            self.status_message = "Solving..."
            self.status_type = "info"
            
            solver = AssignmentSolver(cost_matrix)
            assignments, total_cost, steps = solver.solve()
            
            # Display solution
            self.output_panel.set_content_lines(steps, styles.FONT_MONO_SMALL)
            self.status_message = f"Optimal cost: {total_cost:.2f}"
            self.status_type = "success"
            
        except Exception as e:
            error_lines = [
                "ERROR:",
                "",
                str(e),
                "",
                "Traceback:",
                traceback.format_exc()
            ]
            self.output_panel.set_content_lines(error_lines, styles.FONT_MONO_SMALL)
            self.status_message = f"Error: {str(e)}"
            self.status_type = "error"
    
    def draw(self, surface):
        """Draw the assignment solver screen."""
        surface.fill(styles.LIGHT_GRAY)
        
        # Header
        draw_header(surface, "Driver Assignment - Hungarian Algorithm", styles.SCREEN_WIDTH)
        
        # Draw components
        super().draw(surface)
        
        # Status bar
        draw_status_bar(surface, self.status_message,
                       styles.SCREEN_HEIGHT - styles.STATUS_BAR_HEIGHT,
                       styles.SCREEN_WIDTH, self.status_type)


class TransportationScreen(Screen):
    """Screen for Transportation Problem."""
    
    def __init__(self, screen_manager):
        super().__init__(screen_manager)
        self.solver = None
        self.status_message = "Enter problem data or load example"
        self.status_type = "info"
        
    def on_enter(self):
        """Initialize Transportation screen components."""
        self.components = []
        
        # Input panel
        panel_x = 20
        panel_y = styles.HEADER_HEIGHT + 20
        panel_width = 500
        
        self.cost_matrix_input = InputBox(
            panel_x, panel_y, panel_width, 100,
            label="Cost Matrix (sources × destinations):",
            default_text="8, 6, 10\n9, 12, 13\n14, 9, 16",
            multiline=True,
            font=styles.FONT_SMALL
        )
        
        self.supply_input = InputBox(
            panel_x, panel_y + 130, panel_width, 35,
            label="Supply (one per source):",
            default_text="20, 30, 25",
            font=styles.FONT_SMALL
        )
        
        self.demand_input = InputBox(
            panel_x, panel_y + 190, panel_width, 35,
            label="Demand (one per destination):",
            default_text="15, 25, 35",
            font=styles.FONT_SMALL
        )
        
        self.method_input = InputBox(
            panel_x, panel_y + 250, panel_width, 35,
            label="Initial Method (northwest/least_cost):",
            default_text="least_cost",
            font=styles.FONT_SMALL
        )
        
        # Control buttons
        solve_btn = Button(
            panel_x, panel_y + 310, 150, 40,
            "Solve",
            callback=self._solve,
            color=styles.SUCCESS_GREEN
        )
        
        example_btn = Button(
            panel_x + 160, panel_y + 310, 150, 40,
            "Load Example",
            callback=self._load_example
        )
        
        clear_btn = Button(
            panel_x + 320, panel_y + 310, 150, 40,
            "Clear",
            callback=self._clear,
            color=styles.WARNING_ORANGE
        )
        
        # Back button
        back_btn = Button(
            panel_x, panel_y + 360, 150, 40,
            "← Back",
            callback=lambda: self.screen_manager.switch_screen('main'),
            color=styles.DARK_GRAY,
            font=styles.FONT_NORMAL
        )
        
        # Output panel
        output_x = panel_x + panel_width + 40
        output_y = panel_y
        output_width = styles.SCREEN_WIDTH - output_x - 40
        output_height = styles.SCREEN_HEIGHT - output_y - 60
        
        self.output_panel = ScrollPanel(output_x, output_y, output_width, output_height)
        self.output_panel.set_content_lines(["Solution will appear here..."], styles.FONT_MONO_SMALL)
        
        self.components = [
            back_btn,
            self.cost_matrix_input,
            self.supply_input,
            self.demand_input,
            self.method_input,
            solve_btn,
            example_btn,
            clear_btn,
            self.output_panel
        ]
        
        for component in self.components:
            self.event_dispatcher.register(component)
    
    def _load_example(self):
        """Load example transportation problem."""
        self.cost_matrix_input.set_value("8, 6, 10\n9, 12, 13\n14, 9, 16")
        self.supply_input.set_value("20, 30, 25")
        self.demand_input.set_value("15, 25, 35")
        self.method_input.set_value("least_cost")
        self.status_message = "Example loaded"
        self.status_type = "success"
    
    def _clear(self):
        """Clear inputs."""
        self.cost_matrix_input.set_value("")
        self.supply_input.set_value("")
        self.demand_input.set_value("")
        self.method_input.set_value("northwest")
        self.output_panel.set_content_lines(["Solution will appear here..."], styles.FONT_MONO_SMALL)
        self.status_message = "Cleared"
        self.status_type = "info"
    
    def _solve(self):
        """Solve the transportation problem."""
        try:
            # Parse inputs
            costs = parse_matrix_input(self.cost_matrix_input.get_value())
            supply = [float(x.strip()) for x in self.supply_input.get_value().split(',')]
            demand = [float(x.strip()) for x in self.demand_input.get_value().split(',')]
            method = self.method_input.get_value().strip().lower()
            
            if costs is None:
                raise ValueError("Invalid cost matrix format")
            
            # Validate dimensions
            if len(costs) != len(supply):
                raise ValueError(f"Cost matrix has {len(costs)} rows but {len(supply)} supply values")
            
            if len(costs[0]) != len(demand):
                raise ValueError(f"Cost matrix has {len(costs[0])} columns but {len(demand)} demand values")
            
            # Solve
            self.status_message = "Solving..."
            self.status_type = "info"
            
            solver = TransportationSolver(costs, supply, demand)
            allocation, total_cost, steps = solver.solve(initial_method=method, optimize=True)
            
            # Display solution
            self.output_panel.set_content_lines(steps, styles.FONT_MONO_SMALL)
            self.status_message = f"Minimum cost: {total_cost:.2f}"
            self.status_type = "success"
            
        except Exception as e:
            error_lines = [
                "ERROR:",
                "",
                str(e),
                "",
                "Traceback:",
                traceback.format_exc()
            ]
            self.output_panel.set_content_lines(error_lines, styles.FONT_MONO_SMALL)
            self.status_message = f"Error: {str(e)}"
            self.status_type = "error"
    
    def draw(self, surface):
        """Draw the transportation solver screen."""
        surface.fill(styles.LIGHT_GRAY)
        
        # Header
        draw_header(surface, "Bus Distribution - Transportation Problem (MODI)", styles.SCREEN_WIDTH)
        
        # Draw components
        super().draw(surface)
        
        # Status bar
        draw_status_bar(surface, self.status_message,
                       styles.SCREEN_HEIGHT - styles.STATUS_BAR_HEIGHT,
                       styles.SCREEN_WIDTH, self.status_type)


class Application:
    """
    Main application class.
    
    Manages Pygame initialization, screen management, and main event loop.
    """
    
    def __init__(self):
        """Initialize the application."""
        pygame.init()
        
        # Set up display
        self.screen = pygame.display.set_mode((styles.SCREEN_WIDTH, styles.SCREEN_HEIGHT))
        pygame.display.set_caption("Bus Scheduling Optimizer")
        
        self.clock = pygame.time.Clock()
        self.running = True
        self.fullscreen = False
        
        # Screen manager
        self.screen_manager = ScreenManager()
        
        # Register screens
        self.screen_manager.register_screen('main', MainMenuScreen(self.screen_manager))
        self.screen_manager.register_screen('simplex', SimplexScreen(self.screen_manager))
        self.screen_manager.register_screen('assignment', AssignmentScreen(self.screen_manager))
        self.screen_manager.register_screen('transportation', TransportationScreen(self.screen_manager))
        
        # Start with main menu
        self.screen_manager.switch_screen('main')
        
        # Toggle button for fullscreen/windowed mode
        self.toggle_btn = Button(
            styles.SCREEN_WIDTH - 180, 10, 170, 30,
            "Switch to Fullscreen",
            callback=self._toggle_fullscreen,
            color=styles.SECONDARY_YELLOW,
            text_color=styles.TEXT_PRIMARY,
            font=styles.FONT_SMALL
        )
    
    def _toggle_fullscreen(self):
        """Toggle between fullscreen and windowed mode."""
        self.fullscreen = not self.fullscreen
        if self.fullscreen:
            self.screen = pygame.display.set_mode((styles.SCREEN_WIDTH, styles.SCREEN_HEIGHT), 
                                                  pygame.FULLSCREEN)
            self.toggle_btn.text = "Switch to Windowed"
        else:
            self.screen = pygame.display.set_mode((styles.SCREEN_WIDTH, styles.SCREEN_HEIGHT))
            self.toggle_btn.text = "Switch to Fullscreen"
    
    def run(self):
        """Main application loop."""
        while self.running:
            dt = self.clock.tick(60) / 1000.0  # Delta time in seconds
            
            # Event handling
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.running = False
                
                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_ESCAPE:
                        # ESC returns to main menu or exits
                        current_screen = self.screen_manager.get_current_screen()
                        if isinstance(current_screen, MainMenuScreen):
                            self.running = False
                        else:
                            self.screen_manager.switch_screen('main')
                
                # Handle toggle button
                self.toggle_btn.handle_event(event)
                
                # Delegate to screen manager
                self.screen_manager.handle_event(event)
            
            # Update
            mouse_pos = pygame.mouse.get_pos()
            self.toggle_btn.update(mouse_pos)
            self.screen_manager.update(dt)
            
            # Draw
            self.screen_manager.draw(self.screen)
            
            # Draw toggle button
            self.toggle_btn.draw(self.screen)
            
            pygame.display.flip()
        
        pygame.quit()


def main():
    """Application entry point."""
    try:
        app = Application()
        app.run()
    except Exception as e:
        print("Fatal error:", str(e))
        traceback.print_exc()
        pygame.quit()
        sys.exit(1)


if __name__ == "__main__":
    main()
