#ifndef ZFUNCTIONAL_H
#define ZFUNCTIONAL_H

#include <iterator>
#include <algorithm>
#include <vector>

namespace ZLib {

namespace ZFunctional {

#define all(v) v.begin(), v.end()

template <template <typename, typename> class OutCont = std::vector,
          template <typename> class Alloc = std::allocator,
          typename Fn, class Cont>
OutCont<typename Cont::value_type, Alloc<typename Cont::value_type >> fmap(Fn mapfn, const Cont& inputs)
{
    OutCont<typename Cont::value_type, Alloc<typename Cont::value_type >> result;
    transform(inputs.begin(), inputs.end(), std::back_inserter(result), mapfn);
    return result;
}

template <typename Fn, class Cont>
typename Cont::value_type reduce(Fn reducefn, const Cont& inputs,
                                 typename Cont::value_type initval = typename Cont::value_type())
{
    auto result = initval;
    for (auto& item : inputs)
        result = reducefn(result, item);
    return result;
}

}
}

#endif
