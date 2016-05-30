/*
 * RotationQuaternionAsVectorExpressionNode.hpp
 *
 *  Created on: Jul 17, 2013
 *      Author: hannes
 */

#ifndef ROTATIONQUATERNIONASVECTOREXPRESSIONNODE_HPP_
#define ROTATIONQUATERNIONASVECTOREXPRESSIONNODE_HPP_

#include "../RotationQuaternion.hpp"
#include "../VectorExpressionNode.hpp"

namespace aslam {
namespace backend {


template<bool AssumeLieAlgebraVectorInputInJacobian = true>
struct RotationQuaternionAsVectorExpressionNode : public VectorExpressionNode<4> {
  RotationQuaternion & quat;
  RotationQuaternionAsVectorExpressionNode(RotationQuaternion & quat) : quat(quat){
  }
  virtual vector_t evaluateImplementation() const{ Eigen::MatrixXd ret; quat.getParameters(ret); return ret; };
  virtual void evaluateJacobiansImplementation(JacobianContainer & outJacobians) const override {
    if(AssumeLieAlgebraVectorInputInJacobian){
      quat.evaluateJacobians(outJacobians, sm::kinematics::quatOPlus(quat.getQuaternion() * 0.5).topLeftCorner<4,3>());
    }else{
      quat.evaluateJacobians(outJacobians);
    }
  };
  virtual void evaluateJacobiansImplementationWithDifferential(JacobianContainer & /* outJacobians */, const differential_t & /* chainRuleDifferentail */) const { throw std::runtime_error("should not happen");};
  virtual void getDesignVariablesImplementation(DesignVariable::set_t & designVariables) const { designVariables.insert(&quat);};
};

}
}
#endif /* ROTATIONQUATERNIONASVECTOREXPRESSIONNODE_HPP_ */
