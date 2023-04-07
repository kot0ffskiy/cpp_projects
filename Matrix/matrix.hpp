#pragma once

#include <algorithm>
#include <complex>
#include <iostream>
#include <random>
#include <vector>

template <size_t N, size_t M, class T = int64_t>
class Matrix {
 public:
  Matrix() : matrix_(std::vector(N, std::vector(std::vector<T>(M)))) {}
  Matrix(const Matrix& k_in) { matrix_ = k_in.matrix_; }
  Matrix(std::vector<std::vector<T>> input) { matrix_ = input; }
  Matrix(T elem)
      : matrix_(std::vector(N, std::vector(std::vector<T>(M, elem)))) {}

  T operator()(size_t row, size_t col) const;
  T& operator()(size_t row, size_t col);
  Matrix& operator+=(const Matrix& k_other);
  Matrix operator+(const Matrix& k_other);
  Matrix& operator-=(const Matrix& k_other);
  Matrix operator*(const T& k_mul);
  Matrix operator-(const Matrix& k_other);
  bool operator==(const Matrix& k_other);
  Matrix& operator=(Matrix k_other);
  // Matrix operator*(const T& k_mul, Matrix& k_other);
  Matrix<M, N, T> Transposed();
  T Trace();
  ~Matrix() = default;

 private:
  std::vector<std::vector<T>> matrix_;
};

template <size_t N, size_t M, size_t K, typename T = int64_t>
Matrix<M, N, T> operator*(const Matrix<M, K, T>& k_left,
                          const Matrix<K, N, T>& k_other) {
  Matrix<M, N, T> ret;
  for (size_t i = 0; i < M; ++i) {
    for (size_t j = 0; j < N; ++j) {
      T sum = 0;
      for (size_t k = 0; k < K; ++k) {
        sum += k_left(i, k) * k_other(k, j);
      }
      ret(i, j) = sum;
    }
  }
  return ret;
}

template <size_t N, size_t M, typename T>
T Matrix<N, M, T>::operator()(size_t row, size_t col) const {
  return this->matrix_[row][col];
}

template <size_t N, size_t M, typename T>
T Matrix<N, M, T>::Trace() {
  Matrix<N, N, T> check = *this;
  T val = 0;
  for (size_t i = 0; i < N; ++i) {
    val += this->operator()(i, i);
  }
  return val;
}

template <size_t N, size_t M, typename T>
Matrix<M, N, T> Matrix<N, M, T>::Transposed() {
  Matrix<M, N, T> ret;
  for (size_t i = 0; i < M; ++i) {
    for (size_t j = 0; j < N; ++j) {
      ret(i, j) = this->operator()(j, i);
    }
  }
  return ret;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>& operator*(const T& k_mul, Matrix<N, M, T>& other) {
  return other * k_mul;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>& Matrix<N, M, T>::operator=(Matrix<N, M, T> k_other) {
  this->matrix_ = k_other.matrix_;
  return *this;
}

template <size_t N, size_t M, typename T>
bool Matrix<N, M, T>::operator==(const Matrix<N, M, T>& k_other) {
  return this->matrix_ == k_other.matrix_;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T> Matrix<N, M, T>::operator-(const Matrix<N, M, T>& k_other) {
  Matrix ret = *this;
  ret -= k_other;
  return ret;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T> Matrix<N, M, T>::operator*(const T& k_mul) {
  Matrix ret = *this;
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      ret.matrix_[i][j] *= k_mul;
    }
  }
  return ret;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>& Matrix<N, M, T>::operator-=(const Matrix<N, M, T>& k_other) {
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      this->matrix_[i][j] -= k_other.matrix_[i][j];
    }
  }
  return *this;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T> Matrix<N, M, T>::operator+(const Matrix& k_other) {
  Matrix ret = *this;
  ret += k_other;
  return ret;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>& Matrix<N, M, T>::operator+=(const Matrix<N, M, T>& k_other) {
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      this->matrix_[i][j] += k_other.matrix_[i][j];
    }
  }
  return *this;
}

template <size_t N, size_t M, typename T>
T& Matrix<N, M, T>::operator()(size_t row, size_t col) {
  return this->matrix_[row][col];
}
