#pragma once

#include <gsl/gsl_vector.h>

namespace GslNet
{
	public ref class GslVector
	{
	internal:
		gsl_vector *_pointer;

		GslVector(gsl_vector *pointer)
		{
			_pointer = pointer;
		}
	public:
		GslVector(int size)
		{
			_pointer = gsl_vector_alloc(size);
		}

		~GslVector()
		{
			gsl_vector_free(_pointer);
		}

		property double default[int]
		{
			double get(int index)
			{
				return gsl_vector_get(_pointer, index);
			}

			void set(int index, double value)
			{
				gsl_vector_set(_pointer, index, value);
			}
		}
	};
}
