
void integral_program();

void welcome_function();

int get_form();

void get_bounds(float& low_bound, float& upper_bound);

void get_Coefficients(int form, float& w, float& a, float& b, float& d, float& c, float low_bound, float upper_bound); // Coefficients for any function

void coefficients(int form, float& b, float& c); // Coefficients for linear
void coefficients(int form, float& a, float& b, float& c); // Coefficients for quadratic or exponential
void coefficients(int form, float& A, float& w, float& b, float& c); // Coefficients for sinosoids
void coefficients(int form, float& a, float& b, float& d, float& c, float low_bound, float upper_bound); // Coefficients for log

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
int main() {
float result;
welcome_function();
integral_program();
restart();
return 0;
}

// Inputs: None
// Outputs: Result of a definite integral approximation
// Purpose: Prompts user for inputs and approximates a definite integral
// using a method selected by the user.

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

void welcome_function() {
cout << "\n\tDefinite Integral Approximator\n";
}

// Inputs: None
// Outputs: an integer stored in variable form
// Purpose: prompts the user to enter a number, each of which corresponds to
// a form of function. It stores this number in variable "form"

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

// Inputs: low_bound and upper_bound, pass by reference
// Outputs: Void, pass by reference
// Purpose: Promts user to enter bounds of integration and stores in variables

void get_bounds(float& low_bound, float& upper_bound) {
cout << "\nEnter Lower Bound: ";
cin >> low_bound;
cout << "\nEnter Upper Bound: ";
cin >> upper_bound;
}

// Inputs: None
// Outputs: method
// Purpose: prompts the user to enter a number, each of which corresponds to
// a different method of approximation. it returns this number.

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

// Inputs:
// Outputs:
// Purpose:

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

return result;
}

// Inputs:
// Outputs:
// Purpose:

void compare_methods(int form, float w, float a, float b, float d, float c, float low_bound, float upper_bound) {
cout << "\nTrapazoid Approximation: " << trapazoids(form, w, a, b, d, c, low_bound, upper_bound);
cout << "\nRectangular Approximation: " << rectangles(form, w, a, b, d, c, low_bound, upper_bound);
cout << "\nParabolic Approximation: " << parabola(form, w, a, b, d, c, low_bound, upper_bound);
}

// Inputs: form, w, a, b, c, low_bound, and upper_bound
// Outputs: returns the result of a trapazoidal approximation
// Purpose: It inputs the form of the function, along with it's coefficients
// and the lower and upper bounds of integration, and approximates the definite
// integral using trapazoids.

float trapazoids(int form, float w, float a, float b, float d, float c, float low_bound, float upper_bound) {
float result = 0, x = low_bound;
const int size = 10000;
float array[size];

for (int i = 0; i < size; i++) {
array[i] = f(x, form, w, a, b, d, c);
x += (upper_bound - low_bound) / size;
}

for (int i = 0; i < size - 1; i++)
result += ((upper_bound - low_bound) / size) * ((array[i] + array[i + 1]) / 2);

return result;
}

// Inputs: form, w, a, b, c, low_bound, and upper_bound
// Outputs: returns the result of a rectangular approximation
// Purpose: It inputs the form of the function, along with it's coefficients
// and the lower and upper bounds of integration, and approximates the definite
// integral using rectangles.

float rectangles(int form, float w, float a, float b, float d, float c, float low_bound, float upper_bound) {
float result = 0, x = low_bound;
const int size = 10000;
float array[size];

for (int i = 0; i < size; i++) {
array[i] = f(x, form, w, a, b, d, c);
x += (upper_bound - low_bound) / size;
}

for (int i = 0; i < size - 1; i++)
result += ((upper_bound - low_bound) / size) * (array[i]);

return result;
}

// Inputs: form, w, a, b, c, low_bound, and upper_bound
// Outputs: returns the result of a parapolic approximation
// Purpose: It inputs the form of the function, along with it's coefficients
// and the lower and upper bounds of integration, and approximates the definite
// integral using parabolas (Simpson's Rule)

float parabola(int form, float w, float a, float b, float d, float c, float low_bound, float upper_bound) {
float result = 0, x = low_bound;
const int size = 10000;
float array[size];

for (int i = 0; i < size; i++) {
array[i] = f(x, form, w, a, b, d, c);
x += (upper_bound - low_bound) / size;
}

float h = (upper_bound - low_bound) / size;

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

// Inputs: x, form, w, a, b, c. basically, the form, a variable x, and all the
// cofficients that might possibly be used for this program
// Outputs: y
// Purpose: It inputs the form of the function, along with it's coefficients
// and a point x. It evaluates the function at x and returns that value.

float f(float x, int form, float w, float a, float b, float d, float c) {
float y;

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

// Inputs: Form, and coefficients w, a, b, c.
// Outputs: Void, pass by reference.
// Purpose: Based on previous user inputs, it directs the program to the correct
// get_coefficient function, and stores the user inputted values using pass by
// reference.

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

// Inputs: Form, and coefficients b, the slophttps://www.onlinegdb.com/fork/L99xHsL4H#tab-stdine, and c, the y-intercept.
// Outputs: Void, pass by reference.
// Purpose: Promts to user to input the coefficients for their linear function

void coefficients(int form, float& b, float& c) {

cout << "\nYou Chose 1). y = mx + b\n";

cout << "\nEnter the slope (m): ";
cin >> b;
cout << "\nEnter the y-intercept (b): ";
cin >> c;
}

// Inputs: Form, and coefficients a, b, and c.
// Outputs: Void, pass by reference.
// Purpose: Promts to user to input the coefficients for their quadratic function

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

// Inputs: Form, and coefficients A, w, b, and c.
// Outputs: Void, pass by reference.
// Purpose: Promts to user to input the coefficients for their sinosoidal function

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

// Inputs: Form, and coefficients a, b, d, and c.
// Outputs: Void, pass by reference.
// Purpose: Promts to user to input the coefficients for their natural log function.
// It also checks for discontinuities

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

// Inputs: Result
// Outputs: Void, outputs to user
// Purpose: Takes the result of the integral approximation and displays it to user

void result_displayer(float result, int method) {
if (method != 4)
cout << "\nThe definite integral approximately equals: " << result;
}

// Inputs: None
// Outputs: Void, displays to user
// Purpose: Asks user if they want to restart
// and if so it starts the integral program again

void restart() {
char restart_choice;
cout << "\n\nEnter R to restart, or enter 1 to exit: ";
cin >> restart_choice;

if (restart_choice == 'r' or restart_choice == 'R') {
integral_program();
restart();
}
}




