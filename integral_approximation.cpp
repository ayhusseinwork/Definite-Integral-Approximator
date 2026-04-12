#include <cmath>
#include <iostream>

using namespace std;

float integral_program();

int get_form();

void get_Coefficients(int form, float& w, float& a, float& b, float& d, float& c);

void coefficients(int form, float& b, float& c);                         // Linear
void coefficients(int form, float& a, float& b, float& c);               // Quadratic or exponential
void coefficients(int form, float& A, float& w, float& b, float& c);     // Sinusoidal
void coefficients(int form, float& a, float& b, float& d, float& c, float& dummy);  // Logarithmic

void get_bounds(float& low_bound, float& upper_bound);

int get_method();

float trapezoids(int form, float w, float a, float b, float d, float c, float low_bound, float upper_bound);
float rectangles(int form, float w, float a, float b, float d, float c, float low_bound, float upper_bound);
float parabola(int form, float w, float a, float b, float d, float c, float low_bound, float upper_bound);

float f(float x, int form, float w, float a, float b, float d, float c);

void result_displayer(float result);

int main() {
    float result = integral_program();
    result_displayer(result);
    return 0;
}

// Prompts user for inputs and approximates a definite integral.
float integral_program() {
    float result = 0.0f;
    float a = 0.0f, b = 0.0f, d = 0.0f, c = 0.0f, w = 0.0f;
    float low_bound = 0.0f, upper_bound = 0.0f;
    int method, form;

    form = get_form();
    get_Coefficients(form, w, a, b, d, c);
    get_bounds(low_bound, upper_bound);
    method = get_method();

    if (method == 1) {
        result = trapezoids(form, w, a, b, d, c, low_bound, upper_bound);
    } else if (method == 2) {
        result = rectangles(form, w, a, b, d, c, low_bound, upper_bound);
    } else if (method == 3) {
        result = parabola(form, w, a, b, d, c, low_bound, upper_bound);
    } else if (method == 4) {
        float t = trapezoids(form, w, a, b, d, c, low_bound, upper_bound);
        float r = rectangles(form, w, a, b, d, c, low_bound, upper_bound);
        float p = parabola(form, w, a, b, d, c, low_bound, upper_bound);
        cout << "\nComparison:\n";
        cout << "Trapezoid Rule: " << t << '\n';
        cout << "Rectangle Rule: " << r << '\n';
        cout << "Simpson's Rule: " << p << '\n';
        result = p;  // Return Simpson's estimate by default after comparison.
    } else {
        cout << "\nInvalid method selected. Defaulting to Simpson's Rule.\n";
        result = parabola(form, w, a, b, d, c, low_bound, upper_bound);
    }

    return result;
}

int get_form() {
    int form;

    cout << "\n\tDefinite Integral Approximator\n\n";
    cout << "Choose form of your function: \n\n";
    cout << "\t1). y = mx + b\n";
    cout << "\t2). y = ax^2 + bx + c\n";
    cout << "\t3). y = A cos(wx + b) + c\n";
    cout << "\t4). y = A sin(wx + b) + c\n";
    cout << "\t5). y = A e^(bx) + c\n";
    cout << "\t6). y = A ln(bx + d) + c\n\n";
    cout << "Enter the number only: ";
    cin >> form;

    return form;
}

void get_bounds(float& low_bound, float& upper_bound) {
    cout << "\nEnter Lower Bound: ";
    cin >> low_bound;
    cout << "\nEnter Upper Bound: ";
    cin >> upper_bound;
}

int get_method() {
    int method;

    cout << "\nChoose method of approximation: \n\n";
    cout << "\t1). Trapezoidal Approximation (Trapezoid Rule)\n";
    cout << "\t2). Rectangular Approximation (Riemann Sums)\n";
    cout << "\t3). Parabolic Approximation (Simpson's Rule)\n\n";
    cout << "\tOr enter 4 to compare all methods.\n\n";
    cout << "Enter the number only: ";
    cin >> method;

    return method;
}

float trapezoids(int form, float w, float a, float b, float d, float c, float low_bound, float upper_bound) {
    float result = 0.0f;
    float x = low_bound;
    const int size = 10000;
    float array[size];

    for (int i = 0; i < size; i++) {
        array[i] = f(x, form, w, a, b, d, c);
        x += (upper_bound - low_bound) / size;
    }

    for (int i = 0; i < size - 1; i++) {
        result += ((upper_bound - low_bound) / size) * ((array[i] + array[i + 1]) / 2.0f);
    }

    return result;
}

float rectangles(int form, float w, float a, float b, float d, float c, float low_bound, float upper_bound) {
    float result = 0.0f;
    float x = low_bound;
    const int size = 10000;
    float array[size];

    for (int i = 0; i < size; i++) {
        array[i] = f(x, form, w, a, b, d, c);
        x += (upper_bound - low_bound) / size;
    }

    for (int i = 0; i < size - 1; i++) {
        result += ((upper_bound - low_bound) / size) * array[i];
    }

    return result;
}

float parabola(int form, float w, float a, float b, float d, float c, float low_bound, float upper_bound) {
    float result = 0.0f;
    float x = low_bound;
    const int size = 10000;
    float array[size];

    for (int i = 0; i < size; i++) {
        array[i] = f(x, form, w, a, b, d, c);
        x += (upper_bound - low_bound) / size;
    }

    float h = (upper_bound - low_bound) / size;

    for (int i = 0; i < size - 1; i++) {
        if (i == 0 || i == size - 1) {
            result += (h / 3.0f) * array[i];
        } else if (i % 2 == 1) {
            result += (h / 3.0f) * 4.0f * array[i];
        } else {
            result += (h / 3.0f) * 2.0f * array[i];
        }
    }

    return result;
}

float f(float x, int form, float w, float a, float b, float d, float c) {
    float y = 0.0f;

    if (form == 1) {
        y = b * x + c;
    } else if (form == 2) {
        y = a * x * x + b * x + c;
    } else if (form == 3) {
        y = a * cos(w * x + b) + c;
    } else if (form == 4) {
        y = a * sin(w * x + b) + c;
    } else if (form == 5) {
        y = a * exp(b * x) + c;
    } else if (form == 6) {
        y = a * log(b * x + d) + c;
    }

    return y;
}

void get_Coefficients(int form, float& w, float& a, float& b, float& d, float& c) {
    float dummy;

    if (form == 1) {
        coefficients(form, b, c);
    } else if (form == 2 || form == 5) {
        coefficients(form, a, b, c);
    } else if (form == 3 || form == 4) {
        coefficients(form, a, w, b, c);
    } else if (form == 6) {
        coefficients(form, a, b, d, c, dummy);
    }
}

void coefficients(int form, float& b, float& c) {
    cout << "\nYou chose 1). y = mx + b\n";
    cout << "\nEnter the slope (m): ";
    cin >> b;
    cout << "\nEnter the y-intercept (b): ";
    cin >> c;
}

void coefficients(int form, float& a, float& b, float& c) {
    if (form == 2) {
        cout << "\nYou chose 2). y = ax^2 + bx + c\n";
        cout << "\nEnter the first coefficient (a): ";
    } else if (form == 5) {
        cout << "\nYou chose 5). y = A e^(bx) + c\n";
        cout << "\nEnter the first coefficient (A): ";
    }

    cin >> a;
    cout << "\nEnter the second coefficient (b): ";
    cin >> b;
    cout << "\nEnter the third coefficient (c): ";
    cin >> c;
}

void coefficients(int form, float& A, float& w, float& b, float& c) {
    if (form == 3) {
        cout << "\nYou chose 3). y = A cos(wx + b) + c\n";
    } else if (form == 4) {
        cout << "\nYou chose 4). y = A sin(wx + b) + c\n";
    }

    cout << "\nEnter the amplitude (A): ";
    cin >> A;
    cout << "\nEnter the angular frequency (w): ";
    cin >> w;
    cout << "\nEnter the phase angle (b): ";
    cin >> b;
    cout << "\nEnter the coefficient (c): ";
    cin >> c;
}

void coefficients(int form, float& a, float& b, float& d, float& c, float& dummy) {
    cout << "\nYou chose 6). y = A ln(bx + d) + c\n";
    cout << "\nEnter the first coefficient (A): ";
    cin >> a;
    cout << "\nEnter the second coefficient (b): ";
    cin >> b;
    cout << "\nEnter the third coefficient (d): ";
    cin >> d;
    cout << "\nEnter the fourth coefficient (c): ";
    cin >> c;
    dummy = 0.0f;
}

void result_displayer(float result) {
    float result2;

    cout << "\nThe definite integral approximately equals: " << result << endl;

    char restart;
    cout << "\nEnter R to restart, or enter 1 to exit: ";
    cin >> restart;

    if (restart == 'r' || restart == 'R') {
        result2 = integral_program();
        result_displayer(result2);
    }
}

