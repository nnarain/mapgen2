#ifndef NOISE_GEN_PARAMETER_H
#define NOISE_GEN_PARAMETER_H

template<class T>
struct Ranged
{
    T min;
    T man;
    T value;

    explicit Ranged(T min, T max, T value) : min{ min }, max{ max }, value{ value }
    {
    }
};

using RangedInt = Ranged<int>;
using RangedFloat = Ranged<float>;

#endif // NOISE_GEN_PARAMETER_H