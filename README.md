# Definite-Integral-Approximator

A simple and functional Definite Integral Approximator with:

- C++ integration logic
- A lightweight web UI
- Node.js server bridge between UI and C++

## Features

- Choose function form:
  - `y = mx + b`
  - `y = ax^2 + bx + c`
  - `y = A cos(wx + b) + c`
  - `y = A sin(wx + b) + c`
  - `y = A e^(bx) + c`
  - `y = A ln(bx + d) + c`
- Choose method:
  - Trapezoid Rule
  - Rectangle Rule
  - Simpson's Rule
  - Compare All Methods
- Enter coefficients and integration bounds in the UI
- Get approximate result (and method comparison when selected)

## Requirements

- Node.js 18+
- `g++` with C++17 support

## Quick Start

```bash
npm start
```

Then open:

- `http://localhost:4000`

## How It Works

1. The server compiles `integral_approximation.cpp` into `bin/integral_approx`.
2. The browser UI sends input values to the Node.js server.
3. The server passes those values to the C++ program via stdin.
4. The server parses the C++ output and returns JSON to the UI.

## Available Scripts

- `npm start`:
  - Compiles C++ (`npm run build:cpp`)
  - Starts the web server on port `4000`
- `npm run build:cpp`:
  - Builds `integral_approximation.cpp` into `bin/integral_approx`

## Project Structure

- `integral_approximation.cpp`: core C++ approximation logic
- `server.js`: Node.js server and C++ process bridge
- `public/index.html`: frontend UI
- `bin/`: compiled C++ executable output

## Troubleshooting

- `g++: command not found`:
  - Install a C++ compiler toolchain, then rerun `npm start`.
- `EADDRINUSE` or port already in use:
  - Stop the process using port `4000`, or run with a different port:
  - `PORT=4001 npm start`
