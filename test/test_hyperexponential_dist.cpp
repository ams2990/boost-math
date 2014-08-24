// Copyright 2014 Marco Guazzone (marco.guazzone@gmail.com).
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <algorithm>
#include <boost/math/concepts/real_concept.hpp>
#include <boost/math/distributions/complement.hpp>
#include <boost/math/distributions/hyperexponential.hpp>
#include <boost/math/tools/precision.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <cstddef>
#include <iostream>
#include <vector>


#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
# define HYPEREXP_TEST_LONGDOUBLE(test) (test)()
#else
# define HYPEREXP_TEST_LONGDOUBLE(test) \
    std::cout << "<note>The long double tests have been disabled on this platform " \
                 "either because the long double overloads of the usual math functions are " \
                 "not available at all, or because they are too inaccurate for these tests " \
                 "to pass.</note>" << std::cout
#endif // BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS


// We must use a low precision since it seems the involved computations are very challenging from the numerical point of view.
// Indeed, both Octave 3.6.4, MATLAB 2012a and Mathematica 10 provides different results.
// E.g.:
//  x = [0 1 2 3 4]
//  p = [0.2 0.3 0.5]
//  r = [0.5 1.0 1.5]
//  PDF(x)
//    - MATLAB:      1.033333333333333,  0.335636985323608,  0.135792553231720,   0.061039382459897,   0.028790027125382
//    - Octave:      1.0333333333333332, 0.3356369853236084, 0.1357925532317197,  0.0610393824598966,  0.0287900271253818
//    - Mathematica: 1.15,               0.3383645184340184, 0.11472883036402601, 0.04558088392888389, 0.02088728412278129
//
//  (Tested under Fedora Linux 20 x86_64 running on Intel(R) Core(TM) i7-3540M)
//


template <typename RealT>
struct make_tolerance_impl
{
    RealT operator()() const
    {
/*[code snippet taken from hypergeometric]
        RealT tol = std::max(boost::math::tools::epsilon<RealT>(),
                             static_cast<RealT>(boost::math::tools::epsilon<double>()*5)*150);

        // At float precision we need to up the tolerance, since 
        // the input values are rounded off to inexact quantities
        // the results get thrown off by a noticeable amount.

        if (boost::math::tools::digits<RealT>() < 50)
        {
            tol *= 50;
        }
        if (boost::is_floating_point<RealT>::value != 1)
        {
            tol *= 20; // real_concept special functions are less accurate
        }

        return tol;
*/
 
        return 5e+6*boost::math::tools::epsilon<RealT>();
    }
}; // make_tolerance_impl

template <>
struct make_tolerance_impl<double>
{
    double operator()() const
    {
        return 2e+3*boost::math::tools::epsilon<double>();
    }
}; // make_tolerance_impl

template <>
struct make_tolerance_impl<float>
{
    float operator()() const
    {
        return 1e-4;
    }
};


template <typename RealT>
RealT make_tolerance()
{
    const RealT tol = make_tolerance_impl<RealT>()();

    //std::cout << "[" << __func__ << "] Tolerance: " << tol << "%" << std::endl;

    return tol;
}

template <typename RealT>
void test_range()
{
    const RealT tol = make_tolerance<RealT>();

    const RealT probs[] = {0.2, 0.3, 0.5};
    const RealT rates[] = {0.5, 1.0, 1.5};
    const std::size_t n = sizeof(probs)/sizeof(RealT);

    boost::math::hyperexponential_distribution<RealT> dist(probs, probs+n, rates, rates+n);

    std::pair<RealT,RealT> res;
    res = boost::math::range(dist);

    BOOST_CHECK_CLOSE( res.first, 0, tol );
    BOOST_CHECK_EQUAL( res.second, std::numeric_limits<RealT>::infinity() );
}

template <typename RealT>
void test_support()
{
    const RealT tol = make_tolerance<RealT>();

    const RealT probs[] = {0.2, 0.3, 0.5};
    const RealT rates[] = {0.5, 1.0, 1.5};
    const std::size_t n = sizeof(probs)/sizeof(RealT);

    boost::math::hyperexponential_distribution<RealT> dist(probs, probs+n, rates, rates+n);

    std::pair<RealT,RealT> res;
    res = boost::math::support(dist);

    BOOST_CHECK_CLOSE( res.first, boost::math::tools::min_value<RealT>(), tol );
    BOOST_CHECK_CLOSE( res.second, boost::math::tools::max_value<RealT>(), tol );
}

template <typename RealT>
void test_pdf()
{
    const RealT tol = make_tolerance<RealT>();

    const RealT probs[] = {0.2, 0.3, 0.5};
    const RealT rates[] = {0.5, 1.0, 1.5};
    const std::size_t n = sizeof(probs)/sizeof(RealT);

    boost::math::hyperexponential_distribution<RealT> dist(probs, probs+n, rates, rates+n);

    // Mathematica: Table[SetPrecision[PDF[HyperexponentialDistribution[{0.2, 0.3, 0.5}, {.5, 1.0, 1.5}], x], 35], {x, 0, 4}]
//    BOOST_CHECK_CLOSE( boost::math::pdf(dist, static_cast<RealT>(0)), static_cast<RealT>(1.15), tol );
//    BOOST_CHECK_CLOSE( boost::math::pdf(dist, static_cast<RealT>(1)), static_cast<RealT>(0.3383645184340184), tol );
//    BOOST_CHECK_CLOSE( boost::math::pdf(dist, static_cast<RealT>(2)), static_cast<RealT>(0.11472883036402601), tol );
//    BOOST_CHECK_CLOSE( boost::math::pdf(dist, static_cast<RealT>(3)), static_cast<RealT>(0.04558088392888389), tol );
//    BOOST_CHECK_CLOSE( boost::math::pdf(dist, static_cast<RealT>(4)), static_cast<RealT>(0.02088728412278129), tol );
    BOOST_CHECK_CLOSE( boost::math::pdf(dist, static_cast<RealT>(0)), static_cast<RealT>(1.149999999999999911182158029987476766109466552734375L), tol );
    BOOST_CHECK_CLOSE( boost::math::pdf(dist, static_cast<RealT>(1)), static_cast<RealT>(0.33836451843401837979996571448282338678836822509765625L), tol );
    BOOST_CHECK_CLOSE( boost::math::pdf(dist, static_cast<RealT>(2)), static_cast<RealT>(0.1147288303640260076488033291752799414098262786865234375), tol );
    BOOST_CHECK_CLOSE( boost::math::pdf(dist, static_cast<RealT>(3)), static_cast<RealT>(0.04558088392888388906687424650954199023544788360595703125L), tol );
    BOOST_CHECK_CLOSE( boost::math::pdf(dist, static_cast<RealT>(4)), static_cast<RealT>(0.02088728412278129109580504518817178905010223388671875L), tol );
}

template <typename RealT>
void test_cdf()
{
    const RealT tol = make_tolerance<RealT>();

    const RealT probs[] = {0.2, 0.3, 0.5};
    const RealT rates[] = {0.5, 1.0, 1.5};
    const std::size_t n = sizeof(probs)/sizeof(RealT);

    boost::math::hyperexponential_distribution<RealT> dist(probs, probs+n, rates, rates+n);

    // Mathematica: Table[SetPrecision[CDF[HyperexponentialDistribution[{0.2, 0.3, 0.5}, {.5, 1.0, 1.5}], x], 35], {x, 0, 4}]
    BOOST_CHECK_CLOSE( boost::math::cdf(dist, static_cast<RealT>(0)), static_cast<RealT>(0), tol );
//    BOOST_CHECK_CLOSE( boost::math::cdf(dist, static_cast<RealT>(1)), static_cast<RealT>(0.6567649556318257), tol );
//    BOOST_CHECK_CLOSE( boost::math::cdf(dist, static_cast<RealT>(2)), static_cast<RealT>(0.8609299926107957), tol );
//    BOOST_CHECK_CLOSE( boost::math::cdf(dist, static_cast<RealT>(3)), static_cast<RealT>(0.9348833491908337), tol );
//    BOOST_CHECK_CLOSE( boost::math::cdf(dist, static_cast<RealT>(4)), static_cast<RealT>(0.966198875597724), tol );
    BOOST_CHECK_CLOSE( boost::math::cdf(dist, static_cast<RealT>(1)), static_cast<RealT>(0.65676495563182568648841197500587441027164459228515625L), tol );
    BOOST_CHECK_CLOSE( boost::math::cdf(dist, static_cast<RealT>(2)), static_cast<RealT>(0.86092999261079572459465225620078854262828826904296875L), tol );
    BOOST_CHECK_CLOSE( boost::math::cdf(dist, static_cast<RealT>(3)), static_cast<RealT>(0.93488334919083371232773060910403728485107421875L), tol );
    BOOST_CHECK_CLOSE( boost::math::cdf(dist, static_cast<RealT>(4)), static_cast<RealT>(0.966198875597723993990939561626873910427093505859375L), tol );
}


template <typename RealT>
void test_quantile()
{
    const RealT tol = make_tolerance<RealT>();

    const RealT probs[] = {0.2, 0.3, 0.5};
    const RealT rates[] = {0.5, 1.0, 1.5};
    const std::size_t n = sizeof(probs)/sizeof(RealT);

    boost::math::hyperexponential_distribution<RealT> dist(probs, probs+n, rates, rates+n);

    // Mathematica: Table[SetPrecision[Quantile[HyperexponentialDistribution[{0.2, 0.3, 0.5}, {.5, 1.0, 1.5}], p], 35], {p, {0, 0.65676495563182568648841197500587441027164459228515625`35, 0.86092999261079572459465225620078854262828826904296875`35, 0.93488334919083371232773060910403728485107421875`35, 0.966198875597723993990939561626873910427093505859375`35}}]
    BOOST_CHECK_CLOSE( boost::math::quantile(dist, static_cast<RealT>(0)), static_cast<RealT>(0), tol );
//    BOOST_CHECK_CLOSE( boost::math::quantile(dist, static_cast<RealT>(0.6567649556318257)), static_cast<RealT>(1.0000000000000036), tol );
//    BOOST_CHECK_CLOSE( boost::math::quantile(dist, static_cast<RealT>(0.8609299926107957)), static_cast<RealT>(1.9999999999999947), tol );
//    BOOST_CHECK_CLOSE( boost::math::quantile(dist, static_cast<RealT>(0.9348833491908337)), static_cast<RealT>(3), tol );
//    BOOST_CHECK_CLOSE( boost::math::quantile(dist, static_cast<RealT>(0.966198875597724)), static_cast<RealT>(3.9999999999999964), tol );
    BOOST_CHECK_CLOSE( boost::math::quantile(dist, static_cast<RealT>(0.65676495563182568648841197500587441027164459228515625L)), static_cast<RealT>(1.000000000000003552713678800500929355621337890625), tol );
    BOOST_CHECK_CLOSE( boost::math::quantile(dist, static_cast<RealT>(0.86092999261079572459465225620078854262828826904296875L)), static_cast<RealT>(2.000000000000003552713678800500929355621337890625L), tol );
    BOOST_CHECK_CLOSE( boost::math::quantile(dist, static_cast<RealT>(0.93488334919083371232773060910403728485107421875L)), static_cast<RealT>(2.99999999999999289457264239899814128875732421875L), tol );
    BOOST_CHECK_CLOSE( boost::math::quantile(dist, static_cast<RealT>(0.966198875597723993990939561626873910427093505859375L)), static_cast<RealT>(3.9999999999999946709294817992486059665679931640625L), tol );
}

template <typename RealT>
void test_ccdf()
{
    const RealT tol = make_tolerance<RealT>();

    const RealT probs[] = {0.2, 0.3, 0.5};
    const RealT rates[] = {0.5, 1.0, 1.5};
    const std::size_t n = sizeof(probs)/sizeof(RealT);

    boost::math::hyperexponential_distribution<RealT> dist(probs, probs+n, rates, rates+n);

    // Mathematica: Table[SetPrecision[SurvivalFunction[HyperexponentialDistribution[{0.2, 0.3, 0.5}, {.5, 1.0, 1.5}], x], 35], {x, 0, 4}]
    BOOST_CHECK_CLOSE( boost::math::cdf(boost::math::complement(dist, static_cast<RealT>(0))), static_cast<RealT>(1), tol );
//    BOOST_CHECK_CLOSE( boost::math::cdf(boost::math::complement(dist, static_cast<RealT>(1))), static_cast<RealT>(0.3432350443681743), tol );
//    BOOST_CHECK_CLOSE( boost::math::cdf(boost::math::complement(dist, static_cast<RealT>(2))), static_cast<RealT>(0.13907000738920425), tol );
//    BOOST_CHECK_CLOSE( boost::math::cdf(boost::math::complement(dist, static_cast<RealT>(3))), static_cast<RealT>(0.0651166508091663), tol );
//    BOOST_CHECK_CLOSE( boost::math::cdf(boost::math::complement(dist, static_cast<RealT>(4))), static_cast<RealT>(0.03380112440227598), tol );
    BOOST_CHECK_CLOSE( boost::math::cdf(boost::math::complement(dist, static_cast<RealT>(1))), static_cast<RealT>(0.34323504436817431351158802499412558972835540771484375L), tol );
    BOOST_CHECK_CLOSE( boost::math::cdf(boost::math::complement(dist, static_cast<RealT>(2))), static_cast<RealT>(0.1390700073892042476497721281702979467809200286865234375L), tol );
    BOOST_CHECK_CLOSE( boost::math::cdf(boost::math::complement(dist, static_cast<RealT>(3))), static_cast<RealT>(0.06511665080916630155005719871041947044432163238525390625L), tol );
    BOOST_CHECK_CLOSE( boost::math::cdf(boost::math::complement(dist, static_cast<RealT>(4))), static_cast<RealT>(0.0338011244022759782534848227442125789821147918701171875L), tol );
}


template <typename RealT>
void test_cquantile()
{
    const RealT tol = make_tolerance<RealT>();

    const RealT probs[] = {0.2, 0.3, 0.5};
    const RealT rates[] = {0.5, 1.0, 1.5};
    const std::size_t n = sizeof(probs)/sizeof(RealT);

    boost::math::hyperexponential_distribution<RealT> dist(probs, probs+n, rates, rates+n);

    // Mathematica: Table[SetPrecision[InverseSurvivalFunction[HyperexponentialDistribution[{0.2, 0.3, 0.5}, {.5, 1.0, 1.5}], p], 35], {p, {1.`35, 0.34323504436817431351158802499412558972835540771484375`35, 0.1390700073892042476497721281702979467809200286865234375`35, 0.06511665080916630155005719871041947044432163238525390625`35, 0.0338011244022759782534848227442125789821147918701171875`35}}]
    BOOST_CHECK_CLOSE( boost::math::quantile(boost::math::complement(dist, static_cast<RealT>(1))), static_cast<RealT>(0), tol );
//    BOOST_CHECK_CLOSE( boost::math::quantile(boost::math::complement(dist, static_cast<RealT>(0.3432350443681743))), static_cast<RealT>(1.0000000000000036), tol );
//    BOOST_CHECK_CLOSE( boost::math::quantile(boost::math::complement(dist, static_cast<RealT>(0.13907000738920425))), static_cast<RealT>(1.9999999999999947), tol );
//    BOOST_CHECK_CLOSE( boost::math::quantile(boost::math::complement(dist, static_cast<RealT>(0.0651166508091663))), static_cast<RealT>(3), tol );
//    BOOST_CHECK_CLOSE( boost::math::quantile(boost::math::complement(dist, static_cast<RealT>(0.03380112440227598))), static_cast<RealT>(3.9999999999999964), tol );
    BOOST_CHECK_CLOSE( boost::math::quantile(boost::math::complement(dist, static_cast<RealT>(0.34323504436817431351158802499412558972835540771484375))), static_cast<RealT>(1.000000000000003552713678800500929355621337890625L), tol );
    BOOST_CHECK_CLOSE( boost::math::quantile(boost::math::complement(dist, static_cast<RealT>(0.1390700073892042476497721281702979467809200286865234375))), static_cast<RealT>(1.999999999999996447286321199499070644378662109375L), tol );
    BOOST_CHECK_CLOSE( boost::math::quantile(boost::math::complement(dist, static_cast<RealT>(0.06511665080916630155005719871041947044432163238525390625))), static_cast<RealT>(3.000000000000010658141036401502788066864013671875L), tol );
    BOOST_CHECK_CLOSE( boost::math::quantile(boost::math::complement(dist, static_cast<RealT>(0.0338011244022759782534848227442125789821147918701171875))), static_cast<RealT>(3.999999999999996447286321199499070644378662109375L), tol );
}

template <typename RealT>
void test_mean()
{
    const RealT tol = make_tolerance<RealT>();

    const RealT probs[] = {0.2, 0.3, 0.5};
    const RealT rates[] = {0.5, 1.0, 1.5};
    const std::size_t n = sizeof(probs)/sizeof(RealT);

    boost::math::hyperexponential_distribution<RealT> dist(probs, probs+n, rates, rates+n);

    // Mathematica: SetPrecision[Mean[HyperexponentialDistribution[{0.2, 0.3, 0.5}, {.5, 1.0, 1.5}]], 35]
//    BOOST_CHECK_CLOSE( boost::math::mean(dist), static_cast<RealT>(1.0333333333333332), tol );
    BOOST_CHECK_CLOSE( boost::math::mean(dist), static_cast<RealT>(1.0333333333333332149095440399833023548126220703125L), tol );
}

template <typename RealT>
void test_variance()
{
    const RealT tol = make_tolerance<RealT>();

    const RealT probs[] = {0.2, 0.3, 0.5};
    const RealT rates[] = {0.5, 1.0, 1.5};
    const std::size_t n = sizeof(probs)/sizeof(RealT);

    boost::math::hyperexponential_distribution<RealT> dist(probs, probs+n, rates, rates+n);

    // Mathematica: SetPrecision[Variance[HyperexponentialDistribution[{0.2, 0.3, 0.5}, {.5, 1.0, 1.5}]], 35]
//    BOOST_CHECK_CLOSE( boost::math::variance(dist), static_cast<RealT>(1.5766666666666673), tol );
    BOOST_CHECK_CLOSE( boost::math::variance(dist), static_cast<RealT>(1.5766666666666673268792919770930893719196319580078125L), tol );
}

template <typename RealT>
void test_kurtosis()
{
    const RealT tol = make_tolerance<RealT>();

    const RealT probs[] = {0.2, 0.3, 0.5};
    const RealT rates[] = {0.5, 1.0, 1.5};
    const std::size_t n = sizeof(probs)/sizeof(RealT);

    boost::math::hyperexponential_distribution<RealT> dist(probs, probs+n, rates, rates+n);

    // Mathematica: SetPrecision[Kurtosis[HyperexponentialDistribution[{0.2, 0.3, 0.5}, {.5, 1.0, 1.5}]], 35]
//    BOOST_CHECK_CLOSE( boost::math::kurtosis(dist), static_cast<RealT>(19.75073861680871), tol );
//    BOOST_CHECK_CLOSE( boost::math::kurtosis_excess(dist), static_cast<RealT>(19.75073861680871)-static_cast<RealT>(3), tol );
    BOOST_CHECK_CLOSE( boost::math::kurtosis(dist), static_cast<RealT>(19.75073861680871090129585354588925838470458984375L), tol );
    BOOST_CHECK_CLOSE( boost::math::kurtosis_excess(dist), static_cast<RealT>(19.75073861680871090129585354588925838470458984375L)-static_cast<RealT>(3), tol );
}

template <typename RealT>
void test_skewness()
{
    const RealT tol = make_tolerance<RealT>();

    const RealT probs[] = {0.2, 0.3, 0.5};
    const RealT rates[] = {0.5, 1.0, 1.5};
    const std::size_t n = sizeof(probs)/sizeof(RealT);

    boost::math::hyperexponential_distribution<RealT> dist(probs, probs+n, rates, rates+n);

    // Mathematica: SetPrecision[Skewness[HyperexponentialDistribution[{0.2, 0.3, 0.5}, {.5, 1.0, 1.5}]], 35]
    //BOOST_CHECK_CLOSE( boost::math::skewness(dist), static_cast<RealT>(3.181138744996378), tol );
    BOOST_CHECK_CLOSE( boost::math::skewness(dist), static_cast<RealT>(3.181138744996378164842099067755043506622314453125L), tol );
}

template <typename RealT>
void test_mode()
{
    const RealT tol = make_tolerance<RealT>();

    const RealT probs[] = {0.2, 0.3, 0.5};
    const RealT rates[] = {0.5, 1.0, 1.5};
    const std::size_t n = sizeof(probs)/sizeof(RealT);

    boost::math::hyperexponential_distribution<RealT> dist(probs, probs+n, rates, rates+n);

    BOOST_CHECK_CLOSE( boost::math::mode(dist), static_cast<RealT>(0), tol );
}


BOOST_AUTO_TEST_CASE( range )
{
    test_range<float>();
    test_range<double>();
    HYPEREXP_TEST_LONGDOUBLE( test_range<long double> );
    //test_range<boost::math::concepts::real_concept>();
}

BOOST_AUTO_TEST_CASE( support )
{
    test_support<float>();
    test_support<double>();
    HYPEREXP_TEST_LONGDOUBLE( test_support<long double> );
}

BOOST_AUTO_TEST_CASE( pdf )
{
    test_pdf<float>();
    test_pdf<double>();
    HYPEREXP_TEST_LONGDOUBLE( test_pdf<long double> );
}

BOOST_AUTO_TEST_CASE( cdf )
{
    test_cdf<float>();
    test_cdf<double>();
    HYPEREXP_TEST_LONGDOUBLE( test_cdf<long double> );
}

BOOST_AUTO_TEST_CASE( quantile )
{
    test_quantile<float>();
    test_quantile<double>();
    HYPEREXP_TEST_LONGDOUBLE( test_quantile<long double> );
}

BOOST_AUTO_TEST_CASE( ccdf )
{
    test_ccdf<float>();
    test_ccdf<double>();
    HYPEREXP_TEST_LONGDOUBLE( test_ccdf<long double> );
}

BOOST_AUTO_TEST_CASE( cquantile )
{
    test_cquantile<float>();
    test_cquantile<double>();
    HYPEREXP_TEST_LONGDOUBLE( test_cquantile<long double> );
}

BOOST_AUTO_TEST_CASE( mean )
{
    test_mean<float>();
    test_mean<double>();
    HYPEREXP_TEST_LONGDOUBLE( test_mean<long double> );
}

BOOST_AUTO_TEST_CASE( variance )
{
    test_variance<float>();
    test_variance<double>();
    HYPEREXP_TEST_LONGDOUBLE( test_variance<long double> );
}

BOOST_AUTO_TEST_CASE( kurtosis )
{
    test_kurtosis<float>();
    test_kurtosis<double>();
    HYPEREXP_TEST_LONGDOUBLE( test_kurtosis<long double> );
}

BOOST_AUTO_TEST_CASE( skewness )
{
    test_skewness<float>();
    test_skewness<double>();
    HYPEREXP_TEST_LONGDOUBLE( test_skewness<long double> );
}

BOOST_AUTO_TEST_CASE( mode )
{
    test_mode<float>();
    test_mode<double>();
    HYPEREXP_TEST_LONGDOUBLE( test_mode<long double> );
}

