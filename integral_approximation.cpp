// Definite Integral Approximator
// Console program that approximates integrals for multiple function forms
// using Trapezoid, Rectangle, and Simpson methods.

// Function declarations.
void integral_program();

void welcome_function();

int get_form();

void get_bounds(float& low_bound, float& upper_bound);

void get_Coefficients(int form, float& w, float& a, float& b, float& d, float& c, float low_bound, float upper_bound); // Routes to the correct coefficient-input overload.

void coefficients(int form, float& b, float& c); // Prompts coefficients for linear form.
void coefficients(int form, float& a, float& b, float& c); // Prompts coefficients for quadratic/exponential form.
void coefficients(int form, float& A, float& w, float& b, float& c); // Prompts coefficients for sinusoidal form.
void coefficients(int form, float& a, float& b, float& d, float& c, float low_bound, float upper_bound); // Prompts coefficients for log form with domain validation.

int get_method();

float method_selector(int method, int form, float w, float a, float b, float d, float c, float low_bound, float upper_bound);

void compare_methods(int form, float w, float a, float b, float d, float c, float low_bound, float upper_bound);

float trapazoids(int form, float w, float a, float b, float d, float c, float low_bound, float upper_bound);

float rectangles(int form, float w, float a, float b, float d, float c, float low_bound, float upper_bound);

float parabola(int form, float w, float a, float b, float d, float c, float low_bound, float upper_bound);

float f(float x, int form, float w, float a, float b, float d, float c);

void result_displayer(float result, int method);

void restart();

#include <iostream>
#include <cstring>
#include <cmath>
using namespace std;

// Program entry point:
// shows title, runs one calculation flow, then offers restart.
int main() {
float result;
welcome_function();
integral_program();
restart();
return 0;
}

// Runs one full solve cycle:
// choose form -> enter bounds/coefficients -> choose method -> display result.

void integral_program() {
float result, a, b, d, c, w, low_bound, upper_bound;
int method, form;

form = get_form();

get_bounds(low_bound, upper_bound);

get_Coefficients(form, w, a, b, d, c, low_bound, upper_bound);

method = get_method();

result = method_selector(method, form, w, a, b, d, c, low_bound, upper_bound);

result_displayer(result, method);
}

// Displays the program title banner.
void welcome_function() {
cout << "\n\tDefinite Integral Approximator\n";
}

// Prompts the user to choose which function family to integrate.
// Returns the selected menu number (1-6).

int get_form() {

int form;

cout << "\nChoose form of your function: \n\n";

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

// Reads lower and upper integration bounds by reference.

void get_bounds(float& low_bound, float& upper_bound) {
cout << "\nEnter Lower Bound: ";
cin >> low_bound;
cout << "\nEnter Upper Bound: ";
cin >> upper_bound;
}

// Prompts the user to choose approximation method (1-4) and returns it.

int get_method() {
int method;

cout << "\nChoose method of approximation: \n\n";

cout << "\t1). Trapazoidal Approximation (Trapezoid Rule) \n";
cout << "\t2). Rectangular Approximation (Riemann Sums) \n";
cout << "\t3). Parabolic Approximation (Simpson's Rule) \n\n";
cout << "\tOr Enter 4 to compare all methods!\n\n";

cout << "Enter the number only: ";
cin >> method;

return method;
}

// Dispatches to the selected approximation method.
// For option 4, it prints all methods and returns 0.

float method_selector(int method, int form, float w, float a, float b, float d, float c, float low_bound, float upper_bound) {
float result = 0;

if (method == 1)
result = trapazoids(form, w, a, b, d, c, low_bound, upper_bound);
else if (method == 2)
result = rectangles(form, w, a, b, d, c, low_bound, upper_bound);
else if (method == 3)
result = parabola(form, w, a, b, d, c, low_bound, upper_bound);
else if (method == 4) {
compare_methods(form, w, a, b, d, c, low_bound, upper_bound);
}
// If method is invalid, returns 0 by default.

return result;
}

// Prints trapezoid, rectangle, and Simpson estimates for the same inputs.

void compare_methods(int form, float w, float a, float b, float d, float c, float low_bound, float upper_bound) {
cout << "\nTrapazoid Approximation: " << trapazoids(form, w, a, b, d, c, low_bound, upper_bound);
cout << "\nRectangular Approximation: " << rectangles(form, w, a, b, d, c, low_bound, upper_bound);
cout << "\nParabolic Approximation: " << parabola(form, w, a, b, d, c, low_bound, upper_bound);
}

// Approximates the definite integral using the trapezoid rule.

float trapazoids(int form, float w, float a, float b, float d, float c, float low_bound, float upper_bound) {
float result = 0, x = low_bound;
const int size = 10000;
float array[size];

// Sample function values over the interval.
for (int i = 0; i < size; i++) {
array[i] = f(x, form, w, a, b, d, c);
x += (upper_bound - low_bound) / size;
}

// Sum trapezoid areas between neighboring sample points.
for (int i = 0; i < size - 1; i++)
result += ((upper_bound - low_bound) / size) * ((array[i] + array[i + 1]) / 2);

return result;
}

// Approximates the definite integral using left-endpoint rectangles.

float rectangles(int form, float w, float a, float b, float d, float c, float low_bound, float upper_bound) {
float result = 0, x = low_bound;
const int size = 10000;
float array[size];

// Sample function values at left endpoints.
for (int i = 0; i < size; i++) {
array[i] = f(x, form, w, a, b, d, c);
x += (upper_bound - low_bound) / size;
}

// Sum rectangle areas.
for (int i = 0; i < size - 1; i++)
result += ((upper_bound - low_bound) / size) * (array[i]);

return result;
}

// Approximates the definite integral using Simpson's (parabolic) rule.

float parabola(int form, float w, float a, float b, float d, float c, float low_bound, float upper_bound) {
float result = 0, x = low_bound;
const int size = 10000;
float array[size];

// Sample function values used by Simpson weights.
for (int i = 0; i < size; i++) {
array[i] = f(x, form, w, a, b, d, c);
x += (upper_bound - low_bound) / size;
}

float h = (upper_bound - low_bound) / size;

// Apply Simpson coefficients: 1,4,2,4,...,2,1.
for (int i = 0; i < size - 1; i++) {
if (i == 0 or i == size - 1)
result += (h / 3) * array[i];
else if (i % 2 == 1)
result += (h / 3) * 4 * array[i];
else if (i % 2 == 0)
result += (h / 3) * 2 * array[i];
}

return result;
}

// Evaluates the selected function form at x using provided coefficients.

float f(float x, int form, float w, float a, float b, float d, float c) {
float y;

// Evaluate selected function family at x.
if (form == 1)
y = b * x + c;
else if (form == 2)
y = a * x * x + b * x + c;
else if (form == 3)
y = a * cos(w * x + b) + c;
else if (form == 4)
y = a * sin(w * x + b) + c;
else if (form == 5)
y = a * exp(b * x) + c;
else if (form == 6)
y = a * log(b * x + d) + c;

return y;
}

// Routes coefficient input to the proper overloaded function based on form.

void get_Coefficients(int form, float& w, float& a, float& b, float& d, float& c, float low_bound, float upper_bound) {

if (form == 1)
coefficients(form, b, c);
else if (form == 2 or form == 5)
coefficients(form, a, b, c);
else if (form == 3 or form == 4)
coefficients(form, a, w, b, c);
else if (form == 6)
coefficients(form, a, b, d, c, low_bound, upper_bound);
}

// Reads linear coefficients m and b (stored as b and c parameters here).

void coefficients(int form, float& b, float& c) {

cout << "\nYou Chose 1). y = mx + b\n";

cout << "\nEnter the slope (m): ";
cin >> b;
cout << "\nEnter the y-intercept (b): ";
cin >> c;
}

// Reads coefficients for quadratic or exponential forms.

void coefficients(int form, float& a, float& b, float& c) {

if (form == 2) {
cout << "\nYou Chose 2). y = ax^2 + bx + c\n";
cout << "\nEnter the first coefficient (a): ";
}
else if (form == 5) {
cout << "\nYou Chose 5). y = A e^(bx) + c\n";
cout << "\nEnter the first coefficient (A): ";
}

cin >> a;
cout << "\nEnter the second coefficient (b): ";
cin >> b;
cout << "\nEnter the third coefficient (c): ";
cin >> c;
}

// Reads coefficients for cosine/sine forms: A, w, phase b, and offset c.

void coefficients(int form, float& A, float& w, float& b, float& c) {

if (form == 3)
cout << "\nYou Chose 3). y = A cos(wx + b) + c\n";
else if (form == 4)
cout << "\nYou Chose 4). y = A sin(wx + b) + c\n";

cout << "\nEnter the amplitude (A): ";
cin >> A;
cout << "\nEnter the angular frequency (w): ";
cin >> w;
cout << "\nEnter the phase angle (b): ";
cin >> b;
cout << "\nEnter the coefficient (c): ";
cin >> c;
}

// Reads logarithmic coefficients and validates log-domain at both bounds.
// Re-prompts recursively if bx + d is non-positive at either bound.

void coefficients(int form, float& a, float& b, float& d, float& c, float low_bound, float upper_bound) {

cout << "\nYou Chose 6). y = A ln(bx + d) + c\n";
cout << "\nEnter the first coefficient (A): ";
cin >> a;
cout << "\nEnter the second coefficient (b): ";
cin >> b;
cout << "\nEnter the third coefficient (d): ";
cin >> d;
cout << "\nEnter the fourth coefficient (c): ";
cin >> c;

if (b * low_bound + d <= 0 or b * upper_bound + d <= 0) {
cout << "\nInvalid input, please try again.\n";
coefficients(form, a, b, d, c, low_bound, upper_bound);
}
}

// Displays final result for single-method mode.
// For compare mode (method 4), detailed values are printed elsewhere.

void result_displayer(float result, int method) {
if (method != 4)
cout << "\nThe definite integral approximately equals: " << result;
}

// Prompts user to restart and repeats the full program when requested.

void restart() {
char restart_choice;
cout << "\n\nEnter R to restart, or enter 1 to exit: ";
cin >> restart_choice;

if (restart_choice == 'r' or restart_choice == 'R') {
integral_program();
restart();
}
}




