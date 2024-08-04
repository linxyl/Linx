#pragma once

#include <complex>
#include <memory>
#include <type_traits>

#include "Linx/Utils/TypeTraits.h"
#include "Linx/Math/MathUtils.h"

namespace Linx
{
	/**
	 * Perform FFT calculations on complex numbers.
	 * 
	 * @template N	FFT points.
	 * @param ItIn	The first iterator of the input.
	 * @param ItOut	The first iterator of the output.
	 */
	template<size_t N, typename InType, typename OutType,
		typename = std::enable_if<is_complex_iterator_v<InType> && is_complex_iterator_v<OutType>>>
	static void FFT(InType ItIn, OutType ItOut)
	{
		// bit reversal
		size_t pos[N];
		pos[0] = 0;
		ItOut[0] = ItIn[0];
		for (int i = 1; i < N; i++)
		{
			pos[i] = pos[i / 2] / 2 + (i & 1) * N / 2;
			ItOut[i] = ItIn[pos[i]];
		}

		// butterfly diagram
		for (int k = 2; k <= N; k *= 2)
		{
			for (int i = 0; i < N; i += k)
			{
				std::complex<double> W(cos(PI * 2 / k), -sin(PI * 2 / k));
				std::complex<double> Wk = 1;
				for (int j = 0; j < k / 2; j++)
				{
					std::complex<double> x = ItOut[i + j], y = ItOut[i + j + k / 2] * Wk;
					ItOut[i + j] = x + y;
					ItOut[i + j + k / 2] = x - y;
					Wk *= W;
				}
			}
		}
	}

	/**
	 * Perform FFT calculations on complex numbers.
	 *
	 * @template N	FFT points.
	 * @param ItRe	The first iterator of the real part input.
	 * @param ItIm	The first iterator of the imaginary part input.
	 * @param ItOut	The first iterator of the output.
	 */
	template<size_t N, typename InType, typename OutType,
		typename = std::enable_if<is_complex_iterator_v<OutType>>>
	static void FFT(InType ItRe, InType ItIm, OutType ItOut)
	{
		// bit reversal
		size_t pos[N];
		pos[0] = 0;
		ItOut[0] = std::complex<iterator_traits<InType>::value_type>(ItRe[0], ItIm[0]);
		for (int i = 1; i < N; i++)
		{
			pos[i] = pos[i / 2] / 2 + (i & 1) * N / 2;
			ItOut[i] = std::complex<iterator_traits<InType>::value_type>(ItRe[pos[i]], ItIm[pos[i]]);
		}

		// butterfly diagram
		for (int k = 2; k <= N; k *= 2)
		{
			for (int i = 0; i < N; i += k)
			{
				std::complex<double> W(cos(PI * 2 / k), -sin(PI * 2 / k));
				std::complex<double> Wk = 1;
				for (int j = 0; j < k / 2; j++)
				{
					std::complex<double> x = ItOut[i + j], y = ItOut[i + j + k / 2] * Wk;
					ItOut[i + j] = x + y;
					ItOut[i + j + k / 2] = x - y;
					Wk *= W;
				}
			}
		}
	}

	/**
	 * Perform IFFT calculations on complex numbers.
	 *
	 * @template N	IFFT points.
	 * @param ItIn	The first iterator of the input.
	 * @param ItOut	The first iterator of the output.
	 */
	template<size_t N, typename InType, typename OutType,
		typename = std::enable_if<is_complex_iterator_v<InType>&& is_complex_iterator_v<OutType>>>
	static void IFFT(InType ItIn, OutType ItOut)
	{
		// bit reversal
		size_t pos[N];
		pos[0] = 0;
		ItOut[0] = ItIn[0];
		for (int i = 1; i < N; i++)
		{
			pos[i] = pos[i / 2] / 2 + (i & 1) * N / 2;
			ItOut[i] = ItIn[pos[i]];
		}

		// butterfly diagram
		for (int k = 2; k <= N; k *= 2)
		{
			for (int i = 0; i < N; i += k)
			{
				std::complex<double> W(cos(PI * 2 / k), sin(PI * 2 / k));
				std::complex<double> Wk = 1;
				for (int j = 0; j < k / 2; j++)
				{
					std::complex<double> x = ItOut[i + j], y = ItOut[i + j + k / 2] * Wk;
					ItOut[i + j] = x + y;
					ItOut[i + j + k / 2] = x - y;
					Wk *= W;
				}
			}
		}
	}

	/**
	 * Perform IFFT calculations on complex numbers.
	 *
	 * @template N	IFFT points.
	 * @param ItRe	The first iterator of the real part input.
	 * @param ItIm	The first iterator of the imaginary part input.
	 * @param ItOut	The first iterator of the output.
	 */
	template<size_t N, typename InType, typename OutType,
		typename = std::enable_if<is_complex_iterator_v<OutType>>>
	static void IFFT(InType ItRe, InType ItIm, OutType ItOut)
	{
		// bit reversal
		size_t pos[N];
		pos[0] = 0;
		ItOut[0] = std::complex<iterator_traits<InType>::value_type>(ItRe[0], ItIm[0]);
		for (int i = 1; i < N; i++)
		{
			pos[i] = pos[i / 2] / 2 + (i & 1) * N / 2;
			ItOut[i] = std::complex<iterator_traits<InType>::value_type>(ItRe[pos[i]], ItIm[pos[i]]);
		}

		// butterfly diagram
		for (int k = 2; k <= N; k *= 2)
		{
			for (int i = 0; i < N; i += k)
			{
				std::complex<double> W(cos(PI * 2 / k), sin(PI * 2 / k));
				std::complex<double> Wk = 1;
				for (int j = 0; j < k / 2; j++)
				{
					std::complex<double> x = ItOut[i + j], y = ItOut[i + j + k / 2] * Wk;
					ItOut[i + j] = x + y;
					ItOut[i + j + k / 2] = x - y;
					Wk *= W;
				}
			}
		}
	}

	/**
	 * Calculated amplitude-frequency characteristics of the spectrum.
	 * 
	 * @param InFirst	The first iterator of the input.
	 * @param InLast	The last iterator of the input.
	 * @param OutIt		The first iterator of the output.
	 */
	template<typename InType, typename OutType,
		typename = std::enable_if<is_complex_iterator_v<InType>>>
	inline void SpectrumToAmp(InType InFirst, InType InLast, OutType OutIt)
	{
		for (;InFirst != InLast; ++InFirst)
		{
			*(OutIt++) = abs(*InFirst);
		}
	}

	/**
	 * Calculated the square of the amplitude-frequency characteristics of the spectrum. Has better performance.
	 *
	 * @param InFirst	The first iterator of the input.
	 * @param InLast	The last iterator of the input.
	 * @param OutIt		The first iterator of the output.
	 */
	template<typename InType, typename OutType,
		typename = std::enable_if<is_complex_iterator_v<InType>>>
	inline void SpectrumToAmpSquare(InType InFirst, InType InLast, OutType OutIt)
	{
		for (; InFirst != InLast; ++InFirst)
		{
			*(OutIt++) = norm(*InFirst);
		}
	}

	/**
	 * Calculated phase-frequency characteristics of the spectrum.
	 *
	 * @param InFirst	The first iterator of the input.
	 * @param InLast	The last iterator of the input.
	 * @param OutIt		The first iterator of the output.
	 */
	template<typename InType, typename OutType,
		typename = std::enable_if<is_complex_iterator_v<InType>>>
	inline void SpectrumToPhase(InType InFirst, InType InLast, OutType OutIt)
	{
		for (; InFirst != InLast; ++InFirst)
		{
			*(OutIt++) = std::arg(*InFirst);
		}
	}
}