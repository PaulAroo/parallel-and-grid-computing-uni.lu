#include <cstring>

class IntVector {
  /* The number of elements currently in the array (as stored by the user). */
  size_t n;
  /* The capacity of the vector, which is the size of `values`. */
  size_t cap;
  int* values;

public:

  /* Default constructor */
  IntVector(): n(0), cap(0), values(nullptr) {}

  /* Copy constructor */
  IntVector(const IntVector& other) : n(other.n), cap(other.cap) {
    // memcpy(values, other.values, cap);
    if(cap > 0) {
      values = new int[cap]();
      for(size_t i = 0; i < n; i++) {
        values[i] = other.values[i];
      }
    } else {
      values = nullptr;
    }
  }


  /* Destructor */
  ~IntVector() {
    delete[] values;
  }

  /* allocate an array of size `n` filled with 0. */
  IntVector(size_t n): n(n), cap(n) {
    values = new int[cap]();
  };

  size_t capacity() const {
    return cap;
  };
  size_t size() const {
    return n;
  };

  void resize(size_t new_size) {
    if(new_size > capacity()) {
      reserve(new_size);
    }

    if(new_size > size()) {
      for(size_t i = n; i < new_size; ++i) {
        values[i] = 0;
      }
    }

    n = new_size;
  };

  void reserve(size_t new_cap) {
    if(new_cap > capacity()) {
      int* new_values = new int[new_cap];
      for(size_t i = 0; i < n; ++i) {
        new_values[i] = values[i];
      }

      delete[] values;
      values = new_values;
      cap = new_cap;
    }

  };

  /* Add the value `v` at the back of the vector, reallocating if necessary. */
  void push_back(int v) {
    if(n == cap) {
      size_t new_capacity = cap == 0 ? 1 : cap * 2;
      reserve(new_capacity);
    }
    values[n++] = v;
  };

  /* Remove the last element of the array.
     Precondition: `size() != 0`. */
  void pop_back() {
    if(n != 0) {
      n--;
    }
  };

  int& back() {
    return values[n-1];
  };
  const int& back() const {
    return values[n-1];
  };

  int& front() {
    return values[0];
  };
  const int& front() const {
    return values[0];
  };

  int& operator[](size_t i) {
    return values[i];
  };
  const int& operator[](size_t i) const {
    return values[i];
  };
};
