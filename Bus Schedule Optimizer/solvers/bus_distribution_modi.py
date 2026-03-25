"""
Transportation Problem Solver

This module implements methods for solving the transportation problem:
- Initial feasible solution: North-West Corner, Least-Cost
- Optimization: MODI (Modified Distribution) method with u-v variables

Implementation follows textbook methodology (Taha, Hillier & Lieberman).
"""

import numpy as np
from copy import deepcopy


class TransportationSolver:
    """
    Solves transportation problems using NW Corner, Least-Cost, and MODI methods.
    
    The transportation problem finds the minimum-cost way to ship goods from
    multiple sources (supply points) to multiple destinations (demand points).
    """
    
    def __init__(self, costs, supply, demand):
        """
        Initialize the Transportation Solver.
        
        Args:
            costs: 2D list/array of transportation costs (sources × destinations)
            supply: List of supply quantities at each source
            demand: List of demand quantities at each destination
        """
        self.costs = np.array(costs, dtype=float)
        self.original_supply = np.array(supply, dtype=float)
        self.original_demand = np.array(demand, dtype=float)
        
        self.num_sources = len(supply)
        self.num_destinations = len(demand)
        
        # Balance supply and demand if needed
        self.supply, self.demand, self.costs = self._balance_problem()
        
        # Solution tracking
        self.allocation = np.zeros_like(self.costs)
        self.steps = []
        self.is_solved = False
        self.total_cost = 0
        
    def _balance_problem(self):
        """
        Balance transportation problem by adding dummy source or destination.
        
        Returns:
            Tuple of (balanced_supply, balanced_demand, balanced_costs)
        """
        total_supply = np.sum(self.original_supply)
        total_demand = np.sum(self.original_demand)
        
        supply = self.original_supply.copy()
        demand = self.original_demand.copy()
        costs = self.costs.copy()
        
        if total_supply < total_demand:
            # Add dummy source with zero cost
            shortage = total_demand - total_supply
            supply = np.append(supply, shortage)
            dummy_row = np.zeros((1, self.num_destinations))
            costs = np.vstack([costs, dummy_row])
            self.num_sources += 1
            
            self.steps.append(f"Note: Total demand ({total_demand}) > Total supply ({total_supply})")
            self.steps.append(f"Added dummy source with supply {shortage} and zero costs")
            self.steps.append("")
            
        elif total_demand < total_supply:
            # Add dummy destination with zero cost
            surplus = total_supply - total_demand
            demand = np.append(demand, surplus)
            dummy_col = np.zeros((self.num_sources, 1))
            costs = np.hstack([costs, dummy_col])
            self.num_destinations += 1
            
            self.steps.append(f"Note: Total supply ({total_supply}) > Total demand ({total_demand})")
            self.steps.append(f"Added dummy destination with demand {surplus} and zero costs")
            self.steps.append("")
        
        return supply, demand, costs
    
    def solve(self, initial_method='northwest', optimize=True):
        """
        Solve the transportation problem.
        
        Args:
            initial_method: 'northwest' or 'least_cost' for initial solution
            optimize: Whether to optimize using MODI method
            
        Returns:
            Tuple of (allocation_matrix, total_cost, solution_steps)
        """
        self.steps = []
        self.steps.append("=" * 80)
        self.steps.append("TRANSPORTATION PROBLEM")
        self.steps.append("=" * 80)
        self.steps.append("")
        
        # Display problem
        self._display_problem()
        
        # Get initial feasible solution
        if initial_method.lower() == 'northwest':
            self.steps.append("\n" + "=" * 80)
            self.steps.append("INITIAL SOLUTION: NORTH-WEST CORNER METHOD")
            self.steps.append("=" * 80)
            self.steps.append("")
            self.allocation = self._northwest_corner()
        else:
            self.steps.append("\n" + "=" * 80)
            self.steps.append("INITIAL SOLUTION: LEAST-COST METHOD")
            self.steps.append("=" * 80)
            self.steps.append("")
            self.allocation = self._least_cost()
        
        # Display initial solution
        self._display_allocation("Initial Feasible Solution")
        initial_cost = self._calculate_cost()
        self.steps.append(f"Initial Total Cost: {initial_cost:.2f}")
        self.steps.append("")
        
        # Optimize using MODI if requested
        if optimize:
            self.steps.append("\n" + "=" * 80)
            self.steps.append("OPTIMIZATION: MODI METHOD (u-v Method)")
            self.steps.append("=" * 80)
            self.steps.append("")
            self.allocation = self._modi_optimization()
        
        # Final solution
        self.total_cost = self._calculate_cost()
        self._add_summary()
        
        self.is_solved = True
        return self.allocation, self.total_cost, self.steps
    
    def _northwest_corner(self):
        """
        Find initial feasible solution using North-West Corner method.
        
        Returns:
            Allocation matrix
        """
        allocation = np.zeros_like(self.costs)
        supply = self.supply.copy()
        demand = self.demand.copy()
        
        i, j = 0, 0  # Start at northwest corner
        
        self.steps.append("Starting at northwest corner (top-left cell):")
        self.steps.append("")
        
        step = 1
        while i < self.num_sources and j < self.num_destinations:
            # Allocate minimum of supply and demand
            allocated = min(supply[i], demand[j])
            allocation[i, j] = allocated
            
            self.steps.append(f"Step {step}: Allocate {allocated:.0f} to cell ({i}, {j})")
            self.steps.append(f"  Supply[{i}] = {supply[i]:.0f}, Demand[{j}] = {demand[j]:.0f}")
            
            supply[i] -= allocated
            demand[j] -= allocated
            
            # Move to next cell
            if supply[i] == 0 and demand[j] == 0:
                # Both exhausted - move diagonally (but assign to one direction)
                self.steps.append(f"  Both supply and demand exhausted. Moving to ({i+1}, {j+1})")
                i += 1
                j += 1
            elif supply[i] == 0:
                # Supply exhausted - move down
                self.steps.append(f"  Supply exhausted. Moving to next source ({i+1}, {j})")
                i += 1
            else:
                # Demand exhausted - move right
                self.steps.append(f"  Demand exhausted. Moving to next destination ({i}, {j+1})")
                j += 1
            
            self.steps.append("")
            step += 1
        
        return allocation
    
    def _least_cost(self):
        """
        Find initial feasible solution using Least-Cost method.
        
        Returns:
            Allocation matrix
        """
        allocation = np.zeros_like(self.costs)
        supply = self.supply.copy()
        demand = self.demand.copy()
        
        # Create mask for available cells
        available = np.ones_like(self.costs, dtype=bool)
        
        self.steps.append("Allocating to cells with minimum cost iteratively:")
        self.steps.append("")
        
        step = 1
        while np.any(supply > 0) and np.any(demand > 0):
            # Find cell with minimum cost among available cells
            masked_costs = np.where(available, self.costs, np.inf)
            min_cost = np.min(masked_costs)
            
            # Get position of minimum (first occurrence if multiple)
            i, j = np.unravel_index(np.argmin(masked_costs), masked_costs.shape)
            
            # Allocate minimum of supply and demand
            allocated = min(supply[i], demand[j])
            allocation[i, j] = allocated
            
            self.steps.append(f"Step {step}: Minimum cost = {min_cost:.2f} at cell ({i}, {j})")
            self.steps.append(f"  Allocate {allocated:.0f} units")
            self.steps.append(f"  Supply[{i}] = {supply[i]:.0f}, Demand[{j}] = {demand[j]:.0f}")
            
            supply[i] -= allocated
            demand[j] -= allocated
            
            # Mark row or column as unavailable if exhausted
            if supply[i] == 0:
                available[i, :] = False
                self.steps.append(f"  Source {i} supply exhausted")
            if demand[j] == 0:
                available[:, j] = False
                self.steps.append(f"  Destination {j} demand exhausted")
            
            self.steps.append("")
            step += 1
        
        return allocation
    
    def _modi_optimization(self):
        """
        Optimize solution using MODI (Modified Distribution) method.
        
        Returns:
            Optimized allocation matrix
        """
        allocation = self.allocation.copy()
        iteration = 0
        
        while True:
            iteration += 1
            self.steps.append("")
            self.steps.append("")
            self.steps.append(f"MODI Iteration {iteration}:")
            self.steps.append("-" * 80)
            self.steps.append("")
            
            # Calculate u and v values
            u, v = self._calculate_uv(allocation)
            
            self.steps.append("")
            self.steps.append("Dual variables (u and v):")
            u_str = "  u values: " + ", ".join([f"u{i}={u[i]:.2f}" for i in range(len(u))])
            v_str = "  v values: " + ", ".join([f"v{j}={v[j]:.2f}" for j in range(len(v))])
            self.steps.append(u_str)
            self.steps.append(v_str)
            self.steps.append("")
            
            # Calculate opportunity costs (delta values) for non-basic cells
            deltas = self._calculate_deltas(allocation, u, v)
            
            # Display delta table
            self._display_deltas(deltas, allocation)
            
            # Check for optimality
            min_delta = np.min(deltas[allocation == 0])
            
            if min_delta >= -1e-6:  # Account for floating point errors
                self.steps.append("")
                self.steps.append("✓ All opportunity costs are non-negative.")
                self.steps.append("Current solution is OPTIMAL!")
                self.steps.append("")
                break
            
            # Find entering cell (most negative delta)
            entering_i, entering_j = self._find_entering_cell(deltas, allocation)
            self.steps.append("")
            self.steps.append(f"Entering cell: ({entering_i}, {entering_j}) with Δ = {deltas[entering_i, entering_j]:.2f}")
            self.steps.append("")
            
            # Find loop and perform reallocation
            allocation = self._reallocate(allocation, entering_i, entering_j)
            
            # Display new allocation
            self._display_allocation(f"Allocation after iteration {iteration}")
            current_cost = self._calculate_cost(allocation)
            self.steps.append(f"Total Cost: {current_cost:.2f}")
            self.steps.append("")
            self.steps.append("")
            
            if iteration > 50:  # Safety limit
                self.steps.append("\nWarning: Maximum iterations reached.")
                break
        
        return allocation
    
    def _calculate_uv(self, allocation):
        """
        Calculate u and v dual variables for basic cells.
        
        Uses equation: u[i] + v[j] = c[i][j] for all basic cells
        
        Args:
            allocation: Current allocation matrix
            
        Returns:
            Tuple of (u_values, v_values)
        """
        u = np.full(self.num_sources, np.nan)
        v = np.full(self.num_destinations, np.nan)
        
        # Set u[0] = 0 as starting point
        u[0] = 0
        
        # Iteratively solve for u and v
        max_iterations = 100
        for _ in range(max_iterations):
            updated = False
            
            # For each basic cell (allocated cell)
            for i in range(self.num_sources):
                for j in range(self.num_destinations):
                    if allocation[i, j] > 0:  # Basic cell
                        # u[i] + v[j] = c[i][j]
                        if not np.isnan(u[i]) and np.isnan(v[j]):
                            v[j] = self.costs[i, j] - u[i]
                            updated = True
                        elif np.isnan(u[i]) and not np.isnan(v[j]):
                            u[i] = self.costs[i, j] - v[j]
                            updated = True
            
            # Check if all values computed
            if not np.any(np.isnan(u)) and not np.any(np.isnan(v)):
                break
            
            if not updated:
                # Handle degenerate case - set remaining to 0
                if np.any(np.isnan(u)):
                    u[np.isnan(u)] = 0
                if np.any(np.isnan(v)):
                    v[np.isnan(v)] = 0
                break
        
        return u, v
    
    def _calculate_deltas(self, allocation, u, v):
        """
        Calculate opportunity costs (Δ = c[i][j] - u[i] - v[j]) for non-basic cells.
        
        Args:
            allocation: Current allocation matrix
            u: u dual variables
            v: v dual variables
            
        Returns:
            Matrix of delta values
        """
        deltas = np.zeros_like(self.costs)
        
        for i in range(self.num_sources):
            for j in range(self.num_destinations):
                deltas[i, j] = self.costs[i, j] - u[i] - v[j]
        
        return deltas
    
    def _find_entering_cell(self, deltas, allocation):
        """
        Find entering cell (most negative delta among non-basic cells).
        
        Args:
            deltas: Opportunity cost matrix
            allocation: Current allocation matrix
            
        Returns:
            Tuple of (row, col) for entering cell
        """
        # Mask basic cells
        masked_deltas = np.where(allocation == 0, deltas, np.inf)
        min_delta = np.min(masked_deltas)
        i, j = np.unravel_index(np.argmin(masked_deltas), masked_deltas.shape)
        return i, j
    
    def _reallocate(self, allocation, entering_i, entering_j):
        """
        Perform reallocation along loop starting from entering cell.
        
        Args:
            allocation: Current allocation matrix
            entering_i, entering_j: Entering cell coordinates
            
        Returns:
            Updated allocation matrix
        """
        # Find loop
        loop = self._find_loop(allocation, entering_i, entering_j)
        
        loop_str = " → ".join([f"({i},{j})" for i, j in loop])
        self.steps.append("")
        self.steps.append(f"Reallocation loop: {loop_str}")
        self.steps.append("")
        
        # Find minimum allocation at negative positions (odd indices)
        theta = min(allocation[loop[i]] for i in range(1, len(loop), 2))
        self.steps.append(f"Minimum allocation at '-' positions: θ = {theta:.2f}")
        self.steps.append("")
        
        # Update allocations along loop
        new_allocation = allocation.copy()
        for idx, (i, j) in enumerate(loop):
            if idx % 2 == 0:  # Positive position
                new_allocation[i, j] += theta
            else:  # Negative position
                new_allocation[i, j] -= theta
        
        self.steps.append("Adjustments:")
        for idx, (i, j) in enumerate(loop):
            sign = '+' if idx % 2 == 0 else '-'
            self.steps.append(f"  Cell ({i}, {j}): {sign}{theta:.2f}")
        self.steps.append("")
        
        return new_allocation
    
    def _find_loop(self, allocation, start_i, start_j):
        """
        Find closed loop starting from a non-basic cell.
        
        Uses depth-first search to find path through basic cells.
        
        Args:
            allocation: Current allocation matrix
            start_i, start_j: Starting cell coordinates
            
        Returns:
            List of (row, col) tuples forming the loop
        """
        # This is a simplified loop finder
        # In practice, use graph algorithms for robustness
        
        loop = [(start_i, start_j)]
        visited = {(start_i, start_j)}
        
        # Try to build loop: horizontal -> vertical -> horizontal -> ...
        current_i, current_j = start_i, start_j
        direction = 'horizontal'  # Start by moving horizontally
        
        while len(loop) < 20:  # Safety limit
            if direction == 'horizontal':
                # Find basic cell in same row
                found = False
                for j in range(self.num_destinations):
                    if j != current_j and allocation[current_i, j] > 0:
                        if (current_i, j) not in visited or (current_i, j) == (start_i, start_j) and len(loop) > 2:
                            loop.append((current_i, j))
                            visited.add((current_i, j))
                            current_j = j
                            direction = 'vertical'
                            found = True
                            break
                
                if not found and len(loop) > 2:
                    break
                elif not found:
                    # Try vertical from start
                    direction = 'vertical'
                    continue
                    
            else:  # vertical
                # Find basic cell in same column
                found = False
                for i in range(self.num_sources):
                    if i != current_i and allocation[i, current_j] > 0:
                        if (i, current_j) not in visited or (i, current_j) == (start_i, start_j) and len(loop) > 2:
                            # Check if this closes the loop
                            if (i, current_j) == (start_i, start_j) and len(loop) >= 4:
                                return loop
                            
                            loop.append((i, current_j))
                            visited.add((i, current_j))
                            current_i = i
                            direction = 'horizontal'
                            found = True
                            break
                
                if not found and len(loop) > 2:
                    break
        
        return loop
    
    def _calculate_cost(self, allocation=None):
        """
        Calculate total transportation cost.
        
        Args:
            allocation: Allocation matrix (uses self.allocation if None)
            
        Returns:
            Total cost
        """
        if allocation is None:
            allocation = self.allocation
        
        return np.sum(allocation * self.costs)
    
    def _display_problem(self):
        """Display the transportation problem setup."""
        self.steps.append("Cost Matrix:")
        self.steps.append("")
        
        # Header
        header = "        " + "".join(f"  D{j}  " for j in range(self.num_destinations)) + " | Supply"
        self.steps.append(header)
        self.steps.append("    " + "-" * len(header))
        
        # Rows
        for i in range(self.num_sources):
            row_str = f"S{i}   | "
            row_str += "".join(f" {self.costs[i, j]:5.1f}" for j in range(self.num_destinations))
            row_str += f" | {self.supply[i]:6.1f}"
            self.steps.append(row_str)
        
        # Demand row
        demand_str = "Demand| "
        demand_str += "".join(f" {self.demand[j]:5.1f}" for j in range(self.num_destinations))
        self.steps.append("    " + "-" * len(header))
        self.steps.append(demand_str)
        self.steps.append("")
    
    def _display_allocation(self, title):
        """
        Display current allocation.
        
        Args:
            title: Title for the allocation table
        """
        self.steps.append(f"\n{title}:")
        self.steps.append("")
        
        # Header
        header = "        " + "".join(f"  D{j}  " for j in range(self.num_destinations)) + " | Supply"
        self.steps.append(header)
        self.steps.append("    " + "-" * len(header))
        
        # Rows
        for i in range(self.num_sources):
            row_str = f"S{i}   | "
            row_str += "".join(f" {self.allocation[i, j]:5.1f}" for j in range(self.num_destinations))
            remaining_supply = self.supply[i] - np.sum(self.allocation[i, :])
            row_str += f" | {remaining_supply:6.1f}"
            self.steps.append(row_str)
        
        # Demand row
        demand_str = "Demand| "
        for j in range(self.num_destinations):
            remaining_demand = self.demand[j] - np.sum(self.allocation[:, j])
            demand_str += f" {remaining_demand:5.1f}"
        self.steps.append("    " + "-" * len(header))
        self.steps.append(demand_str)
        self.steps.append("")
    
    def _display_deltas(self, deltas, allocation):
        """
        Display opportunity costs for non-basic cells.
        
        Args:
            deltas: Opportunity cost matrix
            allocation: Current allocation (to identify non-basic cells)
        """
        self.steps.append("")
        self.steps.append("Opportunity Costs (Δ = c[i][j] - u[i] - v[j]) for non-basic cells:")
        self.steps.append("")
        
        has_negative = False
        for i in range(self.num_sources):
            for j in range(self.num_destinations):
                if allocation[i, j] == 0:  # Non-basic cell
                    delta_str = f"  Δ[{i}][{j}] = {deltas[i, j]:7.2f}"
                    if deltas[i, j] < -1e-6:
                        delta_str += "  ← Negative (can improve)"
                        has_negative = True
                    self.steps.append(delta_str)
        
        self.steps.append("")
    
    def _add_summary(self):
        """Add final solution summary."""
        self.steps.append("\n" + "=" * 80)
        self.steps.append("OPTIMAL SOLUTION")
        self.steps.append("=" * 80)
        self.steps.append("")
        
        self.steps.append("Final Shipments:")
        for i in range(self.num_sources):
            for j in range(self.num_destinations):
                if self.allocation[i, j] > 0:
                    cost = self.allocation[i, j] * self.costs[i, j]
                    self.steps.append(f"  S{i} → D{j}: {self.allocation[i, j]:.1f} units @ {self.costs[i, j]:.2f} = {cost:.2f}")
        
        self.steps.append("")
        self.steps.append(f"Total Minimum Transportation Cost: {self.total_cost:.2f}")
        self.steps.append("")
        self.steps.append("=" * 80)
    
    def get_solution_text(self):
        """Get formatted solution text."""
        if not self.is_solved:
            return ["Problem not yet solved. Call solve() first."]
        return self.steps


# Example usage
if __name__ == "__main__":
    print("Test Case: 3×3 Transportation Problem")
    print("=" * 80)
    
    costs = [
        [8, 6, 10],
        [9, 12, 13],
        [14, 9, 16]
    ]
    supply = [20, 30, 25]
    demand = [15, 25, 35]
    
    solver = TransportationSolver(costs, supply, demand)
    allocation, total_cost, steps = solver.solve(initial_method='least_cost', optimize=True)
    
    for line in steps:
        print(line)
