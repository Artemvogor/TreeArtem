#pragma once
#include <cstdio>
#include <memory.h>

#define NONE(K) (1ULL << K)

template <unsigned int K>
class Veb {
private:
    unsigned long long T_min, T_max;
    Veb<(K >> 1)> *T[1ULL << (K >> 1)], *aux;
public:
    Veb();
    ~Veb();
    inline bool empty() const;
    inline unsigned long long get_min() const;
    inline unsigned long long get_max() const;
    inline unsigned long long high(unsigned long long key) const;
    inline unsigned long long low(unsigned long long key) const;
    inline unsigned long long merge(unsigned long long high, unsigned long long low) const;
    void insert(unsigned long long key);
    unsigned long long find_next(unsigned long long key);
    bool lookup(unsigned long long key);
};