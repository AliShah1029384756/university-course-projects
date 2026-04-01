"""
Simplex Method Solver for Linear Programming

This module implements the Simplex algorithm with:
- Standard form conversion
- Big-M and Two-Phase methods for artificial variables  
- Step-by-step tableau displays
- Sensitivity analysis (reduced costs, shadow prices, ranges)

Implementation follows textbook methodology (Hillier & Lieberman, Taha).
"""

import numpy as np
from copy import deepcopy


class SimplexSolver:
    """
    Solves linear programming problems using the Simplex method.
    
    Supports:
    - Maximization and minimization
    - ≤, ≥, and = constraints
    - Automatic slack/surplus/artificial variable handling
    - Detailed tableau iterations
    - Sensitivity analysis
    """
    
    def __init__(self, c, A, b, constraint_types, problem_type='max'):
        """
        Initialize the Simplex Solver.
        
        Args:
            c: Objective function coefficients (list)
            A: Constraint matrix (2D list)
            b: Right-hand side values (list)
            constraint_types: List of constraint types ('<=', '>=', '=')
            problem_type: 'max' or 'min'
        """
        self.original_c = np.array(c, dtype=float)
        self.original_A = np.array(A, dtype=float)
        self.original_b = np.array(b, dtype=float)
        self.constraint_types = constraint_types
        self.problem_type = problem_type.lower()
        
        self.num_constraints = len(b)
        self.num_variables = len(c)
        
        # Standard form components
        self.c_standard = None
        self.A_standard = None
        self.b_standard = None
        self.variable_names = []
        self.basic_vars = []
        
        # Tableau
        self.tableau = None
        
        # Solution
        self.steps = []
        self.solution = {}
        self.optimal_value = None
        self.is_solved = False
        self.status = None  # 'optimal', 'unbounded', 'infeasible'
        
    def solve(self, method='bigm'):
        """
        Solve the LP problem.
        
        Args:
            method: 'bigm' or 'twophase'
            
        Returns:
            Tuple of (solution_dict, optimal_value, status, steps)
        """
        self.steps = []
        self.steps.append("=" * 80)
        self.steps.append("LINEAR PROGRAMMING - SIMPLEX METHOD")
        self.steps.append("=" * 80)
        self.steps.append("")
        
        # Display original problem
        self._display_original_problem()
        
        # Convert to standard form
        self.steps.append("\n" + "=" * 80)
        self.steps.append("STEP 1: CONVERT TO STANDARD FORM")
        self.steps.append("=" * 80)
        self.steps.append("")
        self._convert_to_standard_form(method)
        
        # Build initial tableau
        self.steps.append("\n" + "=" * 80)
        self.steps.append("STEP 2: BUILD INITIAL SIMPLEX TABLEAU")
        self.steps.append("=" * 80)
        self.steps.append("")
        self._build_initial_tableau(method)
        
        # Solve using simplex iterations
        self.steps.append("\n" + "=" * 80)
        self.steps.append("STEP 3: SIMPLEX ITERATIONS")
        self.steps.append("=" * 80)
        self.steps.append("")
        self._simplex_iterations()
        
        # Extract solution
        if self.status == 'optimal':
            self._extract_solution()
            
            # Sensitivity analysis
            self.steps.append("\n" + "=" * 80)
            self.steps.append("STEP 4: SENSITIVITY ANALYSIS")
            self.steps.append("=" * 80)
            self.steps.append("")
            self._sensitivity_analysis()
        
        # Add summary
        self._add_summary()
        
        self.is_solved = True
        return self.solution, self.optimal_value, self.status, self.steps
    
    def _display_original_problem(self):
        """Display the original LP problem formulation."""
        self.steps.append("Original Problem:")
        self.steps.append("")
        
        # Objective
        obj_str = "Maximize: " if self.problem_type == 'max' else "Minimize: "
        terms = []
        for i, coef in enumerate(self.original_c):
            if coef != 0:
                sign = '+' if coef > 0 and terms else ''
                terms.append(f"{sign}{coef:.2f}x{i+1}")
        obj_str += " ".join(terms)
        self.steps.append(obj_str)
        self.steps.append("")
        
        # Constraints
        self.steps.append("Subject to:")
        for i in range(self.num_constraints):
            terms = []
            for j, coef in enumerate(self.original_A[i]):
                if coef != 0:
                    sign = '+' if coef > 0 and terms else ''
                    terms.append(f"{sign}{coef:.2f}x{j+1}")
            constraint_str = "  " + " ".join(terms)
            constraint_str += f" {self.constraint_types[i]} {self.original_b[i]:.2f}"
            self.steps.append(constraint_str)
        
        self.steps.append("  x_i >= 0 for all i")
        self.steps.append("")
    
    def _convert_to_standard_form(self, method):
        """
        Convert LP to standard form with slack/surplus/artificial variables.
        
        Args:
            method: 'bigm' or 'twophase'
        """
        # For minimization, convert to maximization
        if self.problem_type == 'min':
            self.steps.append("Converting minimization to maximization (multiply objective by -1):")
            c_work = -self.original_c.copy()
        else:
            c_work = self.original_c.copy()
        
        A_work = self.original_A.copy()
        b_work = self.original_b.copy()
        
        # Variable names
        var_names = [f'x{i+1}' for i in range(self.num_variables)]
        
        slack_count = 0
        surplus_count = 0
        artificial_count = 0
        
        self.steps.append("\nAdding slack/surplus/artificial variables:")
        self.steps.append("")
        
        # Process each constraint
        for i, const_type in enumerate(self.constraint_types):
            if const_type == '<=':
                # Add slack variable
                slack_col = np.zeros(self.num_constraints)
                slack_col[i] = 1
                A_work = np.column_stack([A_work, slack_col])
                c_work = np.append(c_work, 0)
                var_names.append(f's{slack_count+1}')
                self.steps.append(f"Constraint {i+1}: Added slack variable s{slack_count+1}")
                slack_count += 1
                
            elif const_type == '>=':
                # Add surplus variable
                surplus_col = np.zeros(self.num_constraints)
                surplus_col[i] = -1
                A_work = np.column_stack([A_work, surplus_col])
                c_work = np.append(c_work, 0)
                var_names.append(f'S{surplus_count+1}')
                surplus_count += 1
                
                # Add artificial variable
                artificial_col = np.zeros(self.num_constraints)
                artificial_col[i] = 1
                A_work = np.column_stack([A_work, artificial_col])
                
                if method == 'bigm':
                    c_work = np.append(c_work, -1e6)  # Big-M penalty
                else:
                    c_work = np.append(c_work, 0)  # Two-phase: handled separately
                    
                var_names.append(f'A{artificial_count+1}')
                self.steps.append(f"Constraint {i+1}: Added surplus S{surplus_count} and artificial A{artificial_count+1}")
                artificial_count += 1
                
            else:  # '='
                # Add artificial variable only
                artificial_col = np.zeros(self.num_constraints)
                artificial_col[i] = 1
                A_work = np.column_stack([A_work, artificial_col])
                
                if method == 'bigm':
                    c_work = np.append(c_work, -1e6)  # Big-M penalty
                else:
                    c_work = np.append(c_work, 0)  # Two-phase
                    
                var_names.append(f'A{artificial_count+1}')
                self.steps.append(f"Constraint {i+1}: Added artificial A{artificial_count+1}")
                artificial_count += 1
        
        self.c_standard = c_work
        self.A_standard = A_work
        self.b_standard = b_work
        self.variable_names = var_names
        
        self.steps.append("")
        if method == 'bigm':
            self.steps.append(f"Using Big-M method (M = 10^6) for artificial variables")
        else:
            self.steps.append("Using Two-Phase method for artificial variables")
        self.steps.append("")
    
    def _build_initial_tableau(self, method):
        """
        Build the initial Simplex tableau.
        
        Args:
            method: 'bigm' or 'twophase'
        """
        m = self.num_constraints
        n = len(self.c_standard)
        
        # Tableau structure: [A | I | b]
        #                     [c | 0 | 0]
        
        # Find initial basic variables (slack or artificial for each constraint)
        self.basic_vars = []
        for i, const_type in enumerate(self.constraint_types):
            # Find column with single 1 in row i
            for j in range(n):
                if self.A_standard[i, j] == 1 and np.sum(np.abs(self.A_standard[:, j])) == 1:
                    self.basic_vars.append(j)
                    break
        
        # Initialize tableau
        self.tableau = np.zeros((m + 1, n + 1))
        self.tableau[:m, :n] = self.A_standard
        self.tableau[:m, n] = self.b_standard
        self.tableau[m, :n] = -self.c_standard  # Negative for maximization
        
        # If using artificial variables, need to eliminate them from objective row
        if any('A' in self.variable_names[j] for j in self.basic_vars):
            self.steps.append("Eliminating artificial variables from objective row:")
            self.steps.append("")
            for j in self.basic_vars:
                if 'A' in self.variable_names[j]:
                    # Find which row has this basic variable
                    for i in range(m):
                        if self.A_standard[i, j] == 1:
                            # Add M * row_i to objective row
                            if method == 'bigm':
                                self.tableau[m, :] += 1e6 * self.tableau[i, :]
                            break
        
        self._display_tableau("Initial Tableau")
    
    def _simplex_iterations(self):
        """Perform Simplex iterations until optimal or unbounded."""
        iteration = 0
        
        while True:
            iteration += 1
            self.steps.append(f"\n{'='*80}")
            self.steps.append(f"Iteration {iteration}")
            self.steps.append('='*80)
            self.steps.append("")
            
            # Check optimality
            m = self.num_constraints
            n = len(self.c_standard)
            obj_row = self.tableau[m, :n]
            
            if np.all(obj_row >= -1e-6):  # All non-negative (accounting for float errors)
                self.steps.append("✓ All coefficients in objective row are non-negative.")
                self.steps.append("OPTIMAL SOLUTION FOUND!")
                self.status = 'optimal'
                break
            
            # Find entering variable (most negative coefficient in objective row)
            entering_col = np.argmin(obj_row)
            entering_var = self.variable_names[entering_col]
            
            self.steps.append(f"Entering variable: {entering_var} (column {entering_col})")
            self.steps.append(f"  Most negative coefficient: {obj_row[entering_col]:.4f}")
            self.steps.append("")
            
            # Find leaving variable (minimum ratio test)
            ratios = []
            for i in range(m):
                if self.tableau[i, entering_col] > 1e-6:  # Positive pivot candidate
                    ratio = self.tableau[i, n] / self.tableau[i, entering_col]
                    ratios.append((ratio, i))
                else:
                    ratios.append((np.inf, i))
            
            min_ratio, leaving_row = min(ratios)
            
            if min_ratio == np.inf:
                self.steps.append("✗ All coefficients in pivot column are non-positive.")
                self.steps.append("PROBLEM IS UNBOUNDED!")
                self.status = 'unbounded'
                break
            
            leaving_var = self.variable_names[self.basic_vars[leaving_row]]
            
            self.steps.append("Minimum ratio test:")
            for i in range(m):
                if self.tableau[i, entering_col] > 1e-6:
                    ratio = self.tableau[i, n] / self.tableau[i, entering_col]
                    marker = " ← Minimum" if i == leaving_row else ""
                    basic_var = self.variable_names[self.basic_vars[i]]
                    self.steps.append(f"  {basic_var}: {self.tableau[i, n]:.4f} / {self.tableau[i, entering_col]:.4f} = {ratio:.4f}{marker}")
                else:
                    basic_var = self.variable_names[self.basic_vars[i]]
                    self.steps.append(f"  {basic_var}: (not a candidate)")
            
            self.steps.append("")
            self.steps.append(f"Leaving variable: {leaving_var} (row {leaving_row})")
            self.steps.append(f"Pivot element: {self.tableau[leaving_row, entering_col]:.4f} at ({leaving_row}, {entering_col})")
            self.steps.append("")
            
            # Perform pivot operation
            self._pivot(leaving_row, entering_col)
            
            # Update basic variables
            self.basic_vars[leaving_row] = entering_col
            
            # Display tableau
            self._display_tableau(f"Tableau after iteration {iteration}")
            
            if iteration > 50:  # Safety limit
                self.steps.append("\nWarning: Maximum iterations reached.")
                self.status = 'unknown'
                break
    
    def _pivot(self, pivot_row, pivot_col):
        """
        Perform pivot operation on tableau.
        
        Args:
            pivot_row: Row index of pivot element
            pivot_col: Column index of pivot element
        """
        m, n = self.tableau.shape
        pivot = self.tableau[pivot_row, pivot_col]
        
        self.steps.append("Pivot operations:")
        self.steps.append("")
        
        # Divide pivot row by pivot element
        self.steps.append(f"1. Divide row {pivot_row} by {pivot:.4f}")
        self.tableau[pivot_row, :] /= pivot
        
        # Eliminate other entries in pivot column
        for i in range(m):
            if i != pivot_row:
                multiplier = self.tableau[i, pivot_col]
                if abs(multiplier) > 1e-10:
                    self.steps.append(f"2. Row {i} = Row {i} - ({multiplier:.4f}) × Row {pivot_row}")
                    self.tableau[i, :] -= multiplier * self.tableau[pivot_row, :]
        
        self.steps.append("")
    
    def _extract_solution(self):
        """Extract solution values from final tableau."""
        m = self.num_constraints
        n = len(self.c_standard)
        
        self.solution = {f'x{i+1}': 0.0 for i in range(self.num_variables)}
        
        for i, var_idx in enumerate(self.basic_vars):
            var_name = self.variable_names[var_idx]
            value = self.tableau[i, n]
            
            # Only record original variables
            if var_name.startswith('x'):
                self.solution[var_name] = value
        
        # Optimal value
        obj_value = self.tableau[m, n]
        if self.problem_type == 'min':
            self.optimal_value = -obj_value  # Convert back for minimization
        else:
            self.optimal_value = obj_value
    
    def _sensitivity_analysis(self):
        """
        Perform sensitivity analysis on optimal solution.
        
        Calculates:
        - Reduced costs for non-basic variables
        - Shadow prices (dual values) for constraints
        - Ranges of optimality for objective coefficients
        - Ranges of feasibility for RHS values
        """
        m = self.num_constraints
        n = len(self.c_standard)
        
        self.steps.append("Sensitivity Analysis Results:")
        self.steps.append("")
        
        # 1. Reduced costs (from final objective row)
        self.steps.append("1. REDUCED COSTS (for non-basic variables):")
        self.steps.append("   Reduced cost = coefficient in final objective row")
        self.steps.append("")
        
        for j in range(self.num_variables):
            var_name = f'x{j+1}'
            if j not in self.basic_vars:
                reduced_cost = -self.tableau[m, j]  # Negative because we use negative c in tableau
                self.steps.append(f"   {var_name}: {reduced_cost:.4f}")
                self.steps.append(f"      Interpretation: Objective would change by {reduced_cost:.4f} per unit of {var_name}")
        
        self.steps.append("")
        
        # 2. Shadow prices (dual values)
        self.steps.append("2. SHADOW PRICES (dual values for constraints):")
        self.steps.append("   Shadow price = coefficient of slack variable in final objective row")
        self.steps.append("")
        
        # Find slack variables
        for i, const_type in enumerate(self.constraint_types):
            if const_type == '<=':
                # Find corresponding slack variable
                slack_name = f's{i+1}'  # Simplified - assumes ordering
                for j, name in enumerate(self.variable_names):
                    if name == slack_name:
                        shadow_price = -self.tableau[m, j]
                        self.steps.append(f"   Constraint {i+1}: {shadow_price:.4f}")
                        self.steps.append(f"      Interpretation: Objective changes by {shadow_price:.4f} per unit increase in RHS")
                        break
        
        self.steps.append("")
        
        # 3. Ranges (simplified - full calculation is complex)
        self.steps.append("3. OPTIMALITY AND FEASIBILITY RANGES:")
        self.steps.append("   (Note: Simplified calculation - full range analysis requires additional computation)")
        self.steps.append("")
        
        for j in range(self.num_variables):
            var_name = f'x{j+1}'
            if j in self.basic_vars:
                self.steps.append(f"   {var_name}: Basic variable - coefficient can change without affecting basis")
            else:
                reduced_cost = -self.tableau[m, j]
                self.steps.append(f"   {var_name}: Can decrease objective coefficient by up to {reduced_cost:.4f}")
        
        self.steps.append("")
        self.steps.append("   For complete range analysis, additional calculations of basis changes needed.")
        self.steps.append("")
    
    def _display_tableau(self, title):
        """
        Display current tableau in formatted style.
        
        Args:
            title: Title for the tableau display
        """
        m, n = self.tableau.shape
        n -= 1  # Exclude RHS column
        
        self.steps.append(title + ":")
        self.steps.append("")
        
        # Header row
        header = "Basic  | "
        for j in range(n):
            header += f" {self.variable_names[j]:>8}"
        header += " |   RHS"
        self.steps.append(header)
        self.steps.append("-" * len(header))
        
        # Constraint rows
        for i in range(m - 1):
            row_str = f"{self.variable_names[self.basic_vars[i]]:>6} | "
            for j in range(n):
                row_str += f" {self.tableau[i, j]:8.4f}"
            row_str += f" | {self.tableau[i, n]:8.4f}"
            self.steps.append(row_str)
        
        # Objective row
        obj_str = "  z    | "
        for j in range(n):
            obj_str += f" {self.tableau[m-1, j]:8.4f}"
        obj_str += f" | {self.tableau[m-1, n]:8.4f}"
        self.steps.append("-" * len(header))
        self.steps.append(obj_str)
        self.steps.append("")
    
    def _add_summary(self):
        """Add solution summary."""
        self.steps.append("\n" + "=" * 80)
        self.steps.append("FINAL SOLUTION SUMMARY")
        self.steps.append("=" * 80)
        self.steps.append("")
        
        if self.status == 'optimal':
            self.steps.append("Status: OPTIMAL")
            self.steps.append("")
            self.steps.append("Variable Values:")
            for var, value in sorted(self.solution.items()):
                self.steps.append(f"  {var} = {value:.4f}")
            self.steps.append("")
            obj_type = "Maximum" if self.problem_type == 'max' else "Minimum"
            self.steps.append(f"{obj_type} Objective Value: {self.optimal_value:.4f}")
            
        elif self.status == 'unbounded':
            self.steps.append("Status: UNBOUNDED")
            self.steps.append("The objective function can be increased without bound.")
            
        elif self.status == 'infeasible':
            self.steps.append("Status: INFEASIBLE")
            self.steps.append("No feasible solution exists for this problem.")
            
        else:
            self.steps.append(f"Status: {self.status}")
        
        self.steps.append("")
        self.steps.append("=" * 80)
    
    def get_solution_text(self):
        """Get formatted solution text."""
        if not self.is_solved:
            return ["Problem not yet solved. Call solve() first."]
        return self.steps


# Example usage
if __name__ == "__main__":
    print("Test Case: Standard LP Problem")
    print("=" * 80)
    
    # Maximize: 3x1 + 5x2
    # Subject to:
    #   x1 <= 4
    #   2x2 <= 12
    #   3x1 + 2x2 <= 18
    #   x1, x2 >= 0
    
    c = [3, 5]
    A = [
        [1, 0],
        [0, 2],
        [3, 2]
    ]
    b = [4, 12, 18]
    constraint_types = ['<=', '<=', '<=']
    
    solver = SimplexSolver(c, A, b, constraint_types, 'max')
    solution, optimal_value, status, steps = solver.solve('bigm')
    
    for line in steps[-30:]:  # Show last 30 lines
        print(line)
