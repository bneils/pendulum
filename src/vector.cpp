#include <math.h>

#include "vector.hpp"

Vector::Vector(double x, double y) : x(x), y(y) {}
double Vector::get_x() { return x; }
double Vector::get_y() { return y; }
Vector Vector::scale(double c) {
  Vector resultant(x * c, y * c);
  return resultant;
}

Vector Vector::add(Vector v) {
  Vector resultant(x + v.get_x(), y + v.get_y());
  return resultant;
}

Vector Vector::sub(Vector v) {
  Vector resultant(x - v.get_x(), y - v.get_y());
  return resultant;
}

double Vector::magnitude() {
  return sqrt(x * x + y * y);
}

Vector Vector::normalize() {
  double mag = magnitude();
  Vector resultant(x / mag, y / mag);
  return resultant;
}
