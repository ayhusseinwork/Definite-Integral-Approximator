# Integral Approximation UI (React)

This project now includes a React UI that follows this flow:

1. Welcome screen
2. Select function form (buttons)
3. Select approximation method (buttons)
4. Enter coefficients and bounds
5. Calculate and display result
6. Restart or edit values

## Files

- `src/App.jsx`: UI flow and screen logic
- `src/calculator.js`: Numerical methods + function models
- `src/styles.css`: Visual design and responsive styles
- `integral_approximation.cpp`: Converted C++ console implementation

## Run

Install Node.js first (Node 18+ recommended), then:

```bash
npm install
npm run dev
```

Build for production:

```bash
npm run build
npm run preview
```
