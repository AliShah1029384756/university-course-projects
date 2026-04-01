"""Example datasets for Simplex problems."""

# Example 1: Standard maximization problem
EXAMPLE_1 = {
    "name": "Product Mix Problem",
    "description": "Maximize profit from two products with resource constraints",
    "c": [3, 5],
    "A": [
        [1, 0],
        [0, 2],
        [3, 2]
    ],
    "b": [4, 12, 18],
    "constraint_types": ['<=', '<=', '<='],
    "problem_type": "max",
    "expected_optimal": 36.0
}

# Example 2: Minimization problem
EXAMPLE_2 = {
    "name": "Diet Problem",
    "description": "Minimize cost while meeting nutritional requirements",
    "c": [2, 3],
    "A": [
        [1, 1],
        [2, 1]
    ],
    "b": [4, 6],
    "constraint_types": ['>=', '>='],
    "problem_type": "min",
    "expected_optimal": 10.0
}

# Example 3: Mixed constraints
EXAMPLE_3 = {
    "name": "Production Planning",
    "description": "Maximize profit with mixed constraint types",
    "c": [5, 4, 3],
    "A": [
        [2, 3, 1],
        [4, 1, 2],
        [3, 4, 2]
    ],
    "b": [50, 60, 70],
    "constraint_types": ['<=', '<=', '<='],
    "problem_type": "max",
    "expected_optimal": None  # To be calculated
}

ALL_EXAMPLES = [EXAMPLE_1, EXAMPLE_2, EXAMPLE_3]
