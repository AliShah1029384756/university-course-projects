"""Example datasets for Assignment problems."""

# Example 1: Standard 4x4 assignment
EXAMPLE_1 = {
    "name": "Worker-Task Assignment (4×4)",
    "description": "Assign 4 workers to 4 tasks minimizing total cost",
    "cost_matrix": [
        [9, 2, 7, 8],
        [6, 4, 3, 7],
        [5, 8, 1, 8],
        [7, 6, 9, 4]
    ],
    "expected_cost": 13.0,
    "expected_assignments": [(0, 1), (1, 2), (2, 2), (3, 3)]  # Example
}

# Example 2: Rectangular 3x4 assignment
EXAMPLE_2 = {
    "name": "Unbalanced Assignment (3×4)",
    "description": "3 workers, 4 tasks - one task unassigned",
    "cost_matrix": [
        [10, 12, 8, 11],
        [5, 10, 7, 8],
        [12, 14, 11, 9]
    ],
    "expected_cost": None  # To be calculated
}

# Example 3: 5x5 assignment
EXAMPLE_3 = {
    "name": "Large Assignment (5×5)",
    "description": "Assign 5 salespeople to 5 territories",
    "cost_matrix": [
        [15, 18, 21, 24, 20],
        [19, 23, 22, 18, 21],
        [26, 17, 16, 19, 20],
        [19, 21, 23, 17, 23],
        [18, 19, 19, 21, 22]
    ],
    "expected_cost": None
}

# Example 4: Small 2x2 assignment
EXAMPLE_4 = {
    "name": "Simple 2×2 Assignment",
    "description": "Minimal assignment problem",
    "cost_matrix": [
        [4, 3],
        [2, 5]
    ],
    "expected_cost": 6.0,
    "expected_assignments": [(0, 1), (1, 0)]
}

ALL_EXAMPLES = [EXAMPLE_1, EXAMPLE_2, EXAMPLE_3, EXAMPLE_4]
