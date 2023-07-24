#pragma once
#include <immintrin.h>
#include <type_traits>

template <typename instanceType>
instanceType SqrtSIMD(instanceType value)
{
    using SIMDType = typename std::conditional<
        std::is_same<instanceType, float>::value, __m128,
        typename std::conditional<std::is_same<instanceType, double>::value, __m256,
        __m128i>::type>::type;
    using ScalarType =
        typename std::conditional<std::is_same<instanceType, float>::value, float,
        typename std::conditional<std::is_same<instanceType, double>::value, double, int>::type>::type;

    SIMDType input;
    SIMDType result;

    if constexpr (std::is_same<instanceType, float>::value)
    {
        input = _mm_set_ps(value, value, value, value);
        result = _mm_sqrt_ps(input);
    }
    else if constexpr (std::is_same<instanceType, double>::value)
    {
        input = _mm256_set1_pd(value);
        result = _mm256_sqrt_pd(input);
    }
    else if constexpr (std::is_same<instanceType, int>::value)
    {
        input = _mm_set_epi32(value, value, value, value);
        result = _mm_cvtepi32_ps(input);
        result = _mm_sqrt_ps(result);
    }

    ScalarType scalarResult[4];
    if constexpr (std::is_same<instanceType, int>::value)
        _mm_storeu_si128((__m128i*)scalarResult, _mm_castps_si128(result));
    else
        _mm_storeu_ps(reinterpret_cast<float*>(scalarResult), result);

    return scalarResult[0];
}

template <typename instanceType>
instanceType AcosSIMD(instanceType value)
{
    using SIMDType = typename std::conditional<
        std::is_same<instanceType, float>::value, __m128,
        typename std::conditional<std::is_same<instanceType, double>::value, __m256,
        __m128i>::type>::type;
    using ScalarType =
        typename std::conditional<std::is_same<instanceType, float>::value, float,
        typename std::conditional<std::is_same<instanceType, double>::value, double, int>::type>::type;

    SIMDType input;
    SIMDType result;

    if constexpr (std::is_same<instanceType, float>::value)
    {
        input = _mm_set_ps(value, value, value, value);
        result = _mm_acos_ps(input);
    }
    else if constexpr (std::is_same<instanceType, double>::value)
    {
        input = _mm256_set1_pd(value);
        result = _mm256_acos_pd(input);
    }
    else if constexpr (std::is_same<instanceType, int>::value)
    {
        input = _mm_set_epi32(value, value, value, value);
        __m128 resultFloat = _mm_cvtepi32_ps(input);
        result = _mm_acos_ps(resultFloat);
    }

    ScalarType scalarResult[4];
    if constexpr (std::is_same<instanceType, int>::value)
        _mm_storeu_si128((__m128i*)scalarResult, _mm_castps_si128(result));
    else
        _mm_storeu_ps(reinterpret_cast<float*>(scalarResult), result);

    return scalarResult[0];
}

template <typename instanceType>
instanceType AbsSIMD(instanceType value)
{
    // Determine the appropriate SIMD and scalar types based on the input type
    using SIMDType = typename std::conditional<
        std::is_same<instanceType, float>::value, __m128,
        typename std::conditional<std::is_same<instanceType, double>::value, __m256,
        __m128i>::type>::type;
    using ScalarType =
        typename std::conditional<std::is_same<instanceType, float>::value, float,
        typename std::conditional<std::is_same<instanceType, double>::value, double, int>::type>::type;

    SIMDType input;  // SIMD variable to hold the input value
    SIMDType result; // SIMD variable to hold the absolute value

    if constexpr (std::is_same<instanceType, float>::value)
    {
        // Set all elements of the SIMD input to the input value
        input = _mm_set_ps(value, value, value, value);
        // Compute the absolute value using bitwise AND and a sign-clearing mask
        result = _mm_and_ps(input, _mm_castsi128_ps(_mm_set1_epi32(~0x80000000)));
    }
    else if constexpr (std::is_same<instanceType, double>::value)
    {
        // Set all elements of the SIMD input to the input value
        input = _mm256_set1_pd(value);
        // Compute the absolute value using bitwise AND and a sign-clearing mask
        result = _mm256_and_pd(input, _mm256_castsi256_pd(_mm256_set1_epi64x(~0x8000000000000000)));
    }
    else if constexpr (std::is_same<instanceType, int>::value)
    {
        // Set all elements of the SIMD input to the input value
        input = _mm_set_epi32(value, value, value, value);
        // Compute the absolute value using the SSE2 _mm_abs_epi32 intrinsic
        result = _mm_abs_epi32(input);
    }

    ScalarType scalarResult[4];  // Array to store the scalar values from the SIMD result

    if constexpr (std::is_same<instanceType, int>::value)
        // Store the SIMD result into the scalarResult array
        _mm_storeu_si128((__m128i*)scalarResult, result);
    else
        // Store the SIMD result into the scalarResult array
        _mm_storeu_ps(reinterpret_cast<float*>(scalarResult), result);

    return scalarResult[0];  // Return the first element of the scalarResult array
}

namespace math
{

    template <typename T>
    constexpr const T& clamp(const T& value, const T& min, const T& max)
    {
        return (value < min) ? min : ((value > max) ? max : value);
    }

}

template <typename T>
T SinNonSIMD(T value)
{
    // Implementation of sin without using the standard library or external math libraries
    // This implementation can be replaced with your own custom algorithm

    // Placeholder implementation using Taylor series expansion
    T result = value;
    T term = value;
    T sign = -1;
    T denominator = 2;

    for (int i = 1; i < 10; ++i) {
        term *= (value * value) / (denominator * (denominator + 1));
        result += sign * term;
        sign *= -1;
        denominator += 2;
    }

    return result;
}

template <typename T>
T CosNonSIMD(T value)
{
    // Implementation of cos without using the standard library or external math libraries

    // Placeholder implementation using Taylor series expansion
    T result = 1;
    T term = 1;
    T sign = -1;
    T denominator = 2;

    for (int i = 1; i < 10; ++i) {
        term *= (value * value) / (denominator * (denominator - 1));
        result += sign * term;
        sign *= -1;
        denominator += 2;
    }

    return result;
}


template <typename T>
__forceinline T invert(T From, T& OperatingValue)
{
    return From - OperatingValue;
}

__forceinline float mapValue(float value, float minInput, float maxInput, float minOutput, float maxOutput)
{
    return ((value - minInput) / (maxInput - minInput)) * (maxOutput - minOutput) + minOutput;
}
