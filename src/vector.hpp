#pragma once

class Vector {
public:
  Vector(double x, double y);
  double x();
  double y();
  Vector scale(double c);
  Vector add(Vector v);
  Vector sub(Vector v);
  double magnitude();
  Vector normalize();
private:
  double _x, _y;
};
