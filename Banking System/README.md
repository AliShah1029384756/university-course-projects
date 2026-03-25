# Banking System (C++)

## Overview

A C++ console-based banking/OS simulation project with features related to account handling and system-level concepts.

## Tech Stack

- C++
- Uses headers such as `pthread.h`, `semaphore.h`, and System V message queue APIs.

## Key File

- `Banking.cpp`

## Build And Run (Linux/WSL)

```bash
g++ Banking.cpp -o banking -pthread
./banking
```

## Notes

- On Windows, easiest path is WSL or a POSIX-compatible environment due to Linux-specific APIs.
