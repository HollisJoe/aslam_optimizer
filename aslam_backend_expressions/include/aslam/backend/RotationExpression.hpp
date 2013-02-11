#ifndef ASLAM_BACKEND_ROTATION_EXPRESSION_HPP
#define ASLAM_BACKEND_ROTATION_EXPRESSION_HPP

#include <Eigen/Core>
#include <boost/shared_ptr.hpp>
#include <aslam/backend/JacobianContainer.hpp>
#include "HomogeneousExpression.hpp"
#include "EuclideanExpression.hpp"
#include "TransformationExpression.hpp"
#include <set>

namespace aslam {
  namespace backend {
    
    class RotationExpressionNode;
    class TransformationExpression;
    class EuclideanExpression;
    class HomogeneousExpression;

    class RotationExpression
    {
    public:
      /// \breif initialize from an existing node.
      RotationExpression(boost::shared_ptr<RotationExpressionNode> root);

      /// \brief Initialize from an existing node. The node will not be deleted.
      RotationExpression(RotationExpressionNode * root);
      
      virtual ~RotationExpression();

      /// \brief Evaluate the rotation matrix.
      Eigen::Matrix3d toRotationMatrix();

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
