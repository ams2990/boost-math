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
typedef boost::mpl::list<float, double, long double, boost::math::concepts::real_concept> test_types;
#else
typedef boost::mpl::list<float, double> test_types;
#endif

template <typename RealT>
RealT make_tolerance()
{
    // Tolerance is 100eps expressed as a persentage (as required by Boost.Build):
    return boost::math::tools::epsilon<RealT>() * 100 * 100;;
}

BOOST_AUTO_TEST_CASE_TEMPLATE(range, RealT, test_types)
{
    const RealT tol = make_tolerance<RealT>();

    const RealT probs[] = { static_cast<RealT>(0.2L), static_cast<RealT>(0.3L), static_cast<RealT>(0.5L) };
    const RealT rates[] = { static_cast<RealT>(0.5L), static_cast<RealT>(1.0L), static_cast<RealT>(1.5L) };
    const std::size_t n = sizeof(probs) / sizeof(RealT);

    boost::math::hyperexponential_distribution<RealT> dist(probs, probs+n, rates, rates+n);

    std::pair<RealT,RealT> res;
    res = boost::math::range(dist);

    BOOST_CHECK_CLOSE( res.first, static_cast<RealT>(0), tol );
    if(std::numeric_limits<RealT>::has_infinity)
    {
       BOOST_CHECK_EQUAL(res.second, std::numeric_limits<RealT>::infinity());
    }
    else
    {
       BOOST_CHECK_EQUAL(res.second, boost::math::tools::max_value<RealT>());
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(support, RealT, test_types)
{
    const RealT tol = make_tolerance<RealT>();

    const RealT probs[] = { static_cast<RealT>(0.2L), static_cast<RealT>(0.3L), static_cast<RealT>(0.5L) };
    const RealT rates[] = { static_cast<RealT>(0.5L), static_cast<RealT>(1), static_cast<RealT>(1.5L) };
    const std::size_t n = sizeof(probs)/sizeof(RealT);

    boost::math::hyperexponential_distribution<RealT> dist(probs, probs+n, rates, rates+n);

    std::pair<RealT,RealT> res;
    res = boost::math::support(dist);

    BOOST_CHECK_CLOSE( res.first, boost::math::tools::min_value<RealT>(), tol );
    BOOST_CHECK_CLOSE( res.second, boost::math::tools::max_value<RealT>(), tol );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(pdf, RealT, test_types)
{
    const RealT tol = make_tolerance<RealT>();

    const RealT probs[] = { static_cast<RealT>(0.2L), static_cast<RealT>(0.3L), static_cast<RealT>(0.5L) };
    const RealT rates[] = { static_cast<RealT>(0.5L), static_cast<RealT>(1), static_cast<RealT>(1.5) };
    const std::size_t n = sizeof(probs)/sizeof(RealT);

    boost::math::hyperexponential_distribution<RealT> dist(probs, probs+n, rates, rates+n);

    // Mathematica: Table[N[PDF[HyperexponentialDistribution[{1/5, 3/10, 1/2}, {1/2, 1, 3/2}], x], 35], {x, 0, 4}]
    BOOST_CHECK_CLOSE( boost::math::pdf(dist, static_cast<RealT>(0)), static_cast<RealT>(1.15L), tol );
    BOOST_CHECK_CLOSE( boost::math::pdf(dist, static_cast<RealT>(1)), static_cast<RealT>(0.33836451843401841053899743762056570L), tol );
    BOOST_CHECK_CLOSE( boost::math::pdf(dist, static_cast<RealT>(2)), static_cast<RealT>(0.11472883036402599696225903724543774L), tol );
    BOOST_CHECK_CLOSE( boost::math::pdf(dist, static_cast<RealT>(3)), static_cast<RealT>(0.045580883928883895659238122486617681L), tol );
    BOOST_CHECK_CLOSE( boost::math::pdf(dist, static_cast<RealT>(4)), static_cast<RealT>(0.020887284122781292094799231452333314L), tol );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cdf, RealT, test_types)
{
    const RealT tol = make_tolerance<RealT>();

    const RealT probs[] = { static_cast<RealT>(0.2L), static_cast<RealT>(0.3L), static_cast<RealT>(0.5L) };
    const RealT rates[] = { static_cast<RealT>(0.5L), static_cast<RealT>(1.0L), static_cast<RealT>(1.5L) };
    const std::size_t n = sizeof(probs)/sizeof(RealT);

    boost::math::hyperexponential_distribution<RealT> dist(probs, probs+n, rates, rates+n);

    // Mathematica: Table[N[CDF[HyperexponentialDistribution[{1/5, 3/10, 1/2}, {1/2, 1, 3/2}], x], 35], {x, 0, 4}]
    BOOST_CHECK_CLOSE( boost::math::cdf(dist, static_cast<RealT>(0)), static_cast<RealT>(0), tol );
    BOOST_CHECK_CLOSE( boost::math::cdf(dist, static_cast<RealT>(1)), static_cast<RealT>(0.65676495563182570433394272657131939L), tol );
    BOOST_CHECK_CLOSE( boost::math::cdf(dist, static_cast<RealT>(2)), static_cast<RealT>(0.86092999261079575662302418965093162L), tol );
    BOOST_CHECK_CLOSE( boost::math::cdf(dist, static_cast<RealT>(3)), static_cast<RealT>(0.93488334919083369807146961400871370L), tol );
    BOOST_CHECK_CLOSE( boost::math::cdf(dist, static_cast<RealT>(4)), static_cast<RealT>(0.96619887559772402832156211090812241L), tol );
}


BOOST_AUTO_TEST_CASE_TEMPLATE(quantile, RealT, test_types)
{
    const RealT tol = make_tolerance<RealT>();

    const RealT probs[] = { static_cast<RealT>(0.2L), static_cast<RealT>(0.3L), static_cast<RealT>(0.5L) };
    const RealT rates[] = { static_cast<RealT>(0.5L), static_cast<RealT>(1.0L), static_cast<RealT>(1.5L) };
    const std::size_t n = sizeof(probs)/sizeof(RealT);

    boost::math::hyperexponential_distribution<RealT> dist(probs, probs+n, rates, rates+n);

    // Mathematica: Table[N[Quantile[HyperexponentialDistribution[{1/5, 3/10, 1/2}, {1/2, 1, 3/2}], p], 35], {p, {0.`35, 0.6567649556318257043339427265713193884067872189124925936717`35, 0.8609299926107957566230241896509316171726985139265620607067`35, 0.9348833491908336980714696140087136988562861627183715044229`35, 0.9661988755977240283215621109081224127091468307592751727719`35}}]
    BOOST_CHECK_CLOSE( boost::math::quantile(dist, static_cast<RealT>(0)), static_cast<RealT>(0), tol );
    BOOST_CHECK_CLOSE( boost::math::quantile(dist, static_cast<RealT>(0.65676495563182570433394272657131939L)), static_cast<RealT>(1), tol );
    BOOST_CHECK_CLOSE( boost::math::quantile(dist, static_cast<RealT>(0.86092999261079575662302418965093162L)), static_cast<RealT>(2), tol );
    BOOST_CHECK_CLOSE( boost::math::quantile(dist, static_cast<RealT>(0.93488334919083369807146961400871370L)), static_cast<RealT>(3), tol );
    BOOST_CHECK_CLOSE( boost::math::quantile(dist, static_cast<RealT>(0.96619887559772402832156211090812241L)), static_cast<RealT>(4), tol );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(ccdf, RealT, test_types)
{
    const RealT tol = make_tolerance<RealT>();

    const RealT probs[] = { static_cast<RealT>(0.2L), static_cast<RealT>(0.3L), static_cast<RealT>(0.5L) };
    const RealT rates[] = { static_cast<RealT>(0.5L), static_cast<RealT>(1.0L), static_cast<RealT>(1.5L) };
    const std::size_t n = sizeof(probs)/sizeof(RealT);

    boost::math::hyperexponential_distribution<RealT> dist(probs, probs+n, rates, rates+n);

    // Mathematica: Table[N[SurvivalFunction[HyperexponentialDistribution[{1/5, 3/10, 1/2}, {1/2, 1, 3/2}], x], 35], {x, 0, 4}]
    BOOST_CHECK_CLOSE( boost::math::cdf(boost::math::complement(dist, static_cast<RealT>(0))), static_cast<RealT>(1), tol );
    BOOST_CHECK_CLOSE( boost::math::cdf(boost::math::complement(dist, static_cast<RealT>(1))), static_cast<RealT>(0.34323504436817429566605727342868061L), tol );
    BOOST_CHECK_CLOSE( boost::math::cdf(boost::math::complement(dist, static_cast<RealT>(2))), static_cast<RealT>(0.13907000738920424337697581034906838L), tol );
    BOOST_CHECK_CLOSE( boost::math::cdf(boost::math::complement(dist, static_cast<RealT>(3))), static_cast<RealT>(0.065116650809166301928530385991286301L), tol );
    BOOST_CHECK_CLOSE( boost::math::cdf(boost::math::complement(dist, static_cast<RealT>(4))), static_cast<RealT>(0.033801124402275971678437889091877587L), tol );
}


BOOST_AUTO_TEST_CASE_TEMPLATE(cquantile, RealT, test_types)
{
    const RealT tol = make_tolerance<RealT>();

    const RealT probs[] = { static_cast<RealT>(0.2L), static_cast<RealT>(0.3L), static_cast<RealT>(0.5L) };
    const RealT rates[] = { static_cast<RealT>(0.5L), static_cast<RealT>(1.0L), static_cast<RealT>(1.5L) };
    const std::size_t n = sizeof(probs) / sizeof(RealT);

    boost::math::hyperexponential_distribution<RealT> dist(probs, probs+n, rates, rates+n);

    // Mathematica: Table[N[InverseSurvivalFunction[HyperexponentialDistribution[{1/5, 3/10, 1/2}, {1/2, 1, 3/2}], p], 35], {p, {1.`35, 0.3432350443681742956660572734286806115932127810875074063283`35, 0.1390700073892042433769758103490683828273014860734379392933`35, 0.0651166508091663019285303859912863011437138372816284955771`35, 0.0338011244022759716784378890918775872908531692407248272281`35}}]
    BOOST_CHECK_CLOSE( boost::math::quantile(boost::math::complement(dist, static_cast<RealT>(1))), static_cast<RealT>(0), tol );
    BOOST_CHECK_CLOSE( boost::math::quantile(boost::math::complement(dist, static_cast<RealT>(0.34323504436817429566605727342868061L))), static_cast<RealT>(1), tol );
    BOOST_CHECK_CLOSE( boost::math::quantile(boost::math::complement(dist, static_cast<RealT>(0.13907000738920424337697581034906838L))), static_cast<RealT>(2), tol );
    BOOST_CHECK_CLOSE( boost::math::quantile(boost::math::complement(dist, static_cast<RealT>(0.065116650809166301928530385991286301L))), static_cast<RealT>(3), tol );
    BOOST_CHECK_CLOSE( boost::math::quantile(boost::math::complement(dist, static_cast<RealT>(0.033801124402275971678437889091877587L))), static_cast<RealT>(4), tol );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mean, RealT, test_types)
{
    const RealT tol = make_tolerance<RealT>();

    const RealT probs[] = { static_cast<RealT>(0.2L), static_cast<RealT>(0.3L), static_cast<RealT>(0.5L) };
    const RealT rates[] = { static_cast<RealT>(0.5L), static_cast<RealT>(1.0L), static_cast<RealT>(1.5L) };
    const std::size_t n = sizeof(probs) / sizeof(RealT);

    boost::math::hyperexponential_distribution<RealT> dist(probs, probs+n, rates, rates+n);

    // Mathematica: N[Mean[HyperexponentialDistribution[{1/5, 3/10, 1/2}, {1/2, 1, 3/2}]], 35]
    BOOST_CHECK_CLOSE( boost::math::mean(dist), static_cast<RealT>(1.0333333333333333333333333333333333L), tol );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(variance, RealT, test_types)
{
    const RealT tol = make_tolerance<RealT>();

    const RealT probs[] = { static_cast<RealT>(0.2L), static_cast<RealT>(0.3L), static_cast<RealT>(0.5L) };
    const RealT rates[] = { static_cast<RealT>(0.5L), static_cast<RealT>(1.0L), static_cast<RealT>(1.5L) };
    const std::size_t n = sizeof(probs) / sizeof(RealT);

    boost::math::hyperexponential_distribution<RealT> dist(probs, probs+n, rates, rates+n);

    // Mathematica: N[Variance[HyperexponentialDistribution[{1/5, 3/10, 1/2}, {1/2, 1, 3/2}]], 35]
    BOOST_CHECK_CLOSE( boost::math::variance(dist), static_cast<RealT>(1.5766666666666666666666666666666667L), tol );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(kurtosis, RealT, test_types)
{
    const RealT tol = make_tolerance<RealT>();

    const RealT probs[] = { static_cast<RealT>(0.2L), static_cast<RealT>(0.3L), static_cast<RealT>(0.5L) };
    const RealT rates[] = { static_cast<RealT>(0.5L), static_cast<RealT>(1.0L), static_cast<RealT>(1.5L) };
    const std::size_t n = sizeof(probs) / sizeof(RealT);

    boost::math::hyperexponential_distribution<RealT> dist(probs, probs+n, rates, rates+n);

    // Mathematica: N[Kurtosis[HyperexponentialDistribution[{1/5, 3/10, 1/2}, {1/2, 1, 3/2}]], 35]
    BOOST_CHECK_CLOSE( boost::math::kurtosis(dist), static_cast<RealT>(19.750738616808728416968743435138046L), tol );
    // Mathematica: N[Kurtosis[HyperexponentialDistribution[{1/5, 3/10, 1/2}, {1/2, 1, 3/2}] - 3.`35], 35]
    BOOST_CHECK_CLOSE( boost::math::kurtosis_excess(dist), static_cast<RealT>(16.750738616808728416968743435138046L), tol );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(skewness, RealT, test_types)
{
    const RealT tol = make_tolerance<RealT>();

    const RealT probs[] = { static_cast<RealT>(0.2L), static_cast<RealT>(0.3L), static_cast<RealT>(0.5L) };
    const RealT rates[] = { static_cast<RealT>(0.5L), static_cast<RealT>(1.0L), static_cast<RealT>(1.5L) };
    const std::size_t n = sizeof(probs) / sizeof(RealT);

    boost::math::hyperexponential_distribution<RealT> dist(probs, probs+n, rates, rates+n);

    // Mathematica: N[Skewness[HyperexponentialDistribution[{1/5, 3/10, 1/2}, {1/2, 1, 3/2}]], 35]
    BOOST_CHECK_CLOSE( boost::math::skewness(dist), static_cast<RealT>(3.1811387449963809211146099116375685L), tol );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mode, RealT, test_types)
{
    const RealT tol = make_tolerance<RealT>();

    const RealT probs[] = { static_cast<RealT>(0.2L), static_cast<RealT>(0.3L), static_cast<RealT>(0.5L) };
    const RealT rates[] = { static_cast<RealT>(0.5L), static_cast<RealT>(1.0L), static_cast<RealT>(1.5L) };
    const std::size_t n = sizeof(probs) / sizeof(RealT);

    boost::math::hyperexponential_distribution<RealT> dist(probs, probs+n, rates, rates+n);

    BOOST_CHECK_CLOSE( boost::math::mode(dist), static_cast<RealT>(0), tol );
}

