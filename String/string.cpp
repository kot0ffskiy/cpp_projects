#include "string.hpp"

String::String() {
  capacity_++;
  str_ = new char[capacity_];
  *str_ = '\0';
}

String::String(size_t size, char character) {
  size_ = size;
  capacity_ += size_;
  str_ = new char[capacity_];
  for (size_t i = 0; i < size; ++i) {
    str_[i] = character;
  }
  str_[size_] = '\0';
}

String::String(const char* k_inp) {
  size_ = strlen(k_inp);
  capacity_ += size_;
  str_ = new char[capacity_];
  memcpy(str_, k_inp, size_ + 1);
}
String::String(const String& k_input) {
  size_ = k_input.size_;
  capacity_ = k_input.capacity_;
  str_ = new char[capacity_];
  memcpy(str_, k_input.str_, size_);
}

String& String::operator=(const String& k_input) {
  String newstr = k_input;
  Swap(newstr);
  return *this;
}

void String::Swap(String& other) {
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
  std::swap(str_, other.str_);
}

void String::Clear() {
  size_ = 0;
  *str_ = '\0';
}

void String::PushBack(char character) {
  size_++;
  if (capacity_ <= size_) {
    while (capacity_ <= size_) {
      capacity_ *= 2;
    }
    char* newstr = new char[capacity_];
    memcpy(newstr, str_, size_);
    newstr[size_ - 1] =
        character;  // size_ is the last symbol in string (size_ + 1 is '\0')
    newstr[size_] = '\0';
    delete[] str_;
    str_ = newstr;
  } else {
    str_[size_ - 1] = character;
    str_[size_] = '\0';
  }
}

void String::PopBack() {
  if (size_ == 0) {
    return;
  }
  str_[size_--] = '\0';
}

void String::Resize(size_t new_size) {
  if (size_ >= new_size) {
    size_ = new_size;
    str_[size_] = '\0';
  } else {
    size_t new_cap = capacity_;
    while (new_cap <= new_size) {
      new_cap *= 2;
    }
    Reserve(new_cap);
    size_ = new_size;
    str_[size_] = '\0';
  }
}

void String::Resize(size_t new_size, char character) {
  if (size_ >= new_size) {
    size_ = new_size;
    str_[size_] = '\0';
  } else {
    size_t new_cap = capacity_;
    while (new_cap <= new_size) {
      new_cap *= 2;
    }
    Reserve(new_cap);
    while (size_ < new_size) {
      str_[size_] = character;
      size_++;
    }
    str_[size_] = '\0';
  }
}

void String::Reserve(size_t new_cap) {
  if (capacity_ < new_cap) {
    capacity_ = new_cap;
    char* newstr = new char[capacity_];
    memcpy(newstr, str_, size_);
    delete[] str_;
    str_ = newstr;
  }
}

void String::ShrinkToFit() { capacity_ = size_; }

bool String::Empty() const { return size_ == 0; }

size_t String::Size() const { return size_; }

size_t String::Capacity() const { return capacity_; }

const char* String::Data() const { return str_; }

char* String::Data() { return str_; }

char& String::operator[](size_t index) { return str_[index]; }

const char& String::operator[](size_t index) const { return str_[index]; }

char& String::Front() { return *str_; }

char String::Front() const { return *str_; }

char& String::Back() { return str_[size_ - 1]; }

char String::Back() const { return str_[size_ - 1]; }

bool String::operator==(const String& k_other) const {
  if (size_ == k_other.size_) {
    for (size_t i = 0; i < size_; ++i) {
      if (str_[i] != k_other.str_[i]) {
        return false;
      }
    }
    return true;
  }
  return false;
}

bool String::operator!=(const String& k_other) const {
  return !(*this == k_other);
}

String& String::operator+=(const String& k_other) {
  String* copy = this;
  auto tmp = size_;
  copy->Resize(size_ + k_other.size_);
  memcpy(copy->str_ + tmp, k_other.str_, k_other.size_);
  return *copy;
}

String String::operator+(const String& k_other) const {
  String copy = *this;
  copy += k_other;
  return copy;
}

String& String::operator*=(size_t count) {
  String tmp = *this;
  *this = "";
  for (size_t i = 0; i < count; ++i) {
    *this += tmp;
  }
  return *this;
}

String String::operator*(size_t count) const {
  String tmp = *this;
  return tmp *= count;
}

bool String::operator<(const String& k_other) const {
  size_t iter = std::min(size_, k_other.size_);
  for (size_t i = 0; i < iter; ++i) {
    if (str_[i] == k_other.str_[i]) {
      continue;
    }
    if (str_[i] < k_other.str_[i]) {
      return str_[i] < k_other.str_[i];
    }
  }
  return size_ < k_other.size_;
}

bool String::operator>=(const String& k_other) const {
  return !(*this < k_other);
}

bool String::operator<=(const String& k_other) const {
  return !(*this > k_other);
}

bool String::operator>(const String& k_other) const {
  return !(*this < k_other || *this == k_other);
}

std::ostream& operator<<(std::ostream& os, const String& k_other) {
  for (size_t i = 0; i < k_other.size_; ++i) {
    os << k_other.str_[i];
  }
  return os;
}

std::istream& operator>>(std::istream& in, String& other) {
  char symbol;
  while (in >> symbol) {
    other.PushBack(symbol);
  }
  return in;
}

std::vector<String> String::Split(const String& k_delim) {
  std::vector<String> ans;
  size_t iter = 0;
  size_t delta;
  size_t start = 0;
  const char kFirst = k_delim.Front();
  while (str_[iter] != '\0') {
    if (str_[iter] == kFirst) {
      bool suitable = true;
      for (delta = 0; delta < k_delim.size_; ++delta) {
        if (str_[delta] == '\0') {
          break;
        }
        if (str_[delta + iter] != k_delim[delta]) {
          suitable = false;
          break;
        }
      }
      if (suitable) {
        Copy(ans, start, iter);
        iter += delta;
        start = iter;
        continue;
      }
    }
    iter++;
  }
  Copy(ans, start, iter);
  return ans;
}

String::~String() { delete[] str_; }

String String::Join(const std::vector<String>& k_strings) const {
  String newstr;
  for (size_t i = 0; i < k_strings.size(); ++i) {
    newstr += k_strings[i];
    if (i < k_strings.size() - 1) {
      newstr += *this;
    }
  }
  return newstr;
}

void String::Copy(std::vector<String>& ret, size_t start, size_t iter) {
  String copy;
  for (size_t k = start; k < iter; ++k) {
    copy.PushBack(str_[k]);
  }
  ret.push_back(copy);
}

String operator+(const String& k_left, const String& k_right) {
  String copy = k_left;
  copy += k_right;
  return copy;
}
