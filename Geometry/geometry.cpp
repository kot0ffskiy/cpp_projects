#include "geometry.hpp"

Vector::Vector() : coord_x_(0), coord_y_(0) {}
Vector::Vector(int64_t coord_x, int64_t coord_y)
    : coord_x_(coord_x), coord_y_(coord_y) {}
int64_t Vector::GetX() const { return coord_x_; }
int64_t Vector::GetY() const { return coord_y_; }
Vector& Vector::operator*=(int64_t other) {
  this->coord_x_ *= other;
  this->coord_y_ *= other;
  return *this;
}
Vector::Vector(const Point& k_first, const Point& k_last)
    : coord_x_(k_last.GetX() - k_first.GetX()),
      coord_y_(k_last.GetY() - k_first.GetY()) {}
Vector::Vector(const Point& k_first)
    : coord_x_(k_first.GetX()), coord_y_(k_first.GetY()) {}

Vector Vector::operator*(int64_t other) const {
  Vector result(this->GetX(), this->GetY());
  result *= other;
  return result;
}

Vector operator*(int64_t other, const Vector& k_vec) { return k_vec * other; };

Vector& Vector::operator+=(const Vector& k_other) {
  this->coord_x_ += k_other.coord_x_;
  this->coord_y_ += k_other.coord_y_;
  return *this;
}

Vector Vector::operator+(const Vector& k_other) const {
  Vector result = *this;
  result += k_other;
  return result;
}

Vector& Vector::operator-=(const Vector& k_other) {
  this->coord_x_ -= k_other.coord_x_;
  this->coord_y_ -= k_other.coord_y_;
  return *this;
}

Vector Vector::operator-(const Vector& k_other) const {
  Vector result = *this;
  result -= k_other;
  return result;
}

int64_t Vector::operator*(const Vector& k_other) const {
  return this->coord_x_ * k_other.coord_x_ + this->coord_y_ * k_other.coord_y_;
}

Vector Vector::operator-() const {
  return Vector(-this->coord_x_, -this->coord_y_);
};

int64_t Vector::operator^(const Vector& k_other) const {
  return this->coord_x_ * k_other.coord_y_ - this->coord_y_ * k_other.coord_x_;
}
double Vector::GetLength() const {
  return sqrt(this->coord_x_ * this->coord_x_ +
              this->coord_y_ * this->coord_y_);
}

Point::Point() : coord_x_(0), coord_y_(0) {}
Point::Point(int64_t coord_x, int64_t coord_y)
    : coord_x_(coord_x), coord_y_(coord_y) {}
int64_t Point::GetX() const { return coord_x_; }
int64_t Point::GetY() const { return coord_y_; }
void Point::Move(const Vector& k_other) {
  this->coord_x_ += k_other.GetX();
  this->coord_y_ += k_other.GetY();
}

bool Point::ContainsPoint(const Point& k_other) const {
  return this->coord_x_ == k_other.coord_x_ &&
         this->coord_y_ == k_other.coord_y_;
}

bool Point::CrossSegment(const Segment& k_other) const {
  return k_other.ContainsPoint(*this);
}

Point Point::operator-(const Point& k_right) const {
  return Point(this->coord_x_ - k_right.coord_x_,
               this->coord_y_ - k_right.coord_y_);
}

bool Point::operator==(const Point& k_right) const {
  return this->coord_x_ == k_right.coord_x_ &&
         this->coord_y_ == k_right.coord_y_;
}

Segment::Segment() {
  this->first_ = Point();
  this->last_ = Point();
}
Segment::Segment(const Point& k_first, const Point& k_last)
    : first_(k_first), last_(k_last) {}

Point Segment::GetA() const { return first_; }
Point Segment::GetB() const { return last_; }
void Segment::Move(const Vector& k_other) {
  this->first_.Move(k_other);
  this->last_.Move(k_other);
}

bool Segment::ContainsPoint(const Point& k_other) const {
  Vector left_point(k_other, this->first_);
  Vector left_right(this->last_, this->first_);
  return ((left_point ^ left_right) == 0 && (left_point * left_right) >= 0 &&
          (left_point * left_right) < (left_right * left_right)) ||
         (k_other == this->last_ || k_other == this->first_);
}

bool Segment::CrossSegment(const Segment& k_other) const {
  Vector left_first_ca(k_other.GetA(), this->first_);
  Vector left_last_cb(k_other.GetB(), this->first_);
  Vector right_first_cd(this->last_, this->first_);
  Vector right_last_ab(k_other.GetB(), k_other.GetA());
  Vector right_first_ac = -left_first_ca;
  Vector right_mid_ad(this->last_, k_other.GetA());
  if ((right_last_ab ^ right_first_ac) * (right_last_ab ^ right_mid_ad) == 0 &&
      (right_first_cd ^ left_first_ca) * (right_first_cd ^ left_last_cb) == 0) {
    return this->ContainsPoint(k_other.GetA()) ||
           this->ContainsPoint(k_other.GetB()) ||
           k_other.ContainsPoint(this->first_) ||
           k_other.ContainsPoint(this->last_);
  }
  return (
      (right_last_ab ^ right_first_ac) * (right_last_ab ^ right_mid_ad) <= 0 &&
      (right_first_cd ^ left_first_ca) * (right_first_cd ^ left_last_cb) <= 0);
}

Line::Line() : a_koef_(0), b_koef_(0), c_koef_(0) {}
Line::Line(const Point& k_first, const Point& k_last) {
  this->a_koef_ = k_first.GetY() - k_last.GetY();
  this->b_koef_ = k_last.GetX() - k_first.GetX();
  this->c_koef_ =
      k_first.GetX() * k_last.GetY() - k_last.GetX() * k_first.GetY();
}
int64_t Line::GetA() const { return a_koef_; }
int64_t Line::GetB() const { return b_koef_; }
int64_t Line::GetC() const { return c_koef_; }
void Line::Move(const Vector& k_other) {
  this->c_koef_ -=
      k_other.GetX() * this->a_koef_ + k_other.GetY() * this->b_koef_;
}

bool Line::ContainsPoint(const Point& k_other) const {
  return this->a_koef_ * k_other.GetX() + this->b_koef_ * k_other.GetY() +
             this->c_koef_ ==
         0;
}

bool Line::CrossSegment(const Segment& k_other) const {
  return this->a_koef_ * k_other.GetA().GetX() +
                 this->b_koef_ * k_other.GetA().GetY() + this->c_koef_ ==
             0 ||
         this->a_koef_ * k_other.GetB().GetX() +
                 this->b_koef_ * k_other.GetB().GetY() + this->c_koef_ ==
             0 ||
         (this->a_koef_ * k_other.GetA().GetX() +
                  this->b_koef_ * k_other.GetA().GetY() + this->c_koef_ <=
              0 &&
          this->a_koef_ * k_other.GetB().GetX() +
                  this->b_koef_ * k_other.GetB().GetY() + this->c_koef_ >=
              0) ||
         (this->a_koef_ * k_other.GetA().GetX() +
                  this->b_koef_ * k_other.GetA().GetY() + this->c_koef_ >=
              0 &&
          this->a_koef_ * k_other.GetB().GetX() +
                  this->b_koef_ * k_other.GetB().GetY() + this->c_koef_ <=
              0);
}

[[nodiscard]] Segment* Segment::Clone() { return new Segment(*this); }
[[nodiscard]] Point* Point::Clone() { return new Point(*this); }
[[nodiscard]] Line* Line::Clone() { return new Line(*this); }
[[nodiscard]] Ray* Ray::Clone() { return new Ray(*this); }

Ray::Ray() {
  this->first_ = Point();
  this->direction_ = Vector();
}
Ray::Ray(const Point& k_first, const Point& k_direction)
    : first_(k_first), direction_(Vector(k_first, k_direction)) {}

Point Ray::GetA() const { return first_; }
Vector Ray::GetVector() const { return direction_; }
void Ray::Move(const Vector& k_other) { this->first_.Move(k_other); }
bool Ray::ContainsPoint(const Point& k_other) const {
  Vector left_point(this->first_, k_other);
  return (left_point ^ this->direction_) == 0 &&
         (left_point * this->direction_) >= 0;
}
bool Ray::CrossSegment(const Segment& k_other) const {
  Line check =
      Line(this->first_, Point(this->first_.GetX() + this->direction_.GetX(),
                               this->first_.GetY() + this->direction_.GetY()));
  if (check.CrossSegment(k_other)) {
    Vector vec_ab(k_other.GetA(), k_other.GetB());
    Vector vec_firsta(this->first_, k_other.GetA());
    return ((vec_firsta ^ vec_ab) > 0 && (vec_ab ^ this->direction_) < 0 ||
            (vec_firsta ^ vec_ab) < 0 && (vec_ab ^ this->direction_) > 0) ||
           (vec_firsta ^ vec_ab) == 0 || (vec_ab ^ this->direction_) == 0;
  }
  return false;
}

Circle::Circle() {
  this->center_ = Point();
  this->radius_ = 0;
}
Circle::Circle(const Point& k_center, size_t radius)
    : center_(k_center), radius_(radius) {}
Point Circle::GetCentre() const { return center_; }
size_t Circle::GetRadius() const { return radius_; }
void Circle::Move(const Vector& k_other) { this->center_.Move(k_other); }
bool Circle::ContainsPoint(const Point& k_other) const {
  return static_cast<size_t>((k_other.GetX() - this->center_.GetX()) *
                                 (k_other.GetX() - this->center_.GetX()) +
                             (k_other.GetY() - this->center_.GetY()) *
                                 (k_other.GetY() - this->center_.GetY())) <=
         this->radius_ * this->radius_;
}
bool Circle::CrossSegment(const Segment& k_other) const {
  if ((this->ContainsPoint(k_other.GetA()) &&
       !this->ContainsPoint(k_other.GetB())) ||
      (this->ContainsPoint(k_other.GetB()) &&
       !this->ContainsPoint(k_other.GetA())) ||
      (static_cast<size_t>(
           (k_other.GetA().GetX() - this->center_.GetX()) *
               (k_other.GetA().GetX() - this->center_.GetX()) +
           (k_other.GetA().GetY() - this->center_.GetY()) *
               (k_other.GetA().GetY() - this->center_.GetY())) ==
       this->radius_ * this->radius_) ||
      (static_cast<size_t>(
           (k_other.GetB().GetX() - this->center_.GetX()) *
               (k_other.GetB().GetX() - this->center_.GetX()) +
           (k_other.GetB().GetY() - this->center_.GetY()) *
               (k_other.GetB().GetY() - this->center_.GetY())) ==
       this->radius_ * this->radius_)) {
    return true;
  }
  if (this->ContainsPoint(k_other.GetA()) &&
      this->ContainsPoint(k_other.GetB())) {
    return false;
  }
  // Строим прямую по отрезку
  Line check(k_other.GetA(), k_other.GetB());
  int64_t koef_a = check.GetA();
  int64_t koef_b = check.GetB();
  int64_t koef_c = check.GetC();
  int64_t koef_x0 = this->center_.GetX();
  int64_t koef_y0 = this->center_.GetY();
  size_t radius = this->radius_;
  return (static_cast<size_t>((koef_a * koef_c + koef_y0 * koef_a * koef_b +
                               koef_x0 * koef_a * koef_a) *
                                  (koef_a * koef_c + koef_y0 * koef_a * koef_b +
                                   koef_x0 * koef_a * koef_a) +
                              (koef_b * koef_c + koef_a * koef_b * koef_x0 +
                               koef_y0 * koef_b * koef_b) *
                                  (koef_b * koef_c + koef_a * koef_b * koef_x0 +
                                   koef_y0 * koef_b * koef_b)) <=
          radius * radius * (koef_a * koef_a + koef_b * koef_b) *
              (koef_a * koef_a + koef_b * koef_b));
}
[[nodiscard]] Circle* Circle::Clone() { return new Circle(*this); }
