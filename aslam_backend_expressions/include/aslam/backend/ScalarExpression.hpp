#ifndef ASLAM_SCALAR_EXPRESSION_HPP
#define ASLAM_SCALAR_EXPRESSION_HPP

#include <Eigen/Core>
#include <boost/shared_ptr.hpp>
#include <aslam/backend/JacobianContainer.hpp>
#include <set>


namespace aslam {
  namespace backend {
    class HomogeneousExpression;
    class ScalarExpressionNode;
    
    class ScalarExpression
    {
    public:
      ScalarExpression( double value );
      ScalarExpression(ScalarExpressionNode * designVariable);
      ScalarExpression(boost::shared_ptr<ScalarExpressionNode> designVariable);
      virtual ~ScalarExpression();
      
      double toScalar() const;
      double toValue() const { return toScalar(); }

      void evaluateJacobians(JacobianContainer & outJacobians) const;
      void evaluateJacobians(JacobianContainer & outJacobians, const Eigen::MatrixXd & applyChainRule) const;
      void getDesignVariables(DesignVariable::set_t & designVariables) const;

      boost::shared_ptr<ScalarExpressionNode> root() { return _root; }

      ScalarExpression operator+(const ScalarExpression & s);
      ScalarExpression operator-(const ScalarExpression & s);
      ScalarExpression operator*(const ScalarExpression & s);
      ScalarExpression operator+(double s);
      ScalarExpression operator-(double s);
      ScalarExpression operator*(double s);


    private:

      /// \todo make the default constructor private.
      ScalarExpression();

      friend class RotationExpression;
      
      boost::shared_ptr<ScalarExpressionNode> _root;
    };
    
  } // namespace backend
} // namespace aslam


#endif /* ASLAM_SCALAR_EXPRESSION_HPP */
