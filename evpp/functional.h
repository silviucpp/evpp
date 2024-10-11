#pragma once

// Choose between evpp::function and boost::function by defining USE_BOOST_FUNCTION

#ifdef USE_BOOST_FUNCTION
#include <boost/function.hpp>
#else
#include <functional>
#endif

namespace evpp {

#ifdef USE_BOOST_FUNCTION
template <typename Signature> using function = boost::function<Signature>;
#else
template <typename Signature> using function = std::function<Signature>;
#endif

}

