"""
Assignment Problem Solver - Hungarian Algorithm

This module implements the Hungarian (Kuhn-Munkres) algorithm for solving
the assignment problem. The implementation follows textbook methodology
(Taha, Hillier & Lieberman) with detailed step-by-step output.

The Hungarian algorithm minimizes the total cost of assigning n workers
to n tasks, where each worker must be assigned exactly one task.
"""

import numpy as np
from copy import deepcopy


class AssignmentSolver:
    """
    Solves the assignment problem using the Hungarian algorithm.
    
    Features:
    - Handles rectangular matrices (auto-padding with dummy rows/cols)
    - Step-by-step solution tracking
    - Textbook-style output formatting
    - Optimal assignment extraction
    """
    
    def __init__(self, cost_matrix):
        """
        Initialize the Assignment Solver.
        
        Args:
            cost_matrix: 2D list or numpy array of assignment costs
                        Rows represent workers, columns represent tasks
        """
        self.original_matrix = np.array(cost_matrix, dtype=float)
        self.num_original_rows = self.original_matrix.shape[0]
        self.num_original_cols = self.original_matrix.shape[1]
        
        # Pad to square matrix if needed
        self.matrix = self._pad_matrix(self.original_matrix)
        self.n = self.matrix.shape[0]
        
        # Solution tracking
        self.steps = []
        self.assignments = []
        self.total_cost = 0
        self.is_solved = False
        
    def _pad_matrix(self, matrix):
        """
        Pad matrix to square if rectangular.
        
        Args:
            matrix: Input cost matrix
            
        Returns:
            Square numpy array
        """
        rows, cols = matrix.shape
        if rows == cols:
            return matrix.copy()
        
        size = max(rows, cols)
        padded = np.zeros((size, size))
        padded[:rows, :cols] = matrix
        
        # Add large cost for dummy assignments
        if rows < cols:
            padded[rows:, :] = 0  # Dummy workers
        else:
            padded[:, cols:] = 0  # Dummy tasks
        
        return padded
    
    def solve(self):
        """
        Solve the assignment problem using Hungarian algorithm.
        
        Returns:
            Tuple of (assignments, total_cost, steps)
            - assignments: List of (worker, task) tuples
            - total_cost: Minimum total cost
            - steps: List of solution step descriptions
        """
        self.steps = []
        self.steps.append("=" * 70)
        self.steps.append("ASSIGNMENT PROBLEM - HUNGARIAN ALGORITHM")
        self.steps.append("=" * 70)
        self.steps.append("")
        
        # Display original problem
        self._add_step("Original Cost Matrix:", self.original_matrix)
        
        if self.n > self.original_matrix.shape[0] or self.n > self.original_matrix.shape[1]:
            self.steps.append("\nNote: Matrix padded to square with dummy assignments (cost = 0)")
            self._add_step("Padded Cost Matrix:", self.matrix)
        
        # Step 1: Row Reduction
        self.steps.append("\n" + "-" * 70)
        self.steps.append("STEP 1: ROW REDUCTION")
        self.steps.append("-" * 70)
        reduced_matrix = self._row_reduction()
        
        # Step 2: Column Reduction
        self.steps.append("\n" + "-" * 70)
        self.steps.append("STEP 2: COLUMN REDUCTION")
        self.steps.append("-" * 70)
        reduced_matrix = self._column_reduction(reduced_matrix)
        
        # Step 3-5: Covering and Assignment
        self.steps.append("\n" + "-" * 70)
        self.steps.append("STEP 3-5: ZERO COVERING AND OPTIMAL ASSIGNMENT")
        self.steps.append("-" * 70)
        final_matrix = self._cover_zeros_and_optimize(reduced_matrix)
        
        # Extract final assignments
        self._extract_assignments(final_matrix)
        
        # Calculate total cost
        self._calculate_total_cost()
        
        # Summary
        self._add_summary()
        
        self.is_solved = True
        return self.assignments, self.total_cost, self.steps
    
    def _row_reduction(self):
        """
        Subtract minimum value from each row.
        
        Returns:
            Row-reduced matrix
        """
        matrix = self.matrix.copy()
        
        self.steps.append("\nSubtract the minimum value in each row from all elements in that row:")
        self.steps.append("")
        
        for i in range(self.n):
            min_val = np.min(matrix[i, :])
            self.steps.append(f"Row {i}: min = {min_val:.2f}")
            matrix[i, :] -= min_val
        
        self.steps.append("")
        self._add_step("Matrix after row reduction:", matrix)
        
        return matrix
    
    def _column_reduction(self, matrix):
        """
        Subtract minimum value from each column.
        
        Args:
            matrix: Input matrix
            
        Returns:
            Column-reduced matrix
        """
        result = matrix.copy()
        
        self.steps.append("\nSubtract the minimum value in each column from all elements in that column:")
        self.steps.append("")
        
        for j in range(self.n):
            min_val = np.min(result[:, j])
            self.steps.append(f"Column {j}: min = {min_val:.2f}")
            result[:, j] -= min_val
        
        self.steps.append("")
        self._add_step("Matrix after column reduction:", result)
        
        return result
    
    def _cover_zeros_and_optimize(self, matrix):
        """
        Perform covering and optimization iterations until optimal solution is found.
        
        Args:
            matrix: Reduced cost matrix
            
        Returns:
            Final optimized matrix
        """
        current_matrix = matrix.copy()
        iteration = 0
        
        while True:
            iteration += 1
            self.steps.append(f"\nIteration {iteration}:")
            self.steps.append("")
            
            # Find zero positions
            zeros = np.argwhere(current_matrix == 0)
            
            # Try to make assignments
            assignments = self._greedy_assignment(current_matrix)
            
            if len(assignments) == self.n:
                self.steps.append(f"✓ Found {self.n} independent assignments!")
                self.steps.append("")
                self._display_assignments(assignments, current_matrix)
                return current_matrix
            
            # Need to cover zeros and create more
            self.steps.append(f"Only {len(assignments)}/{self.n} assignments possible.")
            self.steps.append("Covering zeros and creating additional zeros...")
            self.steps.append("")
            
            # Find minimum covering lines
            row_cover, col_cover = self._find_minimum_cover(current_matrix)
            
            self.steps.append("Minimum line cover:")
            if any(row_cover):
                covered_rows = [i for i in range(self.n) if row_cover[i]]
                self.steps.append(f"  Covered rows: {covered_rows}")
            if any(col_cover):
                covered_cols = [j for j in range(self.n) if col_cover[j]]
                self.steps.append(f"  Covered columns: {covered_cols}")
            self.steps.append("")
            
            # Find smallest uncovered element
            min_uncovered = self._find_min_uncovered(current_matrix, row_cover, col_cover)
            self.steps.append(f"Smallest uncovered element: {min_uncovered:.2f}")
            self.steps.append("")
            
            # Subtract from uncovered, add to double-covered
            current_matrix = self._adjust_matrix(current_matrix, row_cover, col_cover, min_uncovered)
            
            self._add_step("Updated matrix:", current_matrix)
            
            if iteration > 20:  # Safety limit
                self.steps.append("\nWarning: Maximum iterations reached.")
                break
        
        return current_matrix
    
    def _greedy_assignment(self, matrix):
        """
        Try to find maximum number of independent zero assignments using greedy approach.
        
        Args:
            matrix: Cost matrix
            
        Returns:
            List of (row, col) assignments
        """
        assigned_rows = set()
        assigned_cols = set()
        assignments = []
        
        # Count zeros in each row and column
        row_zeros = [np.sum(matrix[i, :] == 0) for i in range(self.n)]
        
        # Sort rows by number of zeros (fewer zeros = higher priority)
        row_order = sorted(range(self.n), key=lambda i: row_zeros[i])
        
        for i in row_order:
            if i in assigned_rows:
                continue
            
            # Find available zeros in this row
            available_cols = [j for j in range(self.n) 
                            if matrix[i, j] == 0 and j not in assigned_cols]
            
            if available_cols:
                # Choose column with fewest zeros
                col_zeros = {j: np.sum(matrix[:, j] == 0) for j in available_cols}
                j = min(available_cols, key=lambda x: col_zeros[x])
                
                assignments.append((i, j))
                assigned_rows.add(i)
                assigned_cols.add(j)
        
        return assignments
    
    def _find_minimum_cover(self, matrix):
        """
        Find minimum number of lines to cover all zeros.
        
        Uses König's theorem approach.
        
        Args:
            matrix: Cost matrix
            
        Returns:
            Tuple of (row_cover, col_cover) boolean arrays
        """
        # Start with no covers
        row_cover = [False] * self.n
        col_cover = [False] * self.n
        
        # Mark rows with no assignments
        assignments = self._greedy_assignment(matrix)
        assigned_rows = {i for i, j in assignments}
        marked_rows = {i for i in range(self.n) if i not in assigned_rows}
        
        # Iteratively mark columns and rows
        changed = True
        while changed:
            changed = False
            
            # Mark columns with zeros in marked rows
            for i in marked_rows:
                for j in range(self.n):
                    if matrix[i, j] == 0 and not col_cover[j]:
                        col_cover[j] = True
                        changed = True
            
            # Mark rows with assignments in marked columns
            for i, j in assignments:
                if col_cover[j] and i not in marked_rows:
                    marked_rows.add(i)
                    changed = True
        
        # Cover unmarked rows and marked columns
        row_cover = [i not in marked_rows for i in range(self.n)]
        
        return row_cover, col_cover
    
    def _find_min_uncovered(self, matrix, row_cover, col_cover):
        """
        Find minimum value among uncovered elements.
        
        Args:
            matrix: Cost matrix
            row_cover: Boolean array indicating covered rows
            col_cover: Boolean array indicating covered columns
            
        Returns:
            Minimum uncovered value
        """
        min_val = float('inf')
        
        for i in range(self.n):
            for j in range(self.n):
                if not row_cover[i] and not col_cover[j]:
                    min_val = min(min_val, matrix[i, j])
        
        return min_val
    
    def _adjust_matrix(self, matrix, row_cover, col_cover, value):
        """
        Adjust matrix by subtracting value from uncovered elements
        and adding to double-covered elements.
        
        Args:
            matrix: Cost matrix
            row_cover: Boolean array indicating covered rows
            col_cover: Boolean array indicating covered columns
            value: Value to subtract/add
            
        Returns:
            Adjusted matrix
        """
        result = matrix.copy()
        
        for i in range(self.n):
            for j in range(self.n):
                if not row_cover[i] and not col_cover[j]:
                    # Uncovered: subtract
                    result[i, j] -= value
                elif row_cover[i] and col_cover[j]:
                    # Double-covered: add
                    result[i, j] += value
        
        return result
    
    def _extract_assignments(self, matrix):
        """
        Extract final optimal assignments from matrix.
        
        Args:
            matrix: Final optimized matrix
        """
        self.assignments = self._greedy_assignment(matrix)
        
        # Filter out dummy assignments
        self.assignments = [(i, j) for i, j in self.assignments
                          if i < self.num_original_rows and j < self.num_original_cols]
    
    def _calculate_total_cost(self):
        """Calculate total cost of assignments."""
        self.total_cost = sum(self.original_matrix[i, j] for i, j in self.assignments)
    
    def _add_step(self, description, matrix):
        """
        Add a matrix display to steps.
        
        Args:
            description: Step description
            matrix: Matrix to display
        """
        self.steps.append(description)
        self.steps.append("")
        
        # Format matrix with row/column labels
        rows, cols = matrix.shape
        
        # Header
        header = "      " + "".join(f"  T{j}  " for j in range(cols))
        self.steps.append(header)
        self.steps.append("    " + "-" * (6 * cols + 2))
        
        # Rows
        for i in range(rows):
            row_str = f"W{i} | "
            row_str += "".join(f" {matrix[i, j]:5.2f}" for j in range(cols))
            self.steps.append(row_str)
        
        self.steps.append("")
    
    def _display_assignments(self, assignments, matrix):
        """
        Display assignment list.
        
        Args:
            assignments: List of (row, col) tuples
            matrix: Current matrix for cost lookup
        """
        self.steps.append("Assignments:")
        for i, j in sorted(assignments):
            cost = self.original_matrix[i, j] if i < self.num_original_rows and j < self.num_original_cols else 0
            self.steps.append(f"  Worker {i} → Task {j} (cost: {cost:.2f})")
    
    def _add_summary(self):
        """Add solution summary."""
        self.steps.append("\n" + "=" * 70)
        self.steps.append("OPTIMAL SOLUTION")
        self.steps.append("=" * 70)
        self.steps.append("")
        
        self.steps.append("Final Assignments:")
        for i, j in sorted(self.assignments):
            cost = self.original_matrix[i, j]
            self.steps.append(f"  Worker {i} → Task {j} : {cost:.2f}")
        
        self.steps.append("")
        self.steps.append(f"Total Minimum Cost: {self.total_cost:.2f}")
        self.steps.append("")
        self.steps.append("=" * 70)
    
    def get_solution_text(self):
        """
        Get formatted solution text.
        
        Returns:
            List of text lines for display
        """
        if not self.is_solved:
            return ["Problem not yet solved. Call solve() first."]
        return self.steps
    
    def get_assignment_matrix(self):
        """
        Get binary assignment matrix (1 = assigned, 0 = not assigned).
        
        Returns:
            2D numpy array
        """
        result = np.zeros((self.num_original_rows, self.num_original_cols))
        for i, j in self.assignments:
            result[i, j] = 1
        return result


def solve_assignment_problem(cost_matrix):
    """
    Convenience function to solve assignment problem.
    
    Args:
        cost_matrix: 2D list or array of costs
        
    Returns:
        Tuple of (assignments, total_cost, solution_steps)
    """
    solver = AssignmentSolver(cost_matrix)
    return solver.solve()


# Example usage and testing
if __name__ == "__main__":
    # Test case 1: Standard 4x4 problem
    print("Test Case 1: Standard 4x4 Assignment Problem")
    print("=" * 70)
    
    cost_matrix_1 = [
        [9, 2, 7, 8],
        [6, 4, 3, 7],
        [5, 8, 1, 8],
        [7, 6, 9, 4]
    ]
    
    solver1 = AssignmentSolver(cost_matrix_1)
    assignments1, cost1, steps1 = solver1.solve()
    
    for line in steps1:
        print(line)
    
    print("\n\n")
    
    # Test case 2: Rectangular 3x4 problem
    print("Test Case 2: Rectangular 3x4 Assignment Problem")
    print("=" * 70)
    
    cost_matrix_2 = [
        [10, 12, 8, 11],
        [5, 10, 7, 8],
        [12, 14, 11, 9]
    ]
    
    solver2 = AssignmentSolver(cost_matrix_2)
    assignments2, cost2, steps2 = solver2.solve()
    
    for line in steps2[-20:]:  # Show last 20 lines
        print(line)
