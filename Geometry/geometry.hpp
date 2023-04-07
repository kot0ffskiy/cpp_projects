#include <cmath>
#include <iostream>

class IShape;
class Point;
class Segment;
class Line;

class Vector {
 private:
  int64_t coord_x_;
  int64_t coord_y_;

 public:
  Vector();
  Vector(int64_t coord_x, int64_t coord_y);
  Vector(const Point& k_first, const Point& k_last);
  Vector(const Point& k_first);
  int64_t GetX() const;
  int64_t GetY() const;
  Vector& operator*=(int64_t k_other);
  Vector operator*(int64_t k_other) const;
  Vector& operator+=(const Vector& k_other);
  Vector operator+(const Vector& k_other) const;
  Vector& operator-=(const Vector& k_other);
  Vector operator-(const Vector& k_other) const;
  int64_t operator*(const Vector& k_other) const;
  Vector operator-() const;
  int64_t operator^(const Vector& k_other) const;
  double GetLength() const;
};

class IShape {
 public:
  virtual void Move(const Vector& k_other) = 0;
  virtual bool ContainsPoint(const Point& k_other) const = 0;
  virtual bool CrossSegment(const Segment& k_other) const = 0;
  virtual IShape* Clone() = 0;
  virtual ~IShape() = default;
};

class Point : public IShape {
 private:
  int64_t coord_x_;
  int64_t coord_y_;

 public:
  Point();
  Point(int64_t coord_x, int64_t coord_y);
  int64_t GetX() const;
  int64_t GetY() const;
  void Move(const Vector& k_other) override;
  bool ContainsPoint(const Point& k_other) const override;
  bool CrossSegment(const Segment& k_other) const override;
  Point operator-(const Point& k_right) const;
  bool operator==(const Point& k_right) const;
  Point* Clone() override;
};

class Segment : public IShape {
 private:
  Point first_;
  Point last_;

 public:
  Segment();
  Segment(const Point& k_first, const Point& k_last);
  Point GetA() const;
  Point GetB() const;
  void Move(const Vector& k_other) override;
  bool ContainsPoint(const Point& k_other) const override;
  bool CrossSegment(const Segment& k_other) const override;
  Segment* Clone() override;
};

class Line : public IShape {
 private:
  int64_t a_koef_;
  int64_t b_koef_;
  int64_t c_koef_;

 public:
  Line();
  Line(const Point& k_first, const Point& k_last);
  int64_t GetA() const;
  int64_t GetB() const;
  int64_t GetC() const;
  void Move(const Vector& k_other) override;
  bool ContainsPoint(const Point& k_other) const override;
  bool CrossSegment(const Segment& k_other) const override;
  Line* Clone() override;
};

class Ray : public IShape {
 private:
  Point first_;
  Vector direction_;

 public:
  Ray();
  Ray(const Point& k_first, const Point& k_direction);
  Point GetA() const;
  Vector GetVector() const;
  void Move(const Vector& k_other) override;
  bool ContainsPoint(const Point& k_other) const override;
  bool CrossSegment(const Segment& k_other) const override;
  Ray* Clone() override;
};

class Circle : public IShape {
 private:
  Point center_;
  size_t radius_;

 public:
  Circle();
  Circle(const Point& k_center, size_t k_radius);
  Point GetCentre() const;
  size_t GetRadius() const;
  void Move(const Vector& k_other) override;
  bool ContainsPoint(const Point& k_other) const override;
  bool CrossSegment(const Segment& k_other) const override;
  Circle* Clone() override;
};