#include <sm/eigen/assert_macros.hpp>

namespace aslam {
  namespace backend {

    template <typename MEstimatorType>
    boost::shared_ptr<MEstimatorType> ScalarNonSquaredErrorTerm::getMEstimatorPolicy() {
      return boost::dynamic_pointer_cast<MEstimatorType>(_mEstimatorPolicy);
    }

    template<typename ITERATOR_T>
    void ScalarNonSquaredErrorTerm::setDesignVariablesIterator(ITERATOR_T start, ITERATOR_T end)
    {
      /// \todo Set the back link to the error term in the design variable.
      SM_ASSERT_EQ(aslam::UnsupportedOperationException, _designVariables.size(), 0, "The design variable container already has objects. The design variables may only be set once");
      /// \todo: set the back-link in the design variable.
      int ii = 0;
      for (ITERATOR_T i = start; i != end; ++i, ++ii) {
        SM_ASSERT_TRUE(aslam::InvalidArgumentException, *i != NULL, "Design variable " << ii << " is null");
      }
      _designVariables.insert(_designVariables.begin(), start, end);
    }

    double ScalarNonSquaredErrorTerm::getWeight() const
    {
      return _w;
    }

    void ScalarNonSquaredErrorTerm::setWeight(const double w)
    {
      _w = w;
    }

    void ScalarNonSquaredErrorTerm::getWeightedJacobians(JacobianContainer& outJc, bool useMEstimator)
    {
      // take a copy. \todo Don't take a copy.
      evaluateJacobians(outJc);
      Eigen::Matrix<double, 1, 1> w;
      w << _w;
      outJc.applyChainRule(w);
      JacobianContainer::map_t::iterator it = outJc.begin();
      double mEstWeight = 1.0;
      if (useMEstimator)
        mEstWeight = _mEstimatorPolicy->getWeight(getRawError());
      for (; it != outJc.end(); ++it) {
        it->second *=  mEstWeight * it->first->scaling();
      }
    }

   } // namespace backend
} // namespace aslam
