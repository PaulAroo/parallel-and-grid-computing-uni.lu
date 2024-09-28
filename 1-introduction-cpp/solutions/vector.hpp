
template <class T>
class Vector {
  /* The number of elements currently in the array (as stored by the user). */
  size_t n;
  /* The capacity of the vector, which is the size of `values`. */
  size_t cap;
  T* values;

public:

  /* Default constructor */
  Vector() : n(0), cap(0), values(nullptr) {}

  /* allocate an array of size `n` filled with 0. */
  Vector(size_t n) : n(n), cap(n) {
    values = new T[cap]();
  };

  /* Copy constructor */
  Vector(const Vector& other): n(other.n), cap(other.cap) {
    if(cap > 0) {
      values = new T[cap]();
      for(size_t i = 0; i < n; ++i) {
        values[i] = other.values[i];
      }
    }
    else {
      values = nullptr;
    }
    printf("copied \n");
  }


  // Move constructor
  Vector(Vector&& other) : n(other.n), cap(other.cap) {
    if(cap > 0) {
      values = other.values;

      other.n = 0;
      other.cap = 0;
      other.values = nullptr;
    }
    else {
      values = nullptr;
    }
    printf("moved \n");
  }

  /* Destructor */
  ~Vector() {
    delete[] values;
  }

  size_t capacity() const {
    return cap;
  };
  size_t size() const {
    return n;
  };

  void resize(size_t new_size) {
    if(new_size > cap) {
      reserve(new_size);
    }

    if(new_size > n) {
      for(size_t i = n; i < new_size; ++i) {
        values[i] = 0;
      }
    }

    n = new_size;
  };

  void reserve(size_t new_capacity) {
    if(new_capacity > cap) {
      T* new_values = new T[new_capacity];
      for(size_t i = 0; i < n; ++i) {
        new_values[i] = values[i];
      }

      delete[] values;
      values = new_values;
      cap = new_capacity;
    }
  };

  /* Add the value `v` at the back of the vector, reallocating if necessary. */
  void push_back(const T& v) {
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

  T& back() {
    return values[n-1];
  };
  const T& back() const {
    return values[n-1];
  };

  T& front() {
    return values[0];
  };
  const T& front() const {
    return values[0];
  };

  T& operator[](size_t i) {
    return values[i];
  };
  const T& operator[](size_t i) const {
    return values[i];
  };
};



// Implement a map function taking a vector and applying a function f to each of its component.
// Suppose an integer vector v with the values 1,-5,6, then map(v, [](int x) { return x * 2; }) modifies v in-place and double each value.
template <typename T, typename Func>
void map(Vector<T>& v, Func f) {
  for(size_t i = 0; i < v.size(); ++i) {
    v[i] = f(v[i]);
  }
};


//  Implement reduce functions fold_left and fold_right
template <typename T, typename Func>
T fold_left(Vector<T>& vec, int accu, Func f) {
  int value = accu;
  for(int i = 0; i < vec.size(); ++i) {
    value = f(value, vec[i]);
  }

  // TODO: test this implementation (range based for loops)
  // update: didn't work in maps when I tried it, begin and end errors
  // for(const T& v : vec) {
  //   value = f(value, v);
  // }

  return value;
}

template <typename T, typename Func>
T fold_right(Vector<T>& vec, int accu, Func f) {
  int value = accu;
  for(int i = vec.size() - 1; i >= 0; --i) {
    value = f(value, vec[i]);
  }

  return value;
}