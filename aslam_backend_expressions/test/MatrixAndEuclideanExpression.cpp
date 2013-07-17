#include <sm/eigen/gtest.hpp>
#include <sm/eigen/NumericalDiff.hpp>
#include <aslam/backend/EuclideanExpression.hpp>
#include <sm/kinematics/rotations.hpp>
#include <sm/kinematics/quaternion_algebra.hpp>
#include <aslam/backend/test/ExpressionTests.hpp>
#include <aslam/backend/RotationQuaternion.hpp>
#include <aslam/backend/EuclideanPoint.hpp>
#include <aslam/backend/RotationExpression.hpp>
#include <aslam/backend/EuclideanDirection.hpp>
#include <aslam/backend/MatrixExpression.hpp>
#include <aslam/backend/MatrixTransformation.hpp>
#include <aslam/backend/DesignVariableVector.hpp>

using namespace aslam::backend;
using namespace sm::kinematics;

// Test that the jacobian matches the finite difference jacobian
TEST(EuclideanExpressionNodeTestSuites, testPoint)
{
  try 
    {
      using namespace sm::kinematics;
      EuclideanPoint point(Eigen::Vector3d::Random());
      point.setActive(true);
      point.setBlockIndex(0);
      EuclideanExpression qr(&point);
      
      SCOPED_TRACE("");
      SCOPED_TRACE("");
      testJacobian(qr);
    }
  catch(std::exception const & e)
    {
      FAIL() << e.what();
    }
}


// Test that the jacobian matches the finite difference jacobian
TEST(EuclideanExpressionNodeTestSuites, testRotatedPoint)
{
  try 
    {
      using namespace sm::kinematics;
      RotationQuaternion quat(quatRandom());
      quat.setActive(true);
      quat.setBlockIndex(0);
      RotationExpression C(&quat);


      EuclideanPoint point(Eigen::Vector3d::Random());
      point.setActive(true);
      point.setBlockIndex(1);
      EuclideanExpression p(&point);
      
      EuclideanExpression Cp = C * p;
      
      SCOPED_TRACE("");
      testJacobian(Cp);

      sm::eigen::assertNear(Cp.toEuclidean(), C.toRotationMatrix() * p.toEuclidean(), 1e-14, SM_SOURCE_FILE_POS, "Testing the result is unchanged");
    }
  catch(std::exception const & e)
    {
      FAIL() << e.what();
    }
}



// Test that the jacobian matches the finite difference jacobian
TEST(EuclideanExpressionNodeTestSuites, testRotatedInverse)
{
  try 
    {
      using namespace sm::kinematics;
      RotationQuaternion quat(quatRandom());
      quat.setActive(true);
      quat.setBlockIndex(0);
      RotationExpression C(&quat);


      EuclideanPoint point(Eigen::Vector3d::Random());
      point.setActive(true);
      point.setBlockIndex(1);
      EuclideanExpression p(&point);
      
      EuclideanExpression Cp = C.inverse() * p;
      
      SCOPED_TRACE("");
      testJacobian(Cp);

      sm::eigen::assertNear(Cp.toEuclidean(), C.toRotationMatrix().transpose() * p.toEuclidean(), 1e-14, SM_SOURCE_FILE_POS, "Testing the result is unchanged");
    }
  catch(std::exception const & e)
    {
      FAIL() << e.what();
    }
} 


// Test that the jacobian matches the finite difference jacobian
TEST(EuclideanExpressionNodeTestSuites, testRotationExpression1)
{
  try 
    {
      using namespace sm::kinematics;
      RotationQuaternion quat0(quatRandom());
      quat0.setActive(true);
      quat0.setBlockIndex(0);
      RotationExpression C0(&quat0);

      using namespace sm::kinematics;
      RotationQuaternion quat1(quatRandom());
      quat1.setActive(true);
      quat1.setBlockIndex(1);
      RotationExpression C1(&quat1);


      EuclideanPoint point(Eigen::Vector3d::Random());
      point.setActive(true);
      point.setBlockIndex(2);
      EuclideanExpression p(&point);
      
      RotationExpression C01 = C0 * C1;
      EuclideanExpression C01p = C01 * p;
      
      SCOPED_TRACE("");
      testJacobian(C01p);

      sm::eigen::assertNear(C01p.toEuclidean(), C0.toRotationMatrix() * C1.toRotationMatrix() * p.toEuclidean(), 1e-14, SM_SOURCE_FILE_POS, "Testing the result is unchanged");
    }
  catch(std::exception const & e)
    {
      FAIL() << e.what();
    }
} 

// Test that the jacobian matches the finite difference jacobian
TEST(EuclideanExpressionNodeTestSuites, testRotationExpression2)
{
  try 
    {
      using namespace sm::kinematics;
      RotationQuaternion quat0(quatRandom());
      quat0.setActive(true);
      quat0.setBlockIndex(0);
      RotationExpression C0(&quat0);

      using namespace sm::kinematics;
      RotationQuaternion quat1(quatRandom());
      quat1.setActive(true);
      quat1.setBlockIndex(1);
      RotationExpression C1(&quat1);


      EuclideanPoint point(Eigen::Vector3d::Random());
      point.setActive(true);
      point.setBlockIndex(2);
      EuclideanExpression p(&point);
      
      EuclideanExpression C1p = C1 * p;
      EuclideanExpression C01p = C0 * C1p;
      
      SCOPED_TRACE("");
      testJacobian(C01p);

      sm::eigen::assertNear(C01p.toEuclidean(), C0.toRotationMatrix() * C1.toRotationMatrix() * p.toEuclidean(), 1e-14, SM_SOURCE_FILE_POS, "Testing the result is unchanged");
    }
  catch(std::exception const & e)
    {
      FAIL() << e.what();
    }
} 

// Test that the jacobian matches the finite difference jacobian
TEST(EuclideanExpressionNodeTestSuites, testEuclideanCrossproduct)
{
  try
    {
      using namespace sm::kinematics;
      EuclideanPoint point1(Eigen::Vector3d::Random());
      point1.setActive(true);
      point1.setBlockIndex(1);
      EuclideanExpression p1(&point1);

      EuclideanPoint point2(Eigen::Vector3d::Random());
      point2.setActive(true);
      point2.setBlockIndex(2);
      EuclideanExpression p2(&point2);

      EuclideanExpression p_cross = p1.cross(p2);

      SCOPED_TRACE("");
      testJacobian(p_cross);

    }
  catch(std::exception const & e)
    {
      FAIL() << e.what();
    }
}

// Test that the jacobian matches the finite difference jacobian
TEST(EuclideanExpressionNodeTestSuites, testEuclideanAddition)
{
  try
    {
      using namespace sm::kinematics;
      EuclideanPoint point1(Eigen::Vector3d::Random());
      point1.setActive(true);
      point1.setBlockIndex(1);
      EuclideanExpression p1(&point1);

      EuclideanPoint point2(Eigen::Vector3d::Random());
      point2.setActive(true);
      point2.setBlockIndex(2);
      EuclideanExpression p2(&point2);

      EuclideanExpression p_add = p1 + p2;

      SCOPED_TRACE("");
      testJacobian(p_add);

    }
  catch(std::exception const & e)
    {
      FAIL() << e.what();
    }
}

// Test that the jacobian matches the finite difference jacobian
TEST(EuclideanExpressionNodeTestSuites, testEuclideanSubtraction)
{
  try
    {
      using namespace sm::kinematics;
      EuclideanPoint point1(Eigen::Vector3d::Random());
      point1.setActive(true);
      point1.setBlockIndex(1);
      EuclideanExpression p1(&point1);

      EuclideanPoint point2(Eigen::Vector3d::Random());
      point2.setActive(true);
      point2.setBlockIndex(2);
      EuclideanExpression p2(&point2);

      EuclideanExpression p_diff = p1 - p2;

      SCOPED_TRACE("");
      testJacobian(p_diff);

    }
  catch(std::exception const & e)
    {
      FAIL() << e.what();
    }
}

// Test that the jacobian matches the finite difference jacobian
TEST(EuclideanExpressionNodeTestSuites, testVectorSubtraction)
{
  try
    {
      using namespace sm::kinematics;
      EuclideanPoint point1(Eigen::Vector3d::Random());
      point1.setActive(true);
      point1.setBlockIndex(1);
      EuclideanExpression p1(&point1);

      Eigen::Vector3d p2(Eigen::Vector3d::Random());

      EuclideanExpression p_diff = p1 - p2;

      SCOPED_TRACE("");
      testJacobian(p_diff);

    }
  catch(std::exception const & e)
    {
      FAIL() << e.what();
    }
}

// Test that the jacobian matches the finite difference jacobian
TEST(EuclideanExpressionNodeTestSuites, testNegation)
{
  try
    {
      using namespace sm::kinematics;
      EuclideanPoint point1(Eigen::Vector3d::Random());
      point1.setActive(true);
      point1.setBlockIndex(1);
      EuclideanExpression p1(&point1);

      EuclideanExpression pNegated = -p1;

      sm::eigen::assertNear(-point1.toEuclidean(), pNegated.toEuclidean(), 1e-14, SM_SOURCE_FILE_POS, "Testing the result is unchanged");

      SCOPED_TRACE("");
      testJacobian(pNegated);

    }
  catch(std::exception const & e)
    {
      FAIL() << e.what();
    }
}

// Test that the jacobian matches the finite difference jacobian
TEST(EuclideanExpressionNodeTestSuites, testEuclideanDrection)
{
  try
    {
      using namespace sm::kinematics;
      Eigen::Vector3d p = Eigen::Vector3d::Random() * 10;
      EuclideanDirection point1(p);
      ASSERT_DOUBLE_MX_EQ(p, point1.toEuclidean(),1e-2, "Checking if the euclidean point is recovered correctly")

      point1.setActive(true);
      point1.setBlockIndex(1);
      EuclideanExpression p1 = point1.toExpression();

      //Eigen::Vector3d p2(Eigen::Vector3d::Random());
      
      //EuclideanExpression p_diff = p1 - p2;

      SCOPED_TRACE("");
      testJacobian(p1);

    }
  catch(std::exception const & e)
    {
      FAIL() << e.what();
    }
}

// Test that the jacobian matches the finite difference jacobian
TEST(EuclideanExpressionNodeTestSuites, testAdaptedVectorExpression)
{
  try
    {
      using namespace sm::kinematics;
      EuclideanPoint point1(Eigen::Vector3d::Random());
      point1.setActive(true);
      point1.setBlockIndex(1);
      EuclideanExpression p1(&point1);

      DesignVariableVector<3> point2(Eigen::Vector3d::Random());
      point2.setActive(true);
      point2.setBlockIndex(2);
      EuclideanExpression p2(&point2);

      EuclideanExpression p_add = p1 + p2;

      SCOPED_TRACE("");
      testJacobian(p1);
    }
  catch(std::exception const & e)
    {
      FAIL() << e.what();
    }
}

// Test that the jacobian matches the finite difference jacobian
TEST(EuclideanExpressionNodeTestSuites, testMatrixTransformedPoint)
{
  try
    {
      using namespace sm::kinematics;
      MatrixTransformation a(Eigen::Matrix3d::Random());
      a.setActive(true);
      a.setBlockIndex(0);
      MatrixExpression A(&a);


      EuclideanPoint point(Eigen::Vector3d::Random());
      point.setActive(true);
      point.setBlockIndex(1);
      EuclideanExpression p(&point);

      EuclideanExpression Ap = A * p;

      SCOPED_TRACE("");
      testJacobian(Ap);

      sm::eigen::assertNear(Ap.toEuclidean(), A.toFullMatrix() * p.toEuclidean(), 1e-14, SM_SOURCE_FILE_POS, "Testing the result is unchanged");
    }
  catch(std::exception const & e)
    {
      FAIL() << e.what();
    }
}

// Test that the jacobian matches the finite difference jacobian
TEST(EuclideanExpressionNodeTestSuites, testMatrixdoubleTransformedPoint)
{
  try
    {
      using namespace sm::kinematics;
      MatrixTransformation a(Eigen::Matrix3d::Random());
      a.setActive(true);
      a.setBlockIndex(0);
      MatrixExpression A(&a);

      MatrixTransformation b(Eigen::Matrix3d::Random());
      b.setActive(true);
      b.setBlockIndex(1);
      MatrixExpression B(&b);


      EuclideanPoint point(Eigen::Vector3d::Random());
      point.setActive(true);
      point.setBlockIndex(2);
      EuclideanExpression p(&point);

      EuclideanExpression Ap = B*(A * p);

      SCOPED_TRACE("");
      testJacobian(Ap);

      sm::eigen::assertNear(Ap.toEuclidean(), B.toFullMatrix()* (A.toFullMatrix() * p.toEuclidean()), 1e-14, SM_SOURCE_FILE_POS, "Testing the result is unchanged");
    }
  catch(std::exception const & e)
    {
      FAIL() << e.what();
    }
}

// Test that the jacobian matches the finite difference jacobian
TEST(EuclideanExpressionNodeTestSuites, testDiagonalMatrixTransformedPoint)
{
  try
    {
      using namespace sm::kinematics;
      Eigen::Matrix3d S = Eigen::Matrix3d::Zero();
      S(0,0)= 10*(float)drand48();
      S(1,1)= 10*(float)drand48();
      S(2,2)= 10*(float)drand48();
      MatrixTransformation a(S,Eigen::Matrix3d::Identity());
      a.setActive(true);
      a.setBlockIndex(0);
      MatrixExpression A(&a);


      EuclideanPoint point(Eigen::Vector3d::Random());
      point.setActive(true);
      point.setBlockIndex(1);
      EuclideanExpression p(&point);

      EuclideanExpression Ap = A * p;

      SCOPED_TRACE("");
      testJacobian(Ap);

      sm::eigen::assertNear(Ap.toEuclidean(), A.toFullMatrix() * p.toEuclidean(), 1e-14, SM_SOURCE_FILE_POS, "Testing the result is unchanged");
    }
  catch(std::exception const & e)
    {
      FAIL() << e.what();
    }
}

// Test that the jacobian matches the finite difference jacobian
TEST(EuclideanExpressionNodeTestSuites, testLowerTriangleMatrixTransformedPoint)
{
  try
    {
      using namespace sm::kinematics;
      Eigen::Matrix3d M = Eigen::Matrix3d::Identity();
      Eigen::Matrix3d M_pattern = Eigen::Matrix3d::Zero();
      M(1,0)= 10*(float)drand48();
      M(2,0)= 10*(float)drand48();
      M(2,1)= 10*(float)drand48();
      // set the entries, that will be estimated by the calibration to 1
      M_pattern(1,0)= 1;
      M_pattern(2,0)= 1;
      M_pattern(2,1)= 1;
      MatrixTransformation a(M,M_pattern);
      a.setActive(true);
      a.setBlockIndex(0);
      MatrixExpression A(&a);


      EuclideanPoint point(Eigen::Vector3d::Random());
      point.setActive(true);
      point.setBlockIndex(1);
      EuclideanExpression p(&point);

      EuclideanExpression Ap = A * p;

      SCOPED_TRACE("");
      testJacobian(Ap);

      sm::eigen::assertNear(Ap.toEuclidean(), A.toFullMatrix() * p.toEuclidean(), 1e-14, SM_SOURCE_FILE_POS, "Testing the result is unchanged");
    }
  catch(std::exception const & e)
    {
      FAIL() << e.what();
    }
}

// Test that the jacobian matches the finite difference jacobian
TEST(EuclideanExpressionNodeTestSuites, testLowerMixedMatrixTransformedPoint)
{
  try
    {
      using namespace sm::kinematics;
      Eigen::Matrix3d M = Eigen::Matrix3d::Identity();
      Eigen::Matrix3d M_pattern = Eigen::Matrix3d::Zero();
      M(1,0)= 10*(float)drand48();
      M(2,0)= 10*(float)drand48();
      M(2,1)= 10*(float)drand48();
      // set the entries, that will be estimated by the calibration to 1
      M_pattern(1,0)= 1;
      M_pattern(2,0)= 1;
      M_pattern(2,1)= 1;
      MatrixTransformation a(M,M_pattern);
      a.setActive(true);
      a.setBlockIndex(0);
      MatrixExpression A(&a);
      sm::eigen::assertNear(M, A.toFullMatrix(), 1e-14, SM_SOURCE_FILE_POS, "Testing the initial matrix is good");

      Eigen::Matrix3d S = Eigen::Matrix3d::Zero();
      S(0,0)= 10*(float)drand48();
      S(1,1)= 10*(float)drand48();
      S(2,2)= 10*(float)drand48();
      MatrixTransformation b(S,Eigen::Matrix3d::Identity());
      b.setActive(true);
      b.setBlockIndex(1);
      MatrixExpression B(&b);

      sm::eigen::assertNear(S, B.toFullMatrix(), 1e-14, SM_SOURCE_FILE_POS, "Testing the result is unchanged");


      EuclideanPoint point(Eigen::Vector3d::Random());
      point.setActive(true);
      point.setBlockIndex(2);
      EuclideanExpression p(&point);

      EuclideanExpression Ap = B * (A * p);

      SCOPED_TRACE("");
      testJacobian(Ap);

      sm::eigen::assertNear(Ap.toEuclidean(), B.toFullMatrix() * A.toFullMatrix() * p.toEuclidean(), 1e-13, SM_SOURCE_FILE_POS, "Testing the result is unchanged");
    }
  catch(std::exception const & e)
    {
      FAIL() << e.what();
    }
}

