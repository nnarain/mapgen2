#ifndef NOISE_GEN_PARAMETER_H
#define NOISE_GEN_PARAMETER_H

template<class T>
struct Ranged
{
    T min;
    T max;
    T value;

    explicit Ranged(T min, T max, T value) : min{ min }, max{ max }, value{ value }
    {
    }

    explicit Ranged() : Ranged(T{}, T{}, T{})
    {
    }
};

using RangedInt = Ranged<int>;
using RangedFloat = Ranged<float>;

#endif // NOISE_GEN_PARAMETER_H