const DEFAULT_SEGMENTS = 10000;

export const FORMS = [
  {
    id: 1,
    title: 'Linear',
    equation: 'y = mx + b',
    fields: [
      { key: 'm', label: 'Slope (m)' },
      { key: 'b', label: 'Y-Intercept (b)' }
    ]
  },
  {
    id: 2,
    title: 'Quadratic',
    equation: 'y = ax^2 + bx + c',
    fields: [
      { key: 'a', label: 'Coefficient (a)' },
      { key: 'b', label: 'Coefficient (b)' },
      { key: 'c', label: 'Constant (c)' }
    ]
  },
  {
    id: 3,
    title: 'Cosine',
    equation: 'y = A cos(wx + b) + c',
    fields: [
      { key: 'A', label: 'Amplitude (A)' },
      { key: 'w', label: 'Angular Frequency (w)' },
      { key: 'b', label: 'Phase Angle (b)' },
      { key: 'c', label: 'Offset (c)' }
    ]
  },
  {
    id: 4,
    title: 'Sine',
    equation: 'y = A sin(wx + b) + c',
    fields: [
      { key: 'A', label: 'Amplitude (A)' },
      { key: 'w', label: 'Angular Frequency (w)' },
      { key: 'b', label: 'Phase Angle (b)' },
      { key: 'c', label: 'Offset (c)' }
    ]
  },
  {
    id: 5,
    title: 'Exponential',
    equation: 'y = A e^(bx) + c',
    fields: [
      { key: 'A', label: 'Multiplier (A)' },
      { key: 'b', label: 'Exponent Coefficient (b)' },
      { key: 'c', label: 'Offset (c)' }
    ]
  },
  {
    id: 6,
    title: 'Logarithmic',
    equation: 'y = A ln(bx + d) + c',
    fields: [
      { key: 'A', label: 'Multiplier (A)' },
      { key: 'b', label: 'Log Coefficient (b)' },
      { key: 'd', label: 'Log Shift (d)' },
      { key: 'c', label: 'Offset (c)' }
    ]
  }
];

export const METHODS = [
  { id: 'trapezoid', title: 'Trapezoid Rule' },
  { id: 'rectangle', title: 'Rectangle Rule' },
  { id: 'simpson', title: "Simpson's Rule" },
  { id: 'compare', title: 'Compare All Methods' }
];

function evaluateAtX(formId, coeffs, x) {
  const { m = 0, a = 0, b = 0, c = 0, d = 0, A = 0, w = 0 } = coeffs;

  if (formId === 1) return m * x + b;
  if (formId === 2) return a * x * x + b * x + c;
  if (formId === 3) return A * Math.cos(w * x + b) + c;
  if (formId === 4) return A * Math.sin(w * x + b) + c;
  if (formId === 5) return A * Math.exp(b * x) + c;

  if (formId === 6) {
    const inside = b * x + d;
    if (inside <= 0) {
      throw new Error('Log form requires bx + d > 0 across the selected bounds.');
    }
    return A * Math.log(inside) + c;
  }

  throw new Error('Unknown function form selected.');
}

function trapezoid(formId, coeffs, low, high, n = DEFAULT_SEGMENTS) {
  const h = (high - low) / n;
  let sum = 0.5 * (evaluateAtX(formId, coeffs, low) + evaluateAtX(formId, coeffs, high));

  for (let i = 1; i < n; i += 1) {
    const x = low + i * h;
    sum += evaluateAtX(formId, coeffs, x);
  }

  return h * sum;
}

function rectangle(formId, coeffs, low, high, n = DEFAULT_SEGMENTS) {
  const h = (high - low) / n;
  let sum = 0;

  for (let i = 0; i < n; i += 1) {
    const x = low + i * h;
    sum += evaluateAtX(formId, coeffs, x);
  }

  return h * sum;
}

function simpson(formId, coeffs, low, high, n = DEFAULT_SEGMENTS) {
  const evenN = n % 2 === 0 ? n : n + 1;
  const h = (high - low) / evenN;
  let sum = evaluateAtX(formId, coeffs, low) + evaluateAtX(formId, coeffs, high);

  for (let i = 1; i < evenN; i += 1) {
    const x = low + i * h;
    sum += (i % 2 === 0 ? 2 : 4) * evaluateAtX(formId, coeffs, x);
  }

  return (h / 3) * sum;
}

export function runApproximation({ formId, methodId, coeffs, lowBound, upperBound }) {
  if (!Number.isFinite(lowBound) || !Number.isFinite(upperBound)) {
    throw new Error('Bounds must be valid numbers.');
  }

  if (upperBound <= lowBound) {
    throw new Error('Upper bound must be greater than lower bound.');
  }

  if (methodId === 'trapezoid') {
    return { primaryMethod: 'Trapezoid Rule', results: { 'Trapezoid Rule': trapezoid(formId, coeffs, lowBound, upperBound) } };
  }

  if (methodId === 'rectangle') {
    return { primaryMethod: 'Rectangle Rule', results: { 'Rectangle Rule': rectangle(formId, coeffs, lowBound, upperBound) } };
  }

  if (methodId === 'simpson') {
    return { primaryMethod: "Simpson's Rule", results: { "Simpson's Rule": simpson(formId, coeffs, lowBound, upperBound) } };
  }

  if (methodId === 'compare') {
    return {
      primaryMethod: "Simpson's Rule",
      results: {
        'Trapezoid Rule': trapezoid(formId, coeffs, lowBound, upperBound),
        'Rectangle Rule': rectangle(formId, coeffs, lowBound, upperBound),
        "Simpson's Rule": simpson(formId, coeffs, lowBound, upperBound)
      }
    };
  }

  throw new Error('Unknown method selected.');
}

export function getFormById(id) {
  return FORMS.find((form) => form.id === id) || null;
}

export function getMethodById(id) {
  return METHODS.find((method) => method.id === id) || null;
}
