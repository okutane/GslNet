#pragma once

#include <gsl/gsl_multimin.h>

namespace GslNet
{
	namespace MultiMin
	{
		public value struct AlgorithmWithoutDerivatives
		{
		internal:
			const gsl_multimin_fminimizer_type *_type;
		private:
			AlgorithmWithoutDerivatives(const gsl_multimin_fminimizer_type *type);
		public:
			static const AlgorithmWithoutDerivatives NMSimplex =
				AlgorithmWithoutDerivatives(gsl_multimin_fminimizer_nmsimplex);
		};

		public value struct AlgorithmWithDerivatives
		{
		internal:
			const gsl_multimin_fdfminimizer_type *_type;
		private:
			AlgorithmWithDerivatives(const gsl_multimin_fdfminimizer_type *type);
		public:
			static AlgorithmWithDerivatives SteepestDescent =
				AlgorithmWithDerivatives(gsl_multimin_fdfminimizer_steepest_descent);
			static const AlgorithmWithDerivatives ConjugatePR =
				AlgorithmWithDerivatives(gsl_multimin_fdfminimizer_conjugate_pr);
			/*static const AlgorithmWithDerivatives ConjugateFR = gsl_multimin_fdfminimizer_conjugate_fr;
			static const AlgorithmWithDerivatives VectorBfgs = gsl_multimin_fdfminimizer_vector_bfgs;
			static const AlgorithmWithDerivatives VectorBfgs2 = gsl_multimin_fdfminimizer_vector_bfgs2;*/
		};
	}
}
