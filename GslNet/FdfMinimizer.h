#pragma once

#include <gsl/gsl_multimin.h>

#include "GslVector.h"
#include "Algorithms.h"

using namespace System::Runtime::InteropServices;

namespace GslNet
{
	namespace MultiMin
	{
		public delegate double FFunction(GslVector ^vector);
		public delegate void DFFunction(GslVector ^vector, GslVector ^result);

		[UnmanagedFunctionPointer(CallingConvention::Cdecl)]
		delegate double _fDelegate(const gsl_vector *x, void *params);
		[UnmanagedFunctionPointer(CallingConvention::Cdecl)]
		delegate void _dfDelegate(const gsl_vector *x, void *params, gsl_vector *g);
		[UnmanagedFunctionPointer(CallingConvention::Cdecl)]
		delegate void _fdfDelegate(const gsl_vector *x, void *params, double *f, gsl_vector *g);
			
		ref class FunctionWrapper
		{
			FFunction ^_function;
			DFFunction ^_gradient;
		public:
			FunctionWrapper(FFunction ^function, DFFunction ^gradient)
			{
				_function = function;
				_gradient = gradient;
			}

			double f(const gsl_vector *x, void *params)
			{
				return _function(gcnew GslVector(const_cast<gsl_vector*>(x)));
			}

			void df(const gsl_vector *x, void *params, gsl_vector *g)
			{
				_gradient(gcnew GslVector(const_cast<gsl_vector*>(x)), gcnew GslVector(g));
			}

			void fdf(const gsl_vector *x, void *params, double *f, gsl_vector *g)
			{
				*f = _function(gcnew GslVector(const_cast<gsl_vector*>(x)));
				_gradient(gcnew GslVector(const_cast<gsl_vector*>(x)), gcnew GslVector(g));
			}
		};

		public ref class Fdf
		{
		internal:
			gsl_multimin_function_fdf *_fdf;
			FunctionWrapper ^_wrapper;
			_fDelegate ^_fD;
			_dfDelegate ^_dfD;
			_fdfDelegate ^_fdfD;
		public:
			Fdf(FFunction ^function, DFFunction ^gradient, int n)
			{
				_wrapper = gcnew FunctionWrapper(function, gradient);
				_fdf = new gsl_multimin_function_fdf;

				_fD = gcnew _fDelegate(_wrapper, &FunctionWrapper::f);
				_fdf->f =
					(double(*)(const gsl_vector*, void*))Marshal::GetFunctionPointerForDelegate(
						_fD).ToPointer();
				_dfD = gcnew _dfDelegate(_wrapper, &FunctionWrapper::df);
				_fdf->df =
					(void(*)(const gsl_vector*, void*, gsl_vector*))Marshal::GetFunctionPointerForDelegate(
						_dfD).ToPointer();
				_fdfD = gcnew _fdfDelegate(_wrapper, &FunctionWrapper::fdf);
				_fdf->fdf =
					(void(*)(const gsl_vector*, void*, double*, gsl_vector*))Marshal::GetFunctionPointerForDelegate(
						_fdfD).ToPointer();
				_fdf->n = n;
				_fdf->params = 0;
			}

			~Fdf()
			{
				delete _fdf;
			}
		};

		public ref class FdfMinimizer : public System::IDisposable
		{
			gsl_multimin_fdfminimizer *_pointer;
		public:
			FdfMinimizer(AlgorithmWithDerivatives algorithm, int size);

			void Iterate()
			{
				int result = gsl_multimin_fdfminimizer_iterate(_pointer);
				if(result)
				{
					throw gcnew System::Exception(gcnew System::String(gsl_strerror(result)));
				}
			}

			int Initialize(Fdf ^fdf, GslVector ^x, double stepSize, double tol)
			{
				return gsl_multimin_fdfminimizer_set(_pointer, fdf->_fdf, x->_pointer, stepSize, tol);
			}

			bool TestGradient(double eps)
			{
				return gsl_multimin_test_gradient(_pointer->gradient, eps) == GSL_SUCCESS;
			}

			property GslVector ^X
			{
				GslVector ^get()
				{
					return gcnew GslVector(gsl_multimin_fdfminimizer_x(_pointer));
				}
			}

			property const char *Name
			{
				const char *get()
				{
					return gsl_multimin_fdfminimizer_name(_pointer);
				}
			}

			~FdfMinimizer();
		};
	}
}