import { spawn } from 'node:child_process';
import { access, readFile } from 'node:fs/promises';
import { constants } from 'node:fs';
import http from 'node:http';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);
const binaryPath = path.join(__dirname, 'bin', 'integral_approx');
const indexPath = path.join(__dirname, 'public', 'index.html');
const port = Number(process.env.PORT || 4000);

function sendJson(res, status, payload) {
  res.writeHead(status, { 'Content-Type': 'application/json' });
  res.end(JSON.stringify(payload));
}

function buildInputScript(body) {
  const { formId, coeffs, lowBound, upperBound, methodId } = body;

  if (![1, 2, 3, 4, 5, 6].includes(formId)) {
    throw new Error('Form must be 1-6.');
  }

  if (![1, 2, 3, 4].includes(methodId)) {
    throw new Error('Method must be 1-4.');
  }

  if (!Number.isFinite(lowBound) || !Number.isFinite(upperBound)) {
    throw new Error('Bounds must be numbers.');
  }

  if (upperBound <= lowBound) {
    throw new Error('Upper bound must be greater than lower bound.');
  }

  const lines = [String(formId)];

  if (formId === 1) {
    lines.push(String(coeffs.m), String(coeffs.b));
  } else if (formId === 2) {
    lines.push(String(coeffs.a), String(coeffs.b), String(coeffs.c));
  } else if (formId === 3 || formId === 4) {
    lines.push(String(coeffs.A), String(coeffs.w), String(coeffs.b), String(coeffs.c));
  } else if (formId === 5) {
    lines.push(String(coeffs.A), String(coeffs.b), String(coeffs.c));
  } else if (formId === 6) {
    lines.push(String(coeffs.A), String(coeffs.b), String(coeffs.d), String(coeffs.c));
  }

  if (lines.some((value) => !Number.isFinite(Number(value)))) {
    throw new Error('All coefficients must be valid numbers.');
  }

  lines.push(String(lowBound), String(upperBound), String(methodId), '1');
  return `${lines.join('\n')}\n`;
}

function parseOutput(stdout) {
  const resultMatch = stdout.match(/The definite integral approximately equals:\s*([-+]?\d*\.?\d+(?:[eE][-+]?\d+)?|nan|-nan|inf|-inf)/i);
  if (!resultMatch) {
    throw new Error('Could not parse result from C++ output.');
  }

  const result = Number(resultMatch[1]);
  if (!Number.isFinite(result)) {
    throw new Error('C++ returned a non-finite result.');
  }

  const comparison = {};
  const t = stdout.match(/Trapezoid Rule:\s*([-+]?\d*\.?\d+(?:[eE][-+]?\d+)?)/i);
  const r = stdout.match(/Rectangle Rule:\s*([-+]?\d*\.?\d+(?:[eE][-+]?\d+)?)/i);
  const s = stdout.match(/Simpson's Rule:\s*([-+]?\d*\.?\d+(?:[eE][-+]?\d+)?)/i);

  if (t) comparison.trapezoid = Number(t[1]);
  if (r) comparison.rectangle = Number(r[1]);
  if (s) comparison.simpson = Number(s[1]);

  return {
    result,
    comparison
  };
}

function runCpp(inputScript) {
  return new Promise((resolve, reject) => {
    const child = spawn(binaryPath, [], { stdio: 'pipe' });

    let stdout = '';
    let stderr = '';

    child.stdout.on('data', (data) => {
      stdout += data.toString();
    });

    child.stderr.on('data', (data) => {
      stderr += data.toString();
    });

    child.on('error', (error) => {
      reject(new Error(`Failed to launch C++ binary: ${error.message}`));
    });

    child.on('close', (code) => {
      if (code !== 0) {
        reject(new Error(stderr.trim() || `C++ exited with code ${code}.`));
        return;
      }

      try {
        resolve(parseOutput(stdout));
      } catch (err) {
        reject(err);
      }
    });

    child.stdin.write(inputScript);
    child.stdin.end();
  });
}

async function serveIndex(res) {
  const html = await readFile(indexPath, 'utf8');
  res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
  res.end(html);
}

async function ensureBinaryBuilt() {
  try {
    await access(binaryPath, constants.X_OK);
  } catch {
    throw new Error('C++ binary is missing. Run: npm run build:cpp');
  }
}

const server = http.createServer(async (req, res) => {
  if (req.method === 'GET' && req.url === '/') {
    try {
      await serveIndex(res);
    } catch {
      sendJson(res, 500, { error: 'Could not load index.html' });
    }
    return;
  }

  if (req.method === 'POST' && req.url === '/calculate') {
    let raw = '';
    req.on('data', (chunk) => {
      raw += chunk;
    });

    req.on('end', async () => {
      try {
        const body = JSON.parse(raw || '{}');
        const inputScript = buildInputScript(body);
        const output = await runCpp(inputScript);
        sendJson(res, 200, output);
      } catch (error) {
        sendJson(res, 400, { error: error instanceof Error ? error.message : 'Unknown error' });
      }
    });
    return;
  }

  sendJson(res, 404, { error: 'Not found' });
});

ensureBinaryBuilt()
  .then(() => {
    server.listen(port, () => {
      console.log(`Simple C++ UI server running at http://localhost:${port}`);
      console.log(`Using binary: ${binaryPath}`);
    });
  })
  .catch((error) => {
    console.error(error.message);
    process.exit(1);
  });
