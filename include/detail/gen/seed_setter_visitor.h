#ifndef DETAIL_GEN_SEED_SETTER_VISITOR_H
#define DETAIL_GEN_SEED_SETTER_VISITOR_H

#include <boost/variant.hpp>

namespace detail
{
    namespace gen
    {
        template<typename T>
        struct SeedSetter
        {
        private:
            template<typename U, void(U::*Fn)(int)> struct Eval {};
            template<typename U> static std::true_type test(Eval<U, &U::SetSeed>*)
            {
                return std::true_type{};
            }
            template<typename U> static std::false_type test(...)
            {
                return std::false_type{};
            }

            template<typename U> static void set(U& u, int seed, std::true_type)
            {
                u.SetSeed(seed);
            }

            template<typename U> static void set(T& t, int seed, std::false_type)
            {
                // ...
            }

        public:
            static void setSeed(T& t, int seed)
            {
                set<T>(t, seed, test<T>(nullptr));
            }
        };

        class SeedSetterVisitor : public boost::static_visitor<>
        {
        public:
            SeedSetterVisitor(int seed)
                : seed{seed}
            {

            }

            template<typename T>
            void operator()(T& module) const
            {
                SeedSetter<T>::setSeed(module, seed);
            }
        private:
            int seed;
        };

    }
}

#endif
