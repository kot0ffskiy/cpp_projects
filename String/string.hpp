#include <cstring>
#include <iostream>
#include <vector>

#pragma once

class String {
 public:
  String();
  String(size_t size, char character);
  String(const char* k_inp);
  String(const String& k_input);
  String& operator=(const String& k_input);
  void Swap(String& other);
  void Clear();
  void PushBack(char character);
  void PopBack();
  void Resize(size_t new_size);
  void Resize(size_t new_size, char character);
  void Reserve(size_t new_cap);
  void ShrinkToFit();
  bool Empty() const;
  size_t Size() const;
  size_t Capacity() const;
  const char* Data() const;
  char* Data();
  char& operator[](size_t index);
  const char& operator[](size_t index) const;
  char& Front();
  char Front() const;
  char& Back();
  char Back() const;
  bool operator==(const String& k_other) const;
  bool operator!=(const String& k_other) const;
  String& operator+=(const String& k_other);
  String operator+(const String& k_other) const;
  String& operator*=(size_t count);
  String operator*(size_t count) const;
  bool operator<(const String& k_other) const;
  bool operator>=(const String& k_other) const;
  bool operator>(const String& k_other) const;
  bool operator<=(const String& k_other) const;

  friend std::ostream& operator<<(std::ostream& os, const String& k_other);
  friend std::istream& operator>>(std::istream& in, String& other);

  std::vector<String> Split(const String& k_delim = " ");

  String Join(const std::vector<String>& k_strings) const;

  ~String();

 private:
  void Copy(std::vector<String>& ret, size_t start, size_t iter);
  char* str_ = nullptr;
  size_t size_ = 0;
  size_t capacity_ = 1;
};
