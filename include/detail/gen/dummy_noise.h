#ifndef DETAIL_GEN_DUMMY_NOISE_H
#define DETAIL_GEN_DUMMY_NOISE_H

#include <noise/module/module.h>

/**
    Dummy noise type used for initialization
*/
class DummyNoise : public noise::module::Module
{
public:
    DummyNoise() : Module(0)
    {

    }

    static DummyNoise& instance()
    {
        static DummyNoise d;
        return d;
    }

    virtual int GetSourceModuleCount() const override
    {
        return 0;
    }

    virtual double GetValue(double x, double y, double z) const override
    {
        return 0.0;
    }
};

#endif