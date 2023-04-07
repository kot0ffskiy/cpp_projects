#pragma once
#include <algorithm>
#include <cstring>
#include <iostream>

class BigInt {
 public:
  BigInt();
  BigInt(const BigInt& k_input);
  BigInt(int64_t input);
  BigInt(const std::string& k_input);
  BigInt& operator=(const BigInt& k_other);
  bool operator==(const BigInt& k_other) const;
  bool operator<(const BigInt& k_other) const;
  bool operator>=(const BigInt& k_other) const;
  bool operator!=(const BigInt& k_other) const;
  bool operator>(const BigInt& k_other) const;
  bool operator<=(const BigInt& k_other) const;
  BigInt& operator+=(const BigInt& k_other);
  BigInt& operator-=(const BigInt& k_other);
  BigInt operator+(const BigInt& k_other) const;
  BigInt operator-(const BigInt& k_other) const;
  BigInt operator-();
  BigInt& operator++();
  BigInt operator++(int);
  BigInt& operator--();
  BigInt operator--(int);
  BigInt& operator*=(const BigInt& k_other);
  BigInt& operator/=(const BigInt& k_other);
  BigInt& operator%=(const BigInt& k_other);
  BigInt operator%(const BigInt& k_other) const;
  BigInt operator/(const BigInt& k_other) const;
  BigInt operator*(const BigInt& k_other) const;
  friend std::ostream& operator<<(std::ostream& os, const BigInt& k_other);
  friend std::istream& operator>>(std::istream& in, BigInt& other);
  static void Swap(BigInt& lhs, BigInt& rhs);
  ~BigInt();

 private:
  bool znak_ = false;
  std::string number_;
  static BigInt& Minus(BigInt& lhs, const BigInt& k_rhs, bool sign);
  static BigInt& Plus(BigInt& lhs, const BigInt& k_rhs, bool sign);
  BigInt& Divide(BigInt& divisible, const BigInt& divider);
  BigInt& BothPos(BigInt& copy);
  BigInt& BothNeg(BigInt& copy);
  BigInt& LeftNeg(BigInt& copy);
  BigInt& RightNeg(BigInt& copy);
};