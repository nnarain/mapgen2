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

struct Parameter
{
    enum class Type
    {
        Int,
        Float
    };

    union
    {
        int i;
        float f;
    } value;

    Type type;

    explicit Parameter(int i)
    {
        type = Type::Int;
        value.i = i;
    }

    explicit Parameter(float f)
    {
        type = Type::Float;
        value.f = f;
    }
};

#endif // NOISE_GEN_PARAMETER_H