using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using NUnit.Framework;

using GslNet.MultiMin;

namespace GslNet.Tests.MultiMin
{
    [TestFixture]
    public class FdfMinimizerTests
    {
        [Test]
        public void TestMinimize()
        {
            GslVector vector = new GslVector(2);
            vector[0] = 1;
            vector[1] = 1;
            FdfMinimizer minimizer = new FdfMinimizer(AlgorithmWithDerivatives.ConjugatePR, 2);
            Fdf fdf = new Fdf(v => v[0] * v[0] + v[1] * v[1], delegate(GslVector arg, GslVector result)
            {
                result[0] = 2 * arg[0];
                result[1] = 2 * arg[1];
            }, 2);
            minimizer.Initialize(fdf, vector, 1, 0.1);
            for (int i = 0; i < 2; i++)
            {
                minimizer.Iterate();
            }
            Assert.AreEqual(0, minimizer.X[0], 1e-15);
            Assert.AreEqual(0, minimizer.X[1], 1e-15);
        }
    }
}
