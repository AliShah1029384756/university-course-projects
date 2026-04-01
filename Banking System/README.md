# Banking System (C++)

![Course](https://img.shields.io/badge/Course-Operating%20Systems-0ea5e9)
![Language](https://img.shields.io/badge/Language-C%2B%2B-16a34a)

## Overview

C++ console-based banking and OS simulation with account management, transaction scheduling, memory mapping, and file allocation tracking.

## Tech Stack

- C++
- `pthread.h`, `semaphore.h`, and System V message queue APIs

## Key File

- `Banking.cpp`

## Core Modules

- Account management: create account, enquiry, and account list display
- Transaction processing: deposit/withdraw transactions with Round Robin scheduling
- Process tracking: transaction status table and scheduling metrics
- Memory management: page loading and memory map display
- Disk simulation: FAT table update and I/O request queue processing
- IPC integration: message queue notifications for main actions

## Current Runtime Behavior

- Transactions are queued first and then applied during scheduler execution
- Account lookup is validated by account number before updates
- Duplicate account numbers are rejected during account creation
- Scheduler quantum must be greater than 0
- Message queue operations include runtime safety checks

## Build and Run (Linux/WSL)

```bash
g++ Banking.cpp -o banking -pthread
./banking
```

## Main Menu Options

1. New Account
2. Deposit Amount
3. Withdraw Amount
4. Balance Enquiry
5. Display All Accounts
6. Display All Transactions
7. Execute Transactions with Round Robin Scheduling
8. Display Memory Map
9. Display FAT Table
10. Add I/O Request
11. Process I/O Requests
12. Exit

## Notes

- On Windows, WSL or a POSIX-compatible environment is recommended because of Linux-specific APIs.
