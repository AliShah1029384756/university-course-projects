# Banking System (C++)

![Course](https://img.shields.io/badge/Course-Operating%20Systems-0ea5e9)
![Language](https://img.shields.io/badge/Language-C%2B%2B-16a34a)

## Overview

C++ console-based banking/OS simulation project with account workflows and system-level concepts.

## Tech Stack

- C++
- `pthread.h`, `semaphore.h`, and System V message queue APIs

## Key File

- `Banking.cpp`

## Build and Run (Linux/WSL)

```bash
g++ Banking.cpp -o banking -pthread
./banking
```

## Notes

- On Windows, WSL or a POSIX-compatible environment is recommended because of Linux-specific APIs.
