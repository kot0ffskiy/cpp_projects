#include "big_integer.hpp"

std::string CheckForZero(std::string input) {
  std::string output;
  if (input[0] == '0') {
    size_t pos = input.find_first_not_of('0');
    if (pos > input.size()) {
      output.push_back('0');
    } else {
      output = input.substr(pos);
    }
  } else {
    output = input;
  }
  return output;
}
BigInt::BigInt() { number_ = ""; }
BigInt::BigInt(const BigInt& k_input) {
  if (k_input.znak_) {
    znak_ = true;
  }
  number_ = k_input.number_;
}
BigInt::BigInt(const std::string& k_input) {
  if (k_input[0] == '-') {
    znak_ = true;
    std::string proto = k_input.substr(1);
    number_ = CheckForZero(proto);
  } else {
    number_ = CheckForZero(k_input);
  }
  if (number_[0] == '0') {
    znak_ = znak_ ^ znak_;
  }
}
BigInt::BigInt(int64_t input) {
  if (input < 0) {
    znak_ = true;
    if (input == -input) {
      number_ = std::to_string((uint64_t)input);
    } else {
      number_ = std::to_string(-input);
    }
  } else {
    number_ = std::to_string(input);
  }
}
BigInt& BigInt::operator=(const BigInt& k_other) = default;
bool BigInt::operator==(const BigInt& k_other) const {
  return this->znak_ == k_other.znak_ && this->number_ == k_other.number_;
}
bool BigInt::operator<(const BigInt& k_other) const {
  if (znak_ == k_other.znak_ && znak_) {
    if (number_.length() < k_other.number_.length()) {
      return false;
    }
    if ((number_.length() > k_other.number_.length())) {
      return true;
    }
    return number_ < k_other.number_;
  }
  if (znak_ == k_other.znak_ && !znak_) {
    if (number_.length() < k_other.number_.length()) {
      return true;
    }
    if ((number_.length() > k_other.number_.length())) {
      return false;
    }
    return number_ < k_other.number_;
  }
  return znak_;  // -a < (+)b; a > (-)b
}
bool BigInt::operator>=(const BigInt& k_other) const {
  return !(*this < k_other);
}
bool BigInt::operator!=(const BigInt& k_other) const {
  return !(*this == k_other);
}
bool BigInt::operator>(const BigInt& k_other) const {
  return *this >= k_other && *this != k_other;
}
bool BigInt::operator<=(const BigInt& k_other) const {
  return !(*this > k_other);
}
BigInt& BigInt::operator+=(const BigInt& k_other) {
  BigInt copy = k_other;
  if (znak_ == k_other.znak_ && !znak_) {  // both pos
    return BothPos(copy);
  }
  if (znak_ == k_other.znak_ && znak_) {  // both neg
    return BothNeg(copy);
  }
  if (znak_ != k_other.znak_ && znak_) {  // -a + b
    return LeftNeg(copy);
  }
  if (znak_ != k_other.znak_ && !znak_) {  // a + (-b)
    return RightNeg(copy);
  }
  return *this;
}
BigInt& BigInt::operator-=(const BigInt& k_other) {
  BigInt copy = k_other;
  if (znak_ == k_other.znak_ && !znak_) {  // +a - +b
    return RightNeg(copy);
  }
  if (znak_ == k_other.znak_ && znak_) {  // -a - (-)b == -a + b
    copy.znak_ = false;
    return LeftNeg(copy);
  }
  if (znak_ != k_other.znak_ && !znak_) {  // a - (-)b == a + b
    return BothPos(copy);
  }
  if (znak_ != k_other.znak_ && znak_) {  // -a - +b == -a + (-)b
    return BothNeg(copy);
  }
  return *this;
}
BigInt BigInt::operator+(const BigInt& k_other) const {
  BigInt res = *this;
  return res += k_other;
}
BigInt BigInt::operator-(const BigInt& k_other) const {
  BigInt res = *this;
  return res -= k_other;
}
BigInt BigInt::operator-() {
  if (*this == (int64_t)0) {
    return *this;
  }
  znak_ ^= 1;
  return *this;
}
BigInt& BigInt::operator++() {
  *this += 1;
  return *this;
}
BigInt BigInt::operator++(int) {
  BigInt copy = *this;
  ++*this;
  return copy;
}
BigInt& BigInt::operator--() { return *this -= 1; }
BigInt BigInt::operator--(int) {
  BigInt copy = *this;
  --*this;
  return copy;
}
BigInt& BigInt::operator*=(const BigInt& k_other) {
  if (*this == 0 || k_other == 0) {
    *this = 0;
    return *this;
  }
  BigInt mul = k_other;
  bool sign = znak_ ^ mul.znak_;
  znak_ = false;
  mul.znak_ = false;
  const BigInt kTmp = *this;
  BigInt result = *this;
  BigInt iter = 2;
  BigInt last_iter = iter;
  *this = 0;
  while (mul > 0) {
    while (iter <= mul) {
      result += result;
      last_iter = iter;
      iter += iter;
    }
    *this += result;
    result = kTmp;
    mul -= last_iter;
    iter = 2;
  }
  this->znak_ = sign;
  return *this;
}
BigInt& BigInt::operator/=(const BigInt& k_other) {
  if (k_other == 1 || k_other == -1) {
    znak_ = znak_ ^ k_other.znak_;
    return *this;
  }
  BigInt copy = k_other;
  bool sign = znak_ ^ copy.znak_;
  znak_ = false;
  copy.znak_ = false;
  BigInt div = *this;
  *this = 0;
  Divide(div, copy);
  this->number_ = CheckForZero(this->number_);
  if (*this == 0) {
    this->znak_ = false;
  } else {
    this->znak_ = sign;
  }
  return *this;
}
BigInt& BigInt::operator%=(const BigInt& k_other) {
  if (k_other == 1 || k_other == -1) {
    znak_ = false;
    *this = 0;
    return *this;
  }
  *this = *this - (k_other * (*this / k_other));
  return *this;
}
BigInt BigInt::operator%(const BigInt& k_other) const {
  BigInt copy = *this;
  return copy %= k_other;
}
BigInt BigInt::operator/(const BigInt& k_other) const {
  BigInt res = *this;
  return res /= k_other;
}
BigInt BigInt::operator*(const BigInt& k_other) const {
  BigInt copy = *this;
  return copy *= k_other;
}
std::ostream& operator<<(std::ostream& os, const BigInt& k_other) {
  if (k_other.znak_ && k_other.number_ != std::to_string(0)) {
    std::string output = "-" + k_other.number_;
    os << output;
  } else {
    os << k_other.number_;
  }
  return os;
}
std::istream& operator>>(std::istream& in, BigInt& other) {
  std::string input;
  in >> input;
  other = input;
  return in;
}
void BigInt::Swap(BigInt& lhs, BigInt& rhs) {
  std::swap(lhs.number_, rhs.number_);
  std::swap(lhs.znak_, rhs.znak_);
}
BigInt::~BigInt() { number_.clear(); }

BigInt& BigInt::Minus(BigInt& lhs, const BigInt& k_rhs, bool sign) {
  int iter_num = static_cast<int>(lhs.number_.size());      // a
  int iter_other = static_cast<int>(k_rhs.number_.size());  // b
  std::string fixed_other(iter_num - iter_other, '0');
  fixed_other += k_rhs.number_;
  std::string answer;
  int ten = 0;
  const int kRemten = 10;
  while (--iter_num >= 0) {
    int int1 = static_cast<int>(lhs.number_[iter_num] - '0');
    int int2 = static_cast<int>(fixed_other[iter_num] - '0');
    int res = int1 - int2 - ten;
    res >= 0 ? ten = 0 : (ten = 1, res += kRemten);
    answer += std::to_string(res);
  }
  std::reverse(answer.begin(), answer.end());
  answer = CheckForZero(answer);
  lhs.number_ = answer;
  lhs.znak_ = sign;
  return lhs;
}
BigInt& BigInt::Plus(BigInt& lhs, const BigInt& k_rhs, bool sign) {
  int iter_num = static_cast<int>(lhs.number_.size());
  int iter_other = static_cast<int>(k_rhs.number_.size());
  std::string fixed_other(iter_num - iter_other, '0');
  fixed_other += k_rhs.number_;
  std::string answer;
  int ten = 0;
  const int kRemten = 10;
  while (--iter_num >= 0) {
    int int1 = static_cast<int>(lhs.number_[iter_num]) + ten - '0';
    int int2 = static_cast<int>(fixed_other[iter_num]) - '0';
    answer += std::to_string((int1 + int2) % kRemten);
    ten = (int1 + int2) / kRemten;
  }
  if (ten != 0) {
    answer += std::to_string(ten);
  }
  std::reverse(answer.begin(), answer.end());
  lhs.number_ = answer;
  lhs.znak_ = sign;
  return lhs;
}
BigInt& BigInt::Divide(BigInt& divisible, const BigInt& divider) {
  if (divisible - divider >= 0) {
    const int kMul = 10;
    divisible = Divide(divisible, divider * kMul);
  }
  char iter = '0';
  while (divisible - divider >= 0) {
    divisible -= divider;
    iter++;
  }
  this->number_ += iter;
  return divisible;
}
BigInt& BigInt::BothPos(BigInt& copy) {
  if (*this < copy) {
    Swap(*this, copy);
  }
  return Plus(*this, copy, false);
}
BigInt& BigInt::BothNeg(BigInt& copy) {
  if (*this > copy) {
    Swap(*this, copy);
  }
  return Plus(*this, copy, true);
}
BigInt& BigInt::LeftNeg(BigInt& copy) {
  bool sign = true;
  znak_ = false;  // just to compare 2 nums
  if (*this <= copy) {
    Swap(*this, copy);
    sign = false;
  }
  return Minus(*this, copy, sign);
}
BigInt& BigInt::RightNeg(BigInt& copy) {
  bool sign = false;
  copy.znak_ = false;  // just to compare 2 nums
  if (*this < copy) {
    Swap(*this, copy);
    sign = true;
  }
  return Minus(*this, copy, sign);
}