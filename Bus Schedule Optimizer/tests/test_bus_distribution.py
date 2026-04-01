"""Unit tests for Transportation solver."""

import pytest
import numpy as np
from solvers.bus_distribution_modi import TransportationSolver


class TestTransportationSolver:
    """Test cases for Transportation problem solver."""
    
    def test_balanced_3x3_problem(self):
        """Test standard balanced transportation problem."""
        costs = [
            [8, 6, 10],
            [9, 12, 13],
            [14, 9, 16]
        ]
        supply = [20, 30, 25]
        demand = [15, 25, 35]
        
        solver = TransportationSolver(costs, supply, demand)
        allocation, total_cost, steps = solver.solve(initial_method='northwest', optimize=True)
        
        # Check supply constraints
        for i in range(len(supply)):
            assert np.isclose(allocation[i, :].sum(), supply[i])
        
        # Check demand constraints
        for j in range(len(demand)):
            assert np.isclose(allocation[:, j].sum(), demand[j])
        
        # Cost should be positive
        assert total_cost > 0
        assert solver.is_solved
    
    def test_northwest_corner_method(self):
        """Test North-West Corner initial solution."""
        costs = [[2, 3, 1], [5, 4, 8]]
        supply = [15, 25]
        demand = [10, 20, 10]
        
        solver = TransportationSolver(costs, supply, demand)
        allocation, total_cost, steps = solver.solve(initial_method='northwest', optimize=False)
        
        # Should produce valid allocation
        assert np.isclose(allocation.sum(), sum(supply))
        assert np.isclose(allocation.sum(), sum(demand))
    
    def test_least_cost_method(self):
        """Test Least-Cost initial solution."""
        costs = [[2, 3, 1], [5, 4, 8]]
        supply = [15, 25]
        demand = [10, 20, 10]
        
        solver = TransportationSolver(costs, supply, demand)
        allocation, total_cost, steps = solver.solve(initial_method='least_cost', optimize=False)
        
        # Should produce valid allocation
        assert np.isclose(allocation.sum(), sum(supply))
        assert np.isclose(allocation.sum(), sum(demand))
    
    def test_modi_optimization(self):
        """Test MODI optimization improves solution."""
        costs = [
            [8, 6, 10],
            [9, 12, 13],
            [14, 9, 16]
        ]
        supply = [20, 30, 25]
        demand = [15, 25, 35]
        
        solver = TransportationSolver(costs, supply, demand)
        
        # Get initial solution cost
        _, initial_cost, _ = solver.solve(initial_method='northwest', optimize=False)
        
        # Optimize
        solver2 = TransportationSolver(costs, supply, demand)
        _, optimized_cost, _ = solver2.solve(initial_method='northwest', optimize=True)
        
        # Optimized should be <= initial
        assert optimized_cost <= initial_cost
    
    def test_unbalanced_excess_supply(self):
        """Test problem with excess supply (needs dummy destination)."""
        costs = [[2, 3], [5, 4]]
        supply = [15, 25]  # Total: 40
        demand = [10, 20]  # Total: 30
        
        solver = TransportationSolver(costs, supply, demand)
        allocation, total_cost, steps = solver.solve()
        
        # Dummy destination should be added
        assert allocation.shape[1] == 3  # Original 2 + 1 dummy
        
        # Total allocation should equal demand (with dummy)
        assert np.isclose(allocation.sum(), sum(supply))
    
    def test_unbalanced_excess_demand(self):
        """Test problem with excess demand (needs dummy source)."""
        costs = [[2, 3], [5, 4]]
        supply = [15, 20]  # Total: 35
        demand = [10, 20, 15]  # Total: 45
        
        solver = TransportationSolver(costs, supply, demand)
        allocation, total_cost, steps = solver.solve()
        
        # Dummy source should be added
        assert allocation.shape[0] == 3  # Original 2 + 1 dummy
        
        # Total allocation should equal supply (with dummy)
        assert np.isclose(allocation.sum(), sum(demand))
    
    def test_small_2x2_problem(self):
        """Test minimal 2×2 transportation problem."""
        costs = [[1, 2], [3, 1]]
        supply = [10, 20]
        demand = [15, 15]
        
        solver = TransportationSolver(costs, supply, demand)
        allocation, total_cost, steps = solver.solve(optimize=True)
        
        # Should find optimal allocation
        assert allocation.shape == (2, 2)
        assert total_cost > 0
    
    def test_solution_steps_generated(self):
        """Test that solution steps are properly generated."""
        costs = [[8, 6, 10], [9, 12, 13], [14, 9, 16]]
        supply = [20, 30, 25]
        demand = [15, 25, 35]
        
        solver = TransportationSolver(costs, supply, demand)
        _, _, steps = solver.solve(initial_method='least_cost', optimize=True)
        
        # Should have detailed steps
        assert len(steps) > 20
        
        # Should contain key sections
        step_text = ' '.join(steps)
        assert 'INITIAL SOLUTION' in step_text
        assert 'MODI' in step_text or 'OPTIMIZATION' in step_text
        assert 'OPTIMAL SOLUTION' in step_text
    
    def test_cost_calculation(self):
        """Test cost calculation is correct."""
        costs = [[1, 2], [3, 4]]
        supply = [10, 20]
        demand = [15, 15]
        
        solver = TransportationSolver(costs, supply, demand)
        allocation, total_cost, _ = solver.solve(optimize=False)
        
        # Manually calculate cost
        expected_cost = np.sum(allocation * solver.costs)
        assert np.isclose(total_cost, expected_cost)
    
    def test_4x3_problem(self):
        """Test larger 4×3 transportation problem."""
        costs = [
            [10, 15, 25],
            [20, 10, 30],
            [15, 20, 10],
            [25, 30, 15]
        ]
        supply = [30, 40, 35, 25]
        demand = [40, 50, 40]
        
        solver = TransportationSolver(costs, supply, demand)
        allocation, total_cost, steps = solver.solve(optimize=True)
        
        # Verify constraints
        assert allocation.shape == (4, 3)
        for i in range(4):
            assert np.isclose(allocation[i, :].sum(), supply[i])
        for j in range(3):
            assert np.isclose(allocation[:, j].sum(), demand[j])


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
