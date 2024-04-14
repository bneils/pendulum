#pragma once

class Vector {
public:
  Vector(double x, double y);
  double get_x();
  double get_y();
  Vector scale(double c);
  Vector add(Vector v);
  Vector sub(Vector v);
  double magnitude();
  Vector normalize();
private:
  double x, y;
};
