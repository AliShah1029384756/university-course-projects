# Bus Schedule Optimizer

![Course](https://img.shields.io/badge/Course-Operations%20Research-0ea5e9)
![App](https://img.shields.io/badge/App-Pygame%20Desktop-16a34a)

Desktop GUI application for solving core Operations Research problems in a transportation and bus-scheduling context.

---

## Overview

This project includes educational implementations for:

1. Linear Programming (Simplex)
2. Assignment Problem (Hungarian)
3. Transportation Problem (Northwest Corner, Least Cost, MODI)

It is designed for coursework demonstrations with step-by-step algorithm outputs.

---

## Tech Stack

- Python 3.8+
- Pygame
- NumPy

---

## Run Locally

```bash
pip install -r requirements.txt
python bus_scheduler_app.py
```

Optional tests:

```bash
pytest tests/
```

---

## Project Structure

```text
Bus Schedule Optimizer/
|- bus_scheduler_app.py
|- README.md
|- requirements.txt
|- gui/
|- solvers/
|- examples/
|- tests/
|- assets/
```

---

## Key Features

- Interactive GUI with problem-specific screens
- Input validation and example datasets
- Step-by-step educational outputs
- Multiple OR techniques in one application

---

## Academic Context

Prepared as an Operations Research project focused on practical optimization workflows for transport planning and scheduling.

## Notes

- This repository version is for educational and portfolio use.
- Algorithm naming and output format are kept classroom-friendly.
