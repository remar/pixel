#ifndef POINT_H
#define POINT_H

struct Point {
  int x;
  int y;

  bool operator <(const Point& pt) const {
    return (x < pt.x) || ((!(pt.x < x)) && (y < pt.y));
  }
};

#endif
