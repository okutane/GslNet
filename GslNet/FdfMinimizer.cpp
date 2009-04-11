#include "StdAfx.h"
#include "FdfMinimizer.h"

namespace GslNet
{
	namespace MultiMin
	{
		FdfMinimizer::FdfMinimizer(AlgorithmWithDerivatives algorithm, int size)
		{
			_pointer = gsl_multimin_fdfminimizer_alloc(algorithm._type, size);
		}

		FdfMinimizer::~FdfMinimizer()
		{
			gsl_multimin_fdfminimizer_free(_pointer);
		}
	}
}
