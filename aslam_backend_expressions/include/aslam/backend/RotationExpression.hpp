#ifndef ASLAM_BACKEND_ROTATION_EXPRESSION_HPP
#define ASLAM_BACKEND_ROTATION_EXPRESSION_HPP

#include <Eigen/Core>
#include <boost/shared_ptr.hpp>
#include <aslam/backend/JacobianContainer.hpp>
#include "HomogeneousExpression.hpp"
#include "EuclideanExpression.hpp"
#include "TransformationExpression.hpp"
#include <set>
#include <sm/kinematics/RotationalKinematics.hpp>

namespace aslam {
  namespace backend {
    
    class RotationExpressionNode;
    class TransformationExpression;
    class EuclideanExpression;
    class HomogeneousExpression;

    class RotationExpression
    {
    public:
        SM_DEFINE_EXCEPTION(Exception, std::runtime_error);

      /// \breif initialize from an existing node.
      RotationExpression(boost::shared_ptr<RotationExpressionNode> root);

      /// \brief Initialize from an existing node. The node will not be deleted.
      RotationExpression(RotationExpressionNode * root);
      
      /// \breif initialize from an constant rotation matrix.
      RotationExpression(const Eigen::Matrix3d & C);

      virtual ~RotationExpression();

      /// \brief Evaluate the rotation matrix.
      Eigen::Matrix3d toRotationMatrix();

      EuclideanExpression toParameters(sm::kinematics::RotationalKinematics::Ptr rk);
      
      /// \brief return the expression that inverts the rotation.
      RotationExpression inverse();
      
      /// \brief Evaluate the Jacobians in the form (1 - (S \delta v)^\times) \bar C
      void evaluateJacobians(JacobianContainer & outJacobians) const;

      RotationExpression operator*(const RotationExpression & p);
      EuclideanExpression operator*(const EuclideanExpression & p);
      HomogeneousExpression operator*(const HomogeneousExpression & p);

      TransformationExpression toTransformationExpression();

      void getDesignVariables(DesignVariable::set_t & designVariables) const;

      boost::shared_ptr<RotationExpressionNode> root() { return _root; }
    private:

      RotationExpression();
      boost::shared_ptr<RotationExpressionNode> _root;
    };

  } // namespace backend
} // namespace aslam


#endif /* ASLAM_BACKEND_ROTATION_EXPRESSION_HPP */
