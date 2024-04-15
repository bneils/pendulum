#include <math.h>

#include "vector.hpp"

Vector::Vector(double x, double y) : _x(x), _y(y) {}
double Vector::x() { return _x; }
double Vector::y() { return _y; }
Vector Vector::scale(double c) {
  Vector resultant(_x * c, _y * c);
  return resultant;
}

Vector Vector::add(Vector v) {
  Vector resultant(_x + v.x(), _y + v.y());
  return resultant;
}

Vector Vector::sub(Vector v) {
  Vector resultant(_x - v.x(), _y - v.y());
  return resultant;
}

double Vector::magnitude() {
  return sqrt(_x * _x + _y * _y);
}

Vector Vector::normalize() {
  double mag = magnitude();
  Vector resultant(_x / mag, _y / mag);
  return resultant;
}
