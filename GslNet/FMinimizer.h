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

		[UnmanagedFunctionPointer(CallingConvention::Cdecl)]
		delegate double _fDelegate(const gsl_vector *x, void *params);
			

		public ref class F
		{
			ref class FunctionWrapper
			{
				FFunction ^_function;
			public:
				FunctionWrapper(FFunction ^function)
				{
					_function = function;
				}

				double f(const gsl_vector *x, void *params)
				{
					return _function(gcnew GslVector(const_cast<gsl_vector*>(x)));
				}
			};

		internal:
			gsl_multimin_function *_f;
			FunctionWrapper ^_wrapper;
			_fDelegate ^_fD;
		public:
			F(FFunction ^function, int n)
			{
				_wrapper = gcnew FunctionWrapper(function);
				_f = new gsl_multimin_function;

				_fD = gcnew _fDelegate(_wrapper, &FunctionWrapper::f);
				_f->f =
					(double(*)(const gsl_vector*, void*))Marshal::GetFunctionPointerForDelegate(
						_fD).ToPointer();
				_f->n = n;
				_f->params = 0;
			}

			~F()
			{
				delete _f;
			}
		};

		public ref class FMinimizer : public System::IDisposable
		{
			gsl_multimin_fminimizer *_pointer;
		public:
			FMinimizer(AlgorithmWithoutDerivatives algorithm, int size)
			{
				_pointer = gsl_multimin_fminimizer_alloc(algorithm._type, size);
			}

			void Iterate()
			{
				int result = gsl_multimin_fminimizer_iterate(_pointer);
				if(result != 0)
				{
					throw gcnew System::Exception("Exception in FMinimizer.Iterate");
				}
			}

			int Initialize(F ^f, GslVector ^x, double stepSize)
			{
				gsl_vector *stepSizes = gsl_vector_calloc(x->_pointer->size);
				gsl_vector_add_constant(stepSizes, stepSize);
				int result = gsl_multimin_fminimizer_set(_pointer, f->_f, x->_pointer, stepSizes);
				gsl_vector_free(stepSizes);
				return result;
			}

			bool TestSize(double eps)
			{
				return gsl_multimin_test_size(_pointer->size, eps) == GSL_SUCCESS;
			}

			property GslVector ^X
			{
				GslVector ^get()
				{
					return gcnew GslVector(_pointer->x);
				}
			}

			property const char *Name
			{
				const char *get()
				{
					return gsl_multimin_fminimizer_name(_pointer);
				}
			}

			~FMinimizer()
			{
				gsl_multimin_fminimizer_free(_pointer);
			}
		};
	}
}