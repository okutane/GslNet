using GslNet.MultiMin;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace GslNet.Tests.MultiMin
{
    [TestClass]
    public class FdfMinimizerTests
    {
        [TestMethod]
        public void TestMinimize()
        {
            GslVector vector = new GslVector(2);
            vector[0] = 1;
            vector[1] = 1;
            FdfMinimizer minimizer = new FdfMinimizer(AlgorithmWithDerivatives.ConjugatePR, 2);
            Fdf fdf = new Fdf(new Circle(), 2);
            minimizer.Initialize(fdf, vector, 1, 0.1);
            for (int i = 0; i < 2; i++)
            {
                minimizer.Iterate();
            }
            Assert.AreEqual(0, minimizer.X[0], 1e-15);
            Assert.AreEqual(0, minimizer.X[1], 1e-15);
        }

        private class Circle : IFunctionWithGradient
        {
            public double Evaluate(GslVector argument)
            {
                return argument[0] * argument[0] + argument[1] * argument[1];
            }

            public void EvaluateGradient(GslVector argument, GslVector result)
            {
                result[0] = 2 * argument[0];
                result[1] = 2 * argument[1];
            }

            public double EvaluateValueWithGradient(GslVector argument, GslVector result)
            {
                EvaluateGradient(argument, result);
                return Evaluate(argument);
            }
        }
    }
}
