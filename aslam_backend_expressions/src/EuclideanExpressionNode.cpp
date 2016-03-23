#include <aslam/backend/EuclideanExpressionNode.hpp>
#include <aslam/backend/VectorExpressionNode.hpp>
#include <sm/kinematics/rotations.hpp>
#include <sm/kinematics/homogeneous_coordinates.hpp>
#include <aslam/backend/HomogeneousExpressionNode.hpp>
namespace aslam {
  namespace backend {
      
  EuclideanExpressionNodeMultiply::EuclideanExpressionNodeMultiply(boost::shared_ptr<RotationExpressionNode> lhs, boost::shared_ptr<EuclideanExpressionNode> rhs) :
    _lhs(lhs), _rhs(rhs)
  {
    _C_lhs = _lhs->toRotationMatrix();
    _p_rhs = _rhs->evaluate();
  }

  EuclideanExpressionNodeMultiply::~EuclideanExpressionNodeMultiply()
  {
  }


    void EuclideanExpressionNodeMultiply::getDesignVariablesImplementation(DesignVariable::set_t & designVariables) const
    {
      _lhs->getDesignVariables(designVariables);
      _rhs->getDesignVariables(designVariables);
    }


    Eigen::Vector3d EuclideanExpressionNodeMultiply::evaluateImplementation() const
    {
      _C_lhs = _lhs->toRotationMatrix();
      _p_rhs = _rhs->evaluate();

      return _C_lhs * _p_rhs;
    }

    void EuclideanExpressionNodeMultiply::evaluateJacobiansImplementation(JacobianContainer & outJacobians) const
    {
      _lhs->evaluateJacobians(outJacobians, sm::kinematics::crossMx(_C_lhs * _p_rhs));
      _rhs->evaluateJacobians(outJacobians, _C_lhs);
    }

    void EuclideanExpressionNodeMultiply::evaluateJacobiansImplementation(JacobianContainer & outJacobians, const Eigen::MatrixXd & applyChainRule) const
    {
      _lhs->evaluateJacobians(outJacobians, applyChainRule * sm::kinematics::crossMx(_C_lhs * _p_rhs));
      _rhs->evaluateJacobians(outJacobians, applyChainRule * _C_lhs);
    }
    // -------------------------------------------------------
    // ## New Class for rotations with MatrixExpressions
    EuclideanExpressionNodeMatrixMultiply::EuclideanExpressionNodeMatrixMultiply(boost::shared_ptr<MatrixExpressionNode> lhs, boost::shared_ptr<EuclideanExpressionNode> rhs) :
         _lhs(lhs), _rhs(rhs)
    {
      _A_lhs = _lhs->toFullMatrix();
      _p_rhs = _rhs->evaluate();
    }

    EuclideanExpressionNodeMatrixMultiply::~EuclideanExpressionNodeMatrixMultiply()
    {
      
    }


    void EuclideanExpressionNodeMatrixMultiply::getDesignVariablesImplementation(DesignVariable::set_t & designVariables) const
    {
      _lhs->getDesignVariables(designVariables);
      _rhs->getDesignVariables(designVariables);
    }

    
    Eigen::Vector3d EuclideanExpressionNodeMatrixMultiply::evaluateImplementation() const
    {
      _A_lhs = _lhs->toFullMatrix();
      _p_rhs = _rhs->evaluate();

      return _A_lhs * _p_rhs;
    }

    void EuclideanExpressionNodeMatrixMultiply::evaluateJacobiansImplementation(JacobianContainer & outJacobians) const
    {
    	double p1 = _p_rhs(0), p2 = _p_rhs(1), p3 = _p_rhs(2);
    	Eigen::Matrix<double, 3,9> J_full;
    	J_full << 
            p1, 0,  0, p2,  0,  0, p3,  0,  0,
            0, p1,  0,  0, p2,  0,  0, p3,  0, 
            0,  0, p1,  0,  0, p2,  0,  0, p3;

        _lhs->evaluateJacobians(outJacobians, J_full); 				 // ## Set in the full 3x9 jacobian matrix
        _rhs->evaluateJacobians(outJacobians, _A_lhs);
        //_rhs->evaluateJacobians(outJacobians, Eigen::Matrix3d::Zero());
    }

    void EuclideanExpressionNodeMatrixMultiply::evaluateJacobiansImplementation(JacobianContainer & outJacobians, const Eigen::MatrixXd & applyChainRule) const
    {
    	double p1 = _p_rhs(0), p2 = _p_rhs(1), p3 = _p_rhs(2);
    	Eigen::Matrix<double, 3,9> J_full;
    	J_full << p1, 0, 0, p2, 0, 0, p3, 0, 0, 0, p1, 0, 0, p2, 0, 0, p3, 0, 0, 0, p1, 0, 0, p2, 0, 0, p3;

        _lhs->evaluateJacobians(outJacobians, applyChainRule * J_full);		        // ## Set in the full  3x9 jacobian matrix
        _rhs->evaluateJacobians(outJacobians, applyChainRule * _A_lhs);
        // _rhs->evaluateJacobians(outJacobians, applyChainRule * Eigen::Matrix3d::Identity());
    }

    // ----------------------------

    EuclideanExpressionNodeCrossEuclidean::EuclideanExpressionNodeCrossEuclidean(boost::shared_ptr<EuclideanExpressionNode> lhs, boost::shared_ptr<EuclideanExpressionNode> rhs) :
      _lhs(lhs), _rhs(rhs)
    {

    }

    EuclideanExpressionNodeCrossEuclidean::~EuclideanExpressionNodeCrossEuclidean()
    {

    }


    void EuclideanExpressionNodeCrossEuclidean::getDesignVariablesImplementation(DesignVariable::set_t & designVariables) const
    {
      _lhs->getDesignVariables(designVariables);
      _rhs->getDesignVariables(designVariables);
    }


    Eigen::Vector3d EuclideanExpressionNodeCrossEuclidean::evaluateImplementation() const
    {
      return sm::kinematics::crossMx(_lhs->evaluate()) * _rhs->evaluate();;
    }

    void EuclideanExpressionNodeCrossEuclidean::evaluateJacobiansImplementation(JacobianContainer & outJacobians) const
    {
      _lhs->evaluateJacobians(outJacobians, - sm::kinematics::crossMx(_rhs->evaluate()));
      _rhs->evaluateJacobians(outJacobians, sm::kinematics::crossMx(_lhs->evaluate()));
    }

    void EuclideanExpressionNodeCrossEuclidean::evaluateJacobiansImplementation(JacobianContainer & outJacobians, const Eigen::MatrixXd & applyChainRule) const
    {
      _lhs->evaluateJacobians(outJacobians, - applyChainRule * sm::kinematics::crossMx(_rhs->evaluate()));
      _rhs->evaluateJacobians(outJacobians, applyChainRule * sm::kinematics::crossMx(_lhs->evaluate()));
    }





    EuclideanExpressionNodeAddEuclidean::EuclideanExpressionNodeAddEuclidean(boost::shared_ptr<EuclideanExpressionNode> lhs, boost::shared_ptr<EuclideanExpressionNode> rhs) :
      _lhs(lhs), _rhs(rhs)
    {

    }

    EuclideanExpressionNodeAddEuclidean::~EuclideanExpressionNodeAddEuclidean()
    {

    }


    void EuclideanExpressionNodeAddEuclidean::getDesignVariablesImplementation(DesignVariable::set_t & designVariables) const
    {
      _lhs->getDesignVariables(designVariables);
      _rhs->getDesignVariables(designVariables);
    }


    Eigen::Vector3d EuclideanExpressionNodeAddEuclidean::evaluateImplementation() const
    {
      return _lhs->evaluate() + _rhs->evaluate();
    }

    void EuclideanExpressionNodeAddEuclidean::evaluateJacobiansImplementation(JacobianContainer & outJacobians) const
    {
      _lhs->evaluateJacobians(outJacobians);
      _rhs->evaluateJacobians(outJacobians);
    }

    void EuclideanExpressionNodeAddEuclidean::evaluateJacobiansImplementation(JacobianContainer & outJacobians, const Eigen::MatrixXd & applyChainRule) const
    {
      _lhs->evaluateJacobians(outJacobians, applyChainRule);
      _rhs->evaluateJacobians(outJacobians, applyChainRule);
    }





    EuclideanExpressionNodeSubtractEuclidean::EuclideanExpressionNodeSubtractEuclidean(boost::shared_ptr<EuclideanExpressionNode> lhs, boost::shared_ptr<EuclideanExpressionNode> rhs) :
      _lhs(lhs), _rhs(rhs)
    {

    }

    EuclideanExpressionNodeSubtractEuclidean::~EuclideanExpressionNodeSubtractEuclidean()
    {

    }


    void EuclideanExpressionNodeSubtractEuclidean::getDesignVariablesImplementation(DesignVariable::set_t & designVariables) const
    {
      _lhs->getDesignVariables(designVariables);
      _rhs->getDesignVariables(designVariables);
    }


    Eigen::Vector3d EuclideanExpressionNodeSubtractEuclidean::evaluateImplementation() const
    {
      return _lhs->evaluate() - _rhs->evaluate();
    }

    void EuclideanExpressionNodeSubtractEuclidean::evaluateJacobiansImplementation(JacobianContainer & outJacobians) const
    {
      _lhs->evaluateJacobians(outJacobians);
      _rhs->evaluateJacobians(outJacobians, -Eigen::Matrix3d::Identity());
    }

    void EuclideanExpressionNodeSubtractEuclidean::evaluateJacobiansImplementation(JacobianContainer & outJacobians, const Eigen::MatrixXd & applyChainRule) const
    {
      _lhs->evaluateJacobians(outJacobians, applyChainRule);
      _rhs->evaluateJacobians(outJacobians, - applyChainRule);
    }



    EuclideanExpressionNodeConstant::EuclideanExpressionNodeConstant(const Eigen::Vector3d & p) :
      _p(p)
    {
    }

    EuclideanExpressionNodeConstant::~EuclideanExpressionNodeConstant()
    {
    }

  void EuclideanExpressionNodeConstant::getDesignVariablesImplementation(DesignVariable::set_t & /* designVariables */) const
    {
    }

    Eigen::Vector3d EuclideanExpressionNodeConstant::evaluateImplementation() const
    {
      return _p;
    }

  void EuclideanExpressionNodeConstant::evaluateJacobiansImplementation(JacobianContainer & /* outJacobians */) const
    {
    }

  void EuclideanExpressionNodeConstant::evaluateJacobiansImplementation(JacobianContainer & /* outJacobians */, const Eigen::MatrixXd & /* applyChainRule */) const
    {
    }



    EuclideanExpressionNodeSubtractVector::EuclideanExpressionNodeSubtractVector(boost::shared_ptr<EuclideanExpressionNode> lhs, const Eigen::Vector3d & rhs) :
      _lhs(lhs), _rhs(rhs)
    {

    }

    EuclideanExpressionNodeSubtractVector::~EuclideanExpressionNodeSubtractVector()
    {

    }

    void EuclideanExpressionNodeSubtractVector::getDesignVariablesImplementation(DesignVariable::set_t & designVariables) const
    {
      _lhs->getDesignVariables(designVariables);
    }

    Eigen::Vector3d EuclideanExpressionNodeSubtractVector::evaluateImplementation() const
    {
      return _lhs->evaluate() - _rhs;
    }

    void EuclideanExpressionNodeSubtractVector::evaluateJacobiansImplementation(JacobianContainer & outJacobians) const
    {
      _lhs->evaluateJacobians(outJacobians);
    }

    void EuclideanExpressionNodeSubtractVector::evaluateJacobiansImplementation(JacobianContainer & outJacobians, const Eigen::MatrixXd & applyChainRule) const
    {
      _lhs->evaluateJacobians(outJacobians, applyChainRule);
    }



    EuclideanExpressionNodeNegated::EuclideanExpressionNodeNegated(boost::shared_ptr<EuclideanExpressionNode> operand) :
      _operand(operand)
    {

    }

    EuclideanExpressionNodeNegated::~EuclideanExpressionNodeNegated()
    {
    }

    void EuclideanExpressionNodeNegated::getDesignVariablesImplementation(DesignVariable::set_t & designVariables) const
    {
      _operand->getDesignVariables(designVariables);
    }

    Eigen::Vector3d EuclideanExpressionNodeNegated::evaluateImplementation() const
    {
      return - _operand->evaluate();
    }

    void EuclideanExpressionNodeNegated::evaluateJacobiansImplementation(JacobianContainer & outJacobians) const
    {
      _operand->evaluateJacobians(outJacobians, -Eigen::Matrix3d::Identity());
    }

    void EuclideanExpressionNodeNegated::evaluateJacobiansImplementation(JacobianContainer & outJacobians, const Eigen::MatrixXd & applyChainRule) const
    {
      _operand->evaluateJacobians(outJacobians, -applyChainRule);
    }


    EuclideanExpressionNodeScalarMultiply::EuclideanExpressionNodeScalarMultiply(boost::shared_ptr<EuclideanExpressionNode> p, boost::shared_ptr<ScalarExpressionNode> s) :
      _p(p),
      _s(s)
    {

    }

    EuclideanExpressionNodeScalarMultiply::~EuclideanExpressionNodeScalarMultiply()
    {
    }

    void EuclideanExpressionNodeScalarMultiply::getDesignVariablesImplementation(DesignVariable::set_t & designVariables) const
    {
      _p->getDesignVariables(designVariables);
      _s->getDesignVariables(designVariables);
    }

    Eigen::Vector3d EuclideanExpressionNodeScalarMultiply::evaluateImplementation() const
    {
      return _p->evaluate() * _s->toScalar();
    }

    void EuclideanExpressionNodeScalarMultiply::evaluateJacobiansImplementation(JacobianContainer & outJacobians) const
    {
      _p->evaluateJacobians(outJacobians, Eigen::Matrix3d::Identity() * _s->toScalar());
      _s->evaluateJacobians(outJacobians, _p->evaluate());
    }

    void EuclideanExpressionNodeScalarMultiply::evaluateJacobiansImplementation(JacobianContainer & outJacobians, const Eigen::MatrixXd & applyChainRule) const
    {
      _p->evaluateJacobians(outJacobians, applyChainRule * _s->toScalar());
      _s->evaluateJacobians(outJacobians, applyChainRule * _p->evaluate());
    }


    VectorExpression2EuclideanExpressionAdapter::VectorExpression2EuclideanExpressionAdapter(boost::shared_ptr<VectorExpressionNode<3> > vectorExpressionNode) :
      _vectorExpressionNode(vectorExpressionNode)
    {

    }

    VectorExpression2EuclideanExpressionAdapter::~VectorExpression2EuclideanExpressionAdapter()
    {
    }

    void VectorExpression2EuclideanExpressionAdapter::getDesignVariablesImplementation(DesignVariable::set_t & designVariables) const
    {
      _vectorExpressionNode->getDesignVariables(designVariables);
    }

    Eigen::Vector3d VectorExpression2EuclideanExpressionAdapter::evaluateImplementation() const
    {
      return _vectorExpressionNode->toVector();
    }

    void VectorExpression2EuclideanExpressionAdapter::evaluateJacobiansImplementation(JacobianContainer & outJacobians) const
    {
      _vectorExpressionNode->evaluateJacobians(outJacobians, Eigen::Matrix3d::Identity());
    }

    void VectorExpression2EuclideanExpressionAdapter::evaluateJacobiansImplementation(JacobianContainer & outJacobians, const Eigen::MatrixXd & applyChainRule) const
    {
      _vectorExpressionNode->evaluateJacobians(outJacobians, applyChainRule * Eigen::Matrix3d::Identity());
    }

  
  EuclideanExpressionNodeTranslation::EuclideanExpressionNodeTranslation(boost::shared_ptr<TransformationExpressionNode> operand) :
      _operand(operand) {
    
  }

  EuclideanExpressionNodeTranslation::~EuclideanExpressionNodeTranslation() {

  }


  Eigen::Vector3d EuclideanExpressionNodeTranslation::evaluateImplementation() const {
    return _operand->toTransformationMatrix().topRightCorner<3,1>();
  }

  void EuclideanExpressionNodeTranslation::evaluateJacobiansImplementation(JacobianContainer & outJacobians) const {
    Eigen::MatrixXd J = Eigen::MatrixXd::Identity(3,6);
    Eigen::Vector3d p = _operand->toTransformationMatrix().topRightCorner<3,1>();
    J.topRightCorner<3,3>() = sm::kinematics::crossMx(p);
    _operand->evaluateJacobians(outJacobians, J);
  }

  void EuclideanExpressionNodeTranslation::evaluateJacobiansImplementation(JacobianContainer & outJacobians, const Eigen::MatrixXd & applyChainRule) const {
    Eigen::MatrixXd J = Eigen::MatrixXd::Identity(3,6);
    Eigen::Vector3d p = _operand->toTransformationMatrix().topRightCorner<3,1>();
    J.topRightCorner<3,3>() = sm::kinematics::crossMx(p);
    _operand->evaluateJacobians(outJacobians, applyChainRule*J);
  }

  void EuclideanExpressionNodeTranslation::getDesignVariablesImplementation(DesignVariable::set_t & designVariables) const {
    return _operand->getDesignVariables(designVariables); 
  }



  EuclideanExpressionNodeRotationParameters::EuclideanExpressionNodeRotationParameters(boost::shared_ptr<RotationExpressionNode> operand, sm::kinematics::RotationalKinematics::Ptr rk) :
      _operand(operand), _rk(rk) {

  }

  EuclideanExpressionNodeRotationParameters::~EuclideanExpressionNodeRotationParameters() {

  }


  Eigen::Vector3d EuclideanExpressionNodeRotationParameters::evaluateImplementation() const {
    return _rk->rotationMatrixToParameters(_operand->toRotationMatrix());
  }

  void EuclideanExpressionNodeRotationParameters::evaluateJacobiansImplementation(JacobianContainer & outJacobians) const {
    Eigen::MatrixXd J = _rk->parametersToSMatrix(
        _rk->rotationMatrixToParameters(_operand->toRotationMatrix())).inverse();
    _operand->evaluateJacobians(outJacobians,J);

  }

  void EuclideanExpressionNodeRotationParameters::evaluateJacobiansImplementation(JacobianContainer & outJacobians, const Eigen::MatrixXd & applyChainRule) const {
    Eigen::MatrixXd J = _rk->parametersToSMatrix(
        _rk->rotationMatrixToParameters(_operand->toRotationMatrix())).inverse();
    _operand->evaluateJacobians(outJacobians, applyChainRule*J);

  }

  void EuclideanExpressionNodeRotationParameters::getDesignVariablesImplementation(DesignVariable::set_t & designVariables) const {
    return _operand->getDesignVariables(designVariables);
  }


  EuclideanExpressionNodeFromHomogeneous::EuclideanExpressionNodeFromHomogeneous(boost::shared_ptr<HomogeneousExpressionNode> root) : _root(root) {

  }
  EuclideanExpressionNodeFromHomogeneous:: ~EuclideanExpressionNodeFromHomogeneous() {

  }


  Eigen::Vector3d EuclideanExpressionNodeFromHomogeneous::evaluateImplementation() const {
    return sm::kinematics::fromHomogeneous( _root->toHomogeneous() );
  }

  void EuclideanExpressionNodeFromHomogeneous::evaluateJacobiansImplementation(JacobianContainer & outJacobians) const {
    //Eigen::Vector3d fromHomogeneous(const Eigen::Vector4d & v, Eigen::Matrix<double,3,4> * jacobian = NULL);
    Eigen::Matrix<double,3,4> Jh;
    sm::kinematics::fromHomogeneous( _root->toHomogeneous(), &Jh );
    _root->evaluateJacobians( outJacobians, Jh );
  }

  void EuclideanExpressionNodeFromHomogeneous::evaluateJacobiansImplementation(JacobianContainer & outJacobians, const Eigen::MatrixXd & applyChainRule) const {
    Eigen::Matrix<double,3,4> Jh;
    sm::kinematics::fromHomogeneous( _root->toHomogeneous(), &Jh );
    _root->evaluateJacobians( outJacobians, applyChainRule * Jh );

  }

  void EuclideanExpressionNodeFromHomogeneous::getDesignVariablesImplementation(DesignVariable::set_t & designVariables) const {
    _root->getDesignVariables(designVariables);
  }

  

  
  } // namespace backend
} // namespace aslam
