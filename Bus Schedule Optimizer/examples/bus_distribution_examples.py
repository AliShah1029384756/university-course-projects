"""Example datasets for Transportation problems."""

# Example 1: Balanced 3x3 transportation problem
EXAMPLE_1 = {
    "name": "Standard Transportation (3×3)",
    "description": "Ship from 3 warehouses to 3 stores",
    "costs": [
        [8, 6, 10],
        [9, 12, 13],
        [14, 9, 16]
    ],
    "supply": [20, 30, 25],
    "demand": [15, 25, 35],
    "expected_cost": None  # To be calculated
}

# Example 2: Unbalanced - excess supply
EXAMPLE_2 = {
    "name": "Excess Supply",
    "description": "Supply exceeds demand - dummy destination needed",
    "costs": [
        [2, 3, 1],
        [5, 4, 8],
        [5, 6, 8]
    ],
    "supply": [15, 25, 10],
    "demand": [10, 20, 15],
    "expected_cost": None
}

# Example 3: Unbalanced - excess demand
EXAMPLE_3 = {
    "name": "Excess Demand",
    "description": "Demand exceeds supply - dummy source needed",
    "costs": [
        [6, 1, 9],
        [11, 5, 2]
    ],
    "supply": [40, 60],
    "demand": [20, 30, 60],
    "expected_cost": None
}

# Example 4: 4x3 problem
EXAMPLE_4 = {
    "name": "Large Transportation (4×3)",
    "description": "4 sources to 3 destinations",
    "costs": [
        [10, 15, 25],
        [20, 10, 30],
        [15, 20, 10],
        [25, 30, 15]
    ],
    "supply": [30, 40, 35, 25],
    "demand": [40, 50, 40],
    "expected_cost": None
}

ALL_EXAMPLES = [EXAMPLE_1, EXAMPLE_2, EXAMPLE_3, EXAMPLE_4]
