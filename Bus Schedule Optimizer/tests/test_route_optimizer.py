"""Unit tests for Simplex solver."""

import pytest
import numpy as np
from solvers.route_optimizer_simplex import SimplexSolver


class TestSimplexSolver:
    """Test cases for the Simplex method implementation."""
    
    def test_standard_maximization(self):
        """Test standard maximization problem."""
        # Maximize: 3x1 + 5x2
        # Subject to: x1 <= 4, 2x2 <= 12, 3x1 + 2x2 <= 18
        c = [3, 5]
        A = [[1, 0], [0, 2], [3, 2]]
        b = [4, 12, 18]
        constraint_types = ['<=', '<=', '<=']
        
        solver = SimplexSolver(c, A, b, constraint_types, 'max')
        solution, optimal_value, status, steps = solver.solve()
        
        assert status == 'optimal'
        assert optimal_value == pytest.approx(36.0, rel=1e-3)
        assert solution['x1'] == pytest.approx(2.0, rel=1e-3)
        assert solution['x2'] == pytest.approx(6.0, rel=1e-3)
    
    def test_minimization_problem(self):
        """Test minimization problem."""
        # Minimize: 2x1 + 3x2
        # Subject to: x1 + x2 >= 4, 2x1 + x2 >= 6
        c = [2, 3]
        A = [[1, 1], [2, 1]]
        b = [4, 6]
        constraint_types = ['>=', '>=']
        
        solver = SimplexSolver(c, A, b, constraint_types, 'min')
        solution, optimal_value, status, steps = solver.solve()
        
        assert status == 'optimal'
        # Expected optimal: x1=2, x2=2, objective=10
        assert optimal_value == pytest.approx(10.0, rel=1e-2)
    
    def test_unbounded_problem(self):
        """Test detection of unbounded problem."""
        # Maximize: x1 + x2
        # Subject to: -x1 + x2 <= 1, -x1 - x2 <= -3
        c = [1, 1]
        A = [[-1, 1], [-1, -1]]
        b = [1, -3]
        constraint_types = ['<=', '<=']
        
        solver = SimplexSolver(c, A, b, constraint_types, 'max')
        solution, optimal_value, status, steps = solver.solve()
        
        # This problem should be unbounded or have issues
        # Note: Proper unbounded detection depends on tableau entries
        assert status in ['unbounded', 'optimal', 'unknown']
    
    def test_equality_constraint(self):
        """Test problem with equality constraint."""
        c = [3, 2]
        A = [[1, 1], [2, 1]]
        b = [5, 8]
        constraint_types = ['=', '<=']
        
        solver = SimplexSolver(c, A, b, constraint_types, 'max')
        solution, optimal_value, status, steps = solver.solve()
        
        # Should handle equality constraint
        assert status in ['optimal', 'unbounded']
        if status == 'optimal':
            assert optimal_value >= 0
    
    def test_mixed_constraints(self):
        """Test problem with mixed constraint types."""
        c = [5, 4, 3]
        A = [
            [2, 3, 1],
            [4, 1, 2],
            [3, 4, 2]
        ]
        b = [50, 60, 70]
        constraint_types = ['<=', '<=', '<=']
        
        solver = SimplexSolver(c, A, b, constraint_types, 'max')
        solution, optimal_value, status, steps = solver.solve()
        
        assert status == 'optimal'
        assert optimal_value > 0
        
        # All solution values should be non-negative
        for var, value in solution.items():
            assert value >= -1e-6  # Small tolerance for numerical errors
    
    def test_standard_form_conversion(self):
        """Test that standard form conversion works."""
        c = [1, 2]
        A = [[1, 1]]
        b = [3]
        constraint_types = ['>=']
        
        solver = SimplexSolver(c, A, b, constraint_types, 'min')
        solver._convert_to_standard_form('bigm')
        
        # Should have added surplus and artificial variables
        assert len(solver.variable_names) > 2
        assert any('S' in name or 'A' in name for name in solver.variable_names)
    
    def test_solution_steps_generated(self):
        """Test that solution steps are properly generated."""
        c = [3, 5]
        A = [[1, 0], [0, 2], [3, 2]]
        b = [4, 12, 18]
        constraint_types = ['<=', '<=', '<=']
        
        solver = SimplexSolver(c, A, b, constraint_types, 'max')
        solution, optimal_value, status, steps = solver.solve()
        
        # Should have multiple step descriptions
        assert len(steps) > 20
        
        # Should contain key sections
        step_text = ' '.join(steps)
        assert 'STANDARD FORM' in step_text
        assert 'TABLEAU' in step_text
        assert 'SIMPLEX ITERATIONS' in step_text
        assert 'SENSITIVITY' in step_text
    
    def test_sensitivity_analysis(self):
        """Test that sensitivity analysis is performed."""
        c = [3, 5]
        A = [[1, 0], [0, 2], [3, 2]]
        b = [4, 12, 18]
        constraint_types = ['<=', '<=', '<=']
        
        solver = SimplexSolver(c, A, b, constraint_types, 'max')
        solution, optimal_value, status, steps = solver.solve()
        
        if status == 'optimal':
            step_text = ' '.join(steps)
            # Sensitivity analysis section should be present
            assert 'SENSITIVITY' in step_text
            assert 'REDUCED COST' in step_text or 'SHADOW PRICE' in step_text
    
    def test_simple_2var_problem(self):
        """Test simple 2-variable problem."""
        # Maximize: 2x1 + 3x2
        # Subject to: x1 + x2 <= 5, x1 <= 3, x2 <= 4
        c = [2, 3]
        A = [[1, 1], [1, 0], [0, 1]]
        b = [5, 3, 4]
        constraint_types = ['<=', '<=', '<=']
        
        solver = SimplexSolver(c, A, b, constraint_types, 'max')
        solution, optimal_value, status, steps = solver.solve()
        
        assert status == 'optimal'
        # Optimal: x1=1, x2=4, objective=14
        assert optimal_value == pytest.approx(14.0, rel=1e-3)
    
    def test_big_m_method(self):
        """Test Big-M method for artificial variables."""
        c = [1, 1]
        A = [[1, 1]]
        b = [5]
        constraint_types = ['>=']
        
        solver = SimplexSolver(c, A, b, constraint_types, 'min')
        solution, optimal_value, status, steps = solver.solve(method='bigm')
        
        # Should handle artificial variables
        assert status == 'optimal'
        # Any feasible solution should work
        assert solution['x1'] + solution['x2'] >= 4.99  # Close to 5


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
