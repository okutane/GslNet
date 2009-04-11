#include "StdAfx.h"
#include "Algorithms.h"

namespace GslNet
{
	namespace MultiMin
	{
		AlgorithmWithoutDerivatives::AlgorithmWithoutDerivatives(const gsl_multimin_fminimizer_type *type) :
			_type(type)
		{
		}

		AlgorithmWithDerivatives::AlgorithmWithDerivatives(const gsl_multimin_fdfminimizer_type *type) :
			_type(type)
		{
		}
	}
}
