#pragma once

#include "GslVector.h"

namespace GslNet
{
	namespace MultiMin
	{
		public interface class IFunction
		{
			double Evaluate(GslVector ^argument);
		};

		public interface class IFunctionWithGradient : IFunction
		{
			void EvaluateGradient(GslVector ^argument, GslVector ^result);
			double EvaluateValueWithGradient(GslVector ^argument, GslVector ^result);
		};
	}
}
