import { useMemo, useState } from 'react';
import {
  FORMS,
  METHODS,
  getFormById,
  getMethodById,
  runApproximation
} from './calculator';

const STEPS = {
  WELCOME: 'welcome',
  FORM: 'form',
  METHOD: 'method',
  INPUTS: 'inputs',
  RESULT: 'result'
};

function numberToText(value) {
  if (!Number.isFinite(value)) return 'Invalid Result';
  return value.toLocaleString(undefined, {
    maximumFractionDigits: 10
  });
}

function buildInitialInputs(form) {
  const next = { lowBound: '', upperBound: '' };
  form.fields.forEach((field) => {
    next[field.key] = '';
  });
  return next;
}

export default function App() {
  const [step, setStep] = useState(STEPS.WELCOME);
  const [selectedFormId, setSelectedFormId] = useState(null);
  const [selectedMethodId, setSelectedMethodId] = useState(null);
  const [inputs, setInputs] = useState({});
  const [error, setError] = useState('');
  const [resultPayload, setResultPayload] = useState(null);

  const selectedForm = useMemo(() => getFormById(selectedFormId), [selectedFormId]);
  const selectedMethod = useMemo(() => getMethodById(selectedMethodId), [selectedMethodId]);

  function startFlow() {
    setStep(STEPS.FORM);
    setError('');
  }

  function chooseForm(formId) {
    setSelectedFormId(formId);
    setSelectedMethodId(null);
    setResultPayload(null);
    setError('');
    setStep(STEPS.METHOD);
  }

  function chooseMethod(methodId) {
    const form = getFormById(selectedFormId);
    if (!form) {
      setError('Pick a function form first.');
      setStep(STEPS.FORM);
      return;
    }

    setSelectedMethodId(methodId);
    setInputs(buildInitialInputs(form));
    setError('');
    setStep(STEPS.INPUTS);
  }

  function onInputChange(key, value) {
    setInputs((prev) => ({ ...prev, [key]: value }));
  }

  function calculate() {
    if (!selectedForm || !selectedMethod) {
      setError('Choose both a function form and method first.');
      return;
    }

    try {
      const coeffs = {};
      selectedForm.fields.forEach((field) => {
        const parsed = Number(inputs[field.key]);
        if (!Number.isFinite(parsed)) {
          throw new Error(`Enter a valid number for ${field.label}.`);
        }
        coeffs[field.key] = parsed;
      });

      const lowBound = Number(inputs.lowBound);
      const upperBound = Number(inputs.upperBound);

      const output = runApproximation({
        formId: selectedForm.id,
        methodId: selectedMethod.id,
        coeffs,
        lowBound,
        upperBound
      });

      setResultPayload({
        ...output,
        form: selectedForm,
        method: selectedMethod,
        lowBound,
        upperBound,
        coeffs
      });

      setError('');
      setStep(STEPS.RESULT);
    } catch (err) {
      setError(err instanceof Error ? err.message : 'Something went wrong while calculating.');
    }
  }

  function restart() {
    setStep(STEPS.WELCOME);
    setSelectedFormId(null);
    setSelectedMethodId(null);
    setInputs({});
    setError('');
    setResultPayload(null);
  }

  return (
    <main className="app-shell">
      <div className="grain" aria-hidden="true" />
      <section className="panel">
        <header className="panel-header">
          <p className="eyebrow">Definite Integral Approximation</p>
          <h1>Integral Studio</h1>
          <p className="subtitle">Select a form, pick a method, calculate, and restart anytime.</p>
        </header>

        {step === STEPS.WELCOME && (
          <div className="step-block">
            <h2>Welcome</h2>
            <p>We&apos;ll walk through the same flow as your original program, now in UI form.</p>
            <button className="cta" onClick={startFlow}>
              Start Approximation
            </button>
          </div>
        )}

        {step === STEPS.FORM && (
          <div className="step-block">
            <h2>Step 1: Choose Function Form</h2>
            <div className="button-grid">
              {FORMS.map((form) => (
                <button key={form.id} className="select-btn" onClick={() => chooseForm(form.id)}>
                  <span>{form.title}</span>
                  <small>{form.equation}</small>
                </button>
              ))}
            </div>
          </div>
        )}

        {step === STEPS.METHOD && selectedForm && (
          <div className="step-block">
            <h2>Step 2: Choose Method</h2>
            <p className="selection-pill">Form: {selectedForm.equation}</p>
            <div className="button-grid method-grid">
              {METHODS.map((method) => (
                <button key={method.id} className="select-btn" onClick={() => chooseMethod(method.id)}>
                  <span>{method.title}</span>
                </button>
              ))}
            </div>
            <button className="ghost" onClick={() => setStep(STEPS.FORM)}>
              Back to Function Form
            </button>
          </div>
        )}

        {step === STEPS.INPUTS && selectedForm && selectedMethod && (
          <div className="step-block">
            <h2>Step 3: Enter Coefficients and Bounds</h2>
            <p className="selection-pill">{selectedForm.equation}</p>
            <p className="selection-pill">Method: {selectedMethod.title}</p>

            <div className="input-grid">
              {selectedForm.fields.map((field) => (
                <label key={field.key}>
                  <span>{field.label}</span>
                  <input
                    type="number"
                    step="any"
                    value={inputs[field.key] ?? ''}
                    onChange={(event) => onInputChange(field.key, event.target.value)}
                    placeholder="Enter value"
                  />
                </label>
              ))}

              <label>
                <span>Lower Bound</span>
                <input
                  type="number"
                  step="any"
                  value={inputs.lowBound ?? ''}
                  onChange={(event) => onInputChange('lowBound', event.target.value)}
                  placeholder="e.g. 0"
                />
              </label>

              <label>
                <span>Upper Bound</span>
                <input
                  type="number"
                  step="any"
                  value={inputs.upperBound ?? ''}
                  onChange={(event) => onInputChange('upperBound', event.target.value)}
                  placeholder="e.g. 3.14"
                />
              </label>
            </div>

            <div className="actions">
              <button className="cta" onClick={calculate}>
                Calculate
              </button>
              <button className="ghost" onClick={() => setStep(STEPS.METHOD)}>
                Back to Methods
              </button>
            </div>
          </div>
        )}

        {step === STEPS.RESULT && resultPayload && (
          <div className="step-block">
            <h2>Step 4: Result</h2>
            <p className="selection-pill">Form: {resultPayload.form.equation}</p>
            <p className="selection-pill">Bounds: [{resultPayload.lowBound}, {resultPayload.upperBound}]</p>

            <div className="result-box">
              {Object.entries(resultPayload.results).map(([methodName, value]) => (
                <div key={methodName} className="result-row">
                  <span>{methodName}</span>
                  <strong>{numberToText(value)}</strong>
                </div>
              ))}
            </div>

            <p className="prompt">Would you like to restart and run another approximation?</p>
            <div className="actions">
              <button className="cta" onClick={restart}>
                Restart
              </button>
              <button className="ghost" onClick={() => setStep(STEPS.INPUTS)}>
                Edit Values
              </button>
            </div>
          </div>
        )}

        {error && <p className="error-banner">{error}</p>}
      </section>
    </main>
  );
}
