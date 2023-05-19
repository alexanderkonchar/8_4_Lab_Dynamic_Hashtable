//
// Created by Alex on 3/20/23.
//

#ifndef INC_8_4_LAB_DYNAMIC_HASHTABLE_HASHTABLE_H
#define INC_8_4_LAB_DYNAMIC_HASHTABLE_HASHTABLE_H

#include <iostream>
#include <memory>
#include <cmath>

using namespace std;

template<class Type>
struct Bucket {
    Type data;
    bool empty = true;
};


template<class Type>
class Hashtable;

template<class Type>
ostream &operator<<(ostream &, Hashtable<Type> &);


template<class Type>
class Hashtable {
public:
    Hashtable();

    explicit Hashtable(int);

    Hashtable(int capacity, double loadFactorThreshold);

    Hashtable(const Hashtable<Type> &other);

    Hashtable &operator=(const Hashtable<Type> &other);

    [[nodiscard]] int getSize() const;

    [[nodiscard]] int getCapacity() const;

    [[nodiscard]] double getLoadFactorThreshold() const;

    [[nodiscard]] bool empty() const;

    void insert(const Type &value);

    void rehash();

    void remove(int);

    [[nodiscard]] bool contains(int) const;

    [[nodiscard]] int indexOf(int value) const;

    void clear();

    friend ostream &operator
    <<<>(ostream &, Hashtable<Type> &);

    static bool isPrime(int num);

    static int nextPrime(int num);

private:
    int hash(const Type &v) const;

    int capacity;
    int size;
    double loadFactorThreshold;
    unique_ptr<Bucket<Type>[]> htable;
};


template<class Type>
Hashtable<Type>::Hashtable() {
    size = 0;
    capacity = 17;
    loadFactorThreshold = .65;
    htable = make_unique<Bucket<Type>[]>(capacity);
}


template<class Type>
Hashtable<Type>::Hashtable(int capacity) : capacity(capacity) {
    size = 0;
    loadFactorThreshold = .65;
    htable = make_unique<Bucket<Type>[]>(capacity);
}

template<class Type>
Hashtable<Type>::Hashtable(int capacity, double loadFactorThreshold): capacity(capacity),
                                                                      loadFactorThreshold(loadFactorThreshold) {
    size = 0;
    htable = make_unique<Bucket<Type>[]>(capacity);
}

/*
	Complete the operator= to create a deep copy of the hashtable
*/
template<class Type>
Hashtable<Type>::Hashtable(const Hashtable<Type> &other) {
    capacity = other.capacity;
    size = other.size;
    loadFactorThreshold = other.loadFactorThreshold;
    htable = make_unique<Bucket<Type>[]>(capacity);
    for (int i = 0; i < capacity; ++i) {
        htable[i] = other.htable[i];
    }
}

/*
	Complete the operator= to create a deep copy of the hashtable
*/
template<class Type>
Hashtable<Type> &Hashtable<Type>::operator=(const Hashtable<Type> &other) {
    capacity = other.capacity;
    size = other.size;
    loadFactorThreshold = other.loadFactorThreshold;
    htable = make_unique<Bucket<Type>[]>(capacity);
    for (int i = 0; i < capacity; ++i) {
        htable[i] = other.htable[i];
    }

    return *this;
}


/*
	Complete the hash method to create a modulus division hash algorithm
*/
template<class Type>
int Hashtable<Type>::hash(const Type &v) const {
    return fmod(v, capacity);
}

template<class Type>
int Hashtable<Type>::getSize() const {
    return size;
}

template<class Type>
int Hashtable<Type>::getCapacity() const {
    return capacity;
}

template<class Type>
double Hashtable<Type>::getLoadFactorThreshold() const {
    return loadFactorThreshold;
}

template<class Type>
bool Hashtable<Type>::empty() const {
    if (size == 0) {
        return true;
    }
    return false;
}


/*
	Complete the insert method to insert the value into the hashtable.
	Use linear probing to avoid collisions.  If the hashtable is full, return a
	runtime error
*/
template<class Type>
void Hashtable<Type>::insert(const Type &value) {
    if (size >= capacity) {
        throw runtime_error("Hashtable Full");
    }

    int i = 0;
    while (!htable[fmod((hash(value) + i * i), capacity)].empty) {
        ++i;
    }
    htable[fmod((hash(value) + i * i), capacity)] = {value, false};
    ++size;
    if (size / capacity >= loadFactorThreshold) rehash();
}

template<class Type>
void Hashtable<Type>::rehash() {
    int newCapacity = nextPrime(capacity * 2);

    auto newHtable = make_unique<Bucket<Type>[]>(newCapacity);
    for (int i = 0; i < capacity; ++i) {
        newHtable[i] = htable[i];
    }
    htable = std::move(newHtable);
}

template<class Type>
bool Hashtable<Type>::isPrime(int num) {
    if (num == 1) {
        return false; // 1 is not prime
    }
    for (int i = 2; i <= sqrt(num); ++i) {
        if (num % i == 0) {
            return false; // num is not prime
        }
    }
    return true; // num is prime
}

template<class Type>
int Hashtable<Type>::nextPrime(int num) {
    ++num;
    while (!isPrime(num)) ++num;
    return num;
}

/*
	Complete the remove the value from the hashtable
*/
template<class Type>
void Hashtable<Type>::remove(int value) {
    int i = 0;
    while (htable[fmod((hash(value) + i * i), capacity)].data != value && i < capacity) {
        ++i;
    }
    if (i == capacity) {
        return;
    }
    htable[fmod((hash(value) + i * i), capacity)] = {0, true};
    --size;
}

/*
	Complete the contains method to return true of the Hashtable contains the value
*/
template<class Type>
bool Hashtable<Type>::contains(int value) const {
    int i = 0;
    while (htable[fmod((hash(value) + i * i), capacity)].data != value && i < capacity) {
        ++i;
    }
    if (htable[fmod((hash(value) + i * i), capacity)].data == value && i < capacity) {
        return true;
    }
    return false;
}

template<class Type>
int Hashtable<Type>::indexOf(int value) const {
    int i = 0;
    while (htable[fmod((hash(value) + i * i), capacity)].data != value && i < capacity) {
        ++i;
    }
    if (htable[fmod((hash(value) + i * i), capacity)].data == value && i < capacity) {
        return i;
    }
    return -1;
}

template<class Type>
void Hashtable<Type>::clear() {
    size = 0;
    htable = make_unique<Bucket<Type>[]>(capacity);
}


template<class Type>
ostream &operator<<(ostream &out, Hashtable<Type> &h) {
    out << "\nCapacity: " << h.capacity << "\n";
    for (int i = 0; i < h.capacity; i++) {

        if (h.htable[i].empty == false) {
            out << i << ": " << h.htable[i].data << endl;
        }

    }
    return out;
}

#endif //INC_8_4_LAB_DYNAMIC_HASHTABLE_HASHTABLE_H
