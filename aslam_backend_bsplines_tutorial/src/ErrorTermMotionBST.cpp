#include <aslam/backend/ErrorTermMotionBST.hpp>

namespace aslam {
  namespace backend {
    
    ErrorTermMotionBST::ErrorTermMotionBST(aslam::backend::VectorExpression<1> robotVelocity, double u, double sigma2_u) :
      _robotVelocity(robotVelocity), _u(u)
    {

        Eigen::Matrix<double,1,1> invR;
        invR(0,0) = (1.0/sigma2_u);
      // Fill in the inverse covariance. In this scalar case, this is just an inverse variance.
        setInvR( invR );

      // Tell the super class about the design variables:
        JacobianContainer::set_t dvs;
        robotVelocity.getDesignVariables(dvs);
        ErrorTermFs<1>::setDesignVariablesIterator(dvs.begin(), dvs.end());
    }

    ErrorTermMotionBST::~ErrorTermMotionBST()
    {

    }

        
    /// \brief evaluate the error term and return the weighted squared error e^T invR e
    double ErrorTermMotionBST::evaluateErrorImplementation()
    {
        error_t error;
        error(0) = _robotVelocity.evaluate()(0) - _u;
        //std::cout << "The motion error is: " << error(0) << std::endl;
        setError(error);
        return evaluateChiSquaredError();
    }


    /// \brief evaluate the jacobian
    void ErrorTermMotionBST::evaluateJacobiansImplementation()
    {
    	_robotVelocity.evaluateJacobians(ErrorTermFs<1>::_jacobians);
    }

  } // namespace backend
} // namespace aslam
