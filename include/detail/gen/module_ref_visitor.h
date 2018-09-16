#ifndef DETAIL_GEN_MODULE_REF_VISITOR_H
#define DETAIL_GEN_MODULE_REF_VISITOR_H

#include <boost/variant.hpp>
#include <noise/module/module.h>

struct ModuleRefVistor : public boost::static_visitor<noise::module::Module&>
{
public:
    template<typename T>
    noise::module::Module& operator()(T& module) const
    {
        return static_cast<noise::module::Module&>(module);
    }
};

#endif