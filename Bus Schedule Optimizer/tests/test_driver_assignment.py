"""Unit tests for Assignment solver."""

import pytest
import numpy as np
from solvers.driver_assignment_hungarian import AssignmentSolver, solve_assignment_problem


class TestAssignmentSolver:
    """Test cases for the Hungarian algorithm implementation."""
    
    def test_square_4x4_assignment(self):
        """Test standard 4×4 assignment problem."""
        cost_matrix = [
            [9, 2, 7, 8],
            [6, 4, 3, 7],
            [5, 8, 1, 8],
            [7, 6, 9, 4]
        ]
        
        solver = AssignmentSolver(cost_matrix)
        assignments, total_cost, steps = solver.solve()
        
        # Should find optimal assignment
        assert len(assignments) == 4
        assert total_cost == 13.0  # Known optimal for this problem
        assert solver.status == 'optimal' or solver.is_solved
        
        # Each worker assigned to exactly one task
        workers = [i for i, j in assignments]
        tasks = [j for i, j in assignments]
        assert len(set(workers)) == 4
        assert len(set(tasks)) == 4
    
    def test_rectangular_3x4_assignment(self):
        """Test rectangular assignment (more tasks than workers)."""
        cost_matrix = [
            [10, 12, 8, 11],
            [5, 10, 7, 8],
            [12, 14, 11, 9]
        ]
        
        solver = AssignmentSolver(cost_matrix)
        assignments, total_cost, steps = solver.solve()
        
        # Should assign all 3 workers
        assert len(assignments) == 3
        
        # Each worker assigned once
        workers = [i for i, j in assignments]
        assert len(set(workers)) == 3
        
        # Total cost should be positive
        assert total_cost > 0
    
    def test_rectangular_4x3_assignment(self):
        """Test rectangular assignment (more workers than tasks)."""
        cost_matrix = [
            [8, 6, 10],
            [9, 12, 13],
            [14, 9, 16],
            [7, 5, 8]
        ]
        
        solver = AssignmentSolver(cost_matrix)
        assignments, total_cost, steps = solver.solve()
        
        # Should assign only 3 workers
        assert len(assignments) == 3
        
        # Each task assigned once
        tasks = [j for i, j in assignments]
        assert len(set(tasks)) == 3
    
    def test_2x2_simple_assignment(self):
        """Test minimal 2×2 assignment."""
        cost_matrix = [
            [4, 3],
            [2, 5]
        ]
        
        solver = AssignmentSolver(cost_matrix)
        assignments, total_cost, steps = solver.solve()
        
        # Optimal: (0,1) and (1,0) with cost 3+2=5
        assert len(assignments) == 2
        assert total_cost == 5.0
    
    def test_degenerate_zero_costs(self):
        """Test assignment with many zero costs."""
        cost_matrix = [
            [0, 0, 1],
            [0, 1, 0],
            [1, 0, 0]
        ]
        
        solver = AssignmentSolver(cost_matrix)
        assignments, total_cost, steps = solver.solve()
        
        # Should find all-zero assignment
        assert len(assignments) == 3
        assert total_cost == 0.0
    
    def test_large_5x5_assignment(self):
        """Test larger 5×5 problem."""
        cost_matrix = [
            [15, 18, 21, 24, 20],
            [19, 23, 22, 18, 21],
            [26, 17, 16, 19, 20],
            [19, 21, 23, 17, 23],
            [18, 19, 19, 21, 22]
        ]
        
        solver = AssignmentSolver(cost_matrix)
        assignments, total_cost, steps = solver.solve()
        
        assert len(assignments) == 5
        assert total_cost > 0
        
        # Verify assignment is valid
        workers = [i for i, j in assignments]
        tasks = [j for i, j in assignments]
        assert sorted(workers) == list(range(5))
        assert sorted(tasks) == list(range(5))
    
    def test_solution_steps_generated(self):
        """Test that solution steps are properly generated."""
        cost_matrix = [[4, 3], [2, 5]]
        
        solver = AssignmentSolver(cost_matrix)
        assignments, total_cost, steps = solver.solve()
        
        # Should have multiple step descriptions
        assert len(steps) > 10
        
        # Should contain key phrases
        step_text = ' '.join(steps)
        assert 'ROW REDUCTION' in step_text
        assert 'COLUMN REDUCTION' in step_text
        assert 'OPTIMAL SOLUTION' in step_text
    
    def test_get_assignment_matrix(self):
        """Test binary assignment matrix generation."""
        cost_matrix = [[4, 3], [2, 5]]
        
        solver = AssignmentSolver(cost_matrix)
        solver.solve()
        
        assignment_matrix = solver.get_assignment_matrix()
        
        # Should be 2x2
        assert assignment_matrix.shape == (2, 2)
        
        # Each row and column should sum to 1
        assert np.allclose(assignment_matrix.sum(axis=0), 1)
        assert np.allclose(assignment_matrix.sum(axis=1), 1)
    
    def test_convenience_function(self):
        """Test the convenience solve function."""
        cost_matrix = [[9, 2, 7, 8],
                      [6, 4, 3, 7],
                      [5, 8, 1, 8],
                      [7, 6, 9, 4]]
        
        assignments, total_cost, steps = solve_assignment_problem(cost_matrix)
        
        assert len(assignments) == 4
        assert total_cost == 13.0
        assert len(steps) > 0


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
