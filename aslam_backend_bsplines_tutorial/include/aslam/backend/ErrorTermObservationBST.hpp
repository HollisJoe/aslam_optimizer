#ifndef ASLAM_BACKEND_TUTORIAL_OBSERVATION_HPP
#define ASLAM_BACKEND_TUTORIAL_OBSERVATION_HPP

#include <aslam/backend/ErrorTerm.hpp>
#include <aslam/backend/EuclideanExpression.hpp>
#include <aslam/backend/VectorExpression.hpp>

namespace aslam {
  namespace backend {
    
    // An error term implementing our observation model.
    // This class derives from ErrorTermFs<1> because the
    // errors are of dimension 1.
    class ErrorTermObservationBST : public ErrorTermFs<1>
    {
    public:
      // This is important. The superclass holds some fixed-sized Eigen types
      // For more information, see:
      // http://eigen.tuxfamily.org/dox-devel/TopicStructHavingEigenMembers.html
      EIGEN_MAKE_ALIGNED_OPERATOR_NEW

      ErrorTermObservationBST(aslam::backend::VectorExpression<1> robotPos, aslam::backend::EuclideanExpression wallPoint, double y, double sigma2_n);
      virtual ~ErrorTermObservationBST();

    protected:
      /// This is the inteface required by ErrorTermFs<>
      
      /// \brief evaluate the error term and return the weighted squared error e^T invR e
      virtual double evaluateErrorImplementation();

      /// \brief evaluate the jacobian
      virtual void evaluateJacobiansImplementation();

    private:
      aslam::backend::VectorExpression<1> _robotPos;
      aslam::backend::EuclideanExpression _wallPoint;

      double _y;
      
    };

  } // namespace backend
} // namespace aslam


#endif /* ASLAM_BACKEND_TUTORIAL_OBSERVATION_HPP */
