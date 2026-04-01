# Inventory Management System (IMS)

A C++ Inventory Management System developed for OOP coursework requirements. The application supports three core roles:

- Administrator
- Shopkeeper
- Customer

The system is file-based (no database) and implements role-based operations for user and product management, cart handling, and checkout.

## Project Rename

The project has been renamed to:

- Project display name: InventoryManagementSystem
- Output target/executable name: InventoryManagementSystem

Updated metadata files:

- InventoryManagementSystem.sln
- InventoryManagementSystem/InventoryManagementSystem.vcxproj

## Repository Quality

This repository is cleaned for public upload:

- Build artifacts are excluded via .gitignore
- Visual Studio local cache/user files are excluded
- Legacy runtime CSV files from earlier iterations are removed
- Only IMS-relevant source, project, and required data files are retained

## Technology Stack

- Language: C++
- IDE/Build System: Visual Studio (VC++ project)
- Runtime Model: Console application (entry from MyForm.cpp)
- Data Persistence: Plain text files

## Core Features

### 1. Administrator Module

- Sign in with fixed credentials (`admin` / `admin`)
- Add new shopkeeper
- Add new customer
- Add new product
- Edit shopkeeper details
- Edit customer details
- Edit product details
- Delete shopkeeper
- Delete customer
- Delete product
- View all shopkeepers, customers, products

### 2. Shopkeeper Module

- Register as a shopkeeper
- Sign in with username/password
- Add products
- Edit own products
- View own products

### 3. Customer Module

- Register as a customer
- Sign in with username/password
- Enter wallet amount after login
- View available products
- Add products to cart
- Update cart quantity
- Remove products from cart
- View bill summary with:
  - 2% sales tax
  - fixed 200 delivery charges
- Checkout with stock validation and cash validation

## Data Files (Required)

All data is persisted in project-local text files:

- InventoryManagementSystem/shopkeeper.txt
- InventoryManagementSystem/customer.txt
- InventoryManagementSystem/products.txt
- InventoryManagementSystem/addToCart.txt

## ID and Data Rules

- Shopkeeper ID: auto-generated 4-digit numeric string (`0001`, `0002`, ...)
- Customer ID: auto-generated 4-digit numeric string (`0001`, `0002`, ...)
- Product ID: category code + serial format (`CCNNNN`), for example:
  - `01` for Sports
  - `02` for Garments
  - `03` for Eatables
  - `04` for Medicines
  - `05` for Fashion
- Product quantity `0` is treated as out-of-stock
- Username uniqueness is enforced across shopkeepers and customers

## OOP Concepts Covered

The implementation includes multiple OOP concepts expected by the assignment:

- Classes and constructors
- Inheritance (`Person` -> `Shopkeeper`, `Customer`)
- Virtual functions / runtime polymorphism
- Static data members (ID/serial counters)
- Operator overloading (`Product::operator+=`)
- Friend function (`operator<<` for `Product`)
- Modular multi-file structure

## Folder Structure (Relevant)

- InventoryManagementSystem.sln
- InventoryManagementSystem/
  - IMS.h
  - IMS.cpp
  - MyForm.cpp (entry point)
  - InventoryManagementSystem.vcxproj
  - shopkeeper.txt
  - customer.txt
  - products.txt
  - addToCart.txt

## Build and Run

1. Open `InventoryManagementSystem.sln` in Visual Studio.
2. Select configuration: `Debug | x64` (recommended).
3. Build the solution.
4. Run the project.
5. A console window opens with the IMS main menu.

## Usage Flow (Recommended Demo)

1. Login as Administrator (`admin` / `admin`).
2. Add at least one Shopkeeper and one Customer.
3. Add products (either through Admin or Shopkeeper login).
4. Login as Customer and add products to cart.
5. Perform checkout to validate stock deduction and billing logic.

## Default Credentials

- Administrator
  - Username: `admin`
  - Password: `admin`

## Submission Checklist

Before final submission:

- Add group roll numbers at top comments where required.
- Verify all four required data files are included in the zip.
- Include this README in submission package.
- Perform one complete test run for:
  - Admin CRUD operations
  - Shopkeeper product operations
  - Customer cart and checkout flow

## Notes

- The active IMS flow is implemented through `IMS.h`, `IMS.cpp`, and the current `main` in `MyForm.cpp`.
- Keep file names and relative paths unchanged so runtime file loading works correctly.
