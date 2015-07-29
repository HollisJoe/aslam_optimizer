#ifndef ASLAM_BACKEND_OPTIMIZER_RPROP_HPP
#define ASLAM_BACKEND_OPTIMIZER_RPROP_HPP


#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <sm/assert_macros.hpp>
#include <Eigen/Core>
#include "OptimizerRpropOptions.hpp"
#include "backend.hpp"
#include "OptimizationProblemBase.hpp"
#include <aslam/Exceptions.hpp>
#include <aslam/backend/Matrix.hpp>
#include <aslam/backend/LevenbergMarquardtTrustRegionPolicy.hpp>
#include <aslam/backend/DogLegTrustRegionPolicy.hpp>
#include <sm/timing/Timer.hpp>
#include <boost/thread.hpp>
#include <sparse_block_matrix/linear_solver.h>
#include "ScalarNonSquaredErrorTerm.hpp"

namespace sm {
  class PropertyTree;
}

namespace aslam {
  namespace backend {

    /**
     * \class OptimizerRprop
     *
     * RPROP implementation for the ASLAM framework.
     */
    class OptimizerRprop {
    public:
#ifdef aslam_backend_ENABLE_TIMING
      typedef sm::timing::Timer Timer;
#else
      typedef sm::timing::DummyTimer Timer;
#endif
      typedef boost::shared_ptr<OptimizerRprop> Ptr;
      typedef boost::shared_ptr<const OptimizerRprop> ConstPtr;
      typedef sparse_block_matrix::SparseBlockMatrix<Eigen::MatrixXd> SparseBlockMatrix;
      typedef Eigen::Matrix<double, Eigen::Dynamic, 1> ColumnVectorType;
      typedef Eigen::Matrix<double, 1, Eigen::Dynamic> RowVectorType;

      SM_DEFINE_EXCEPTION(Exception, aslam::Exception);

      /// \brief Constructor with default options
      OptimizerRprop();
      /// \brief Constructor with custom options
      OptimizerRprop(const OptimizerRpropOptions& options);
      /// \brief Constructor from property tree
      OptimizerRprop(const sm::PropertyTree& config);
      /// \brief Destructor
      ~OptimizerRprop();

      /// \brief Set up to work on the optimization problem.
      void setProblem(boost::shared_ptr<OptimizationProblemBase> problem);

      /// \brief initialize the optimizer to run on an optimization problem.
      void initialize();

      /// \brief Run the optimization
      void optimize();

      /// \brief Get the optimizer options.
      OptimizerRpropOptions& options();

      /// \brief compute the current gradient of the objective function
      void computeGradient(RowVectorType& outGrad, size_t nThreads, bool useMEstimator);

      /// \brief Return the current gradient norm
      inline double getGradientNorm() { return _curr_gradient_norm; }

      /// \brief Get the number of iterations the solver has run.
      ///        If it has never been started, the value will be zero.
      inline std::size_t getNumberOfIterations() const { return _nIterations; }

      /// \brief Get dense design variable i.
      DesignVariable* designVariable(size_t i);

      /// \brief how many dense design variables are involved in the problem
      size_t numDesignVariables() const;

      /// \brief print the internal timing information.
      void printTiming() const;

      /// \brief Do a bunch of checks to see if the problem is well-defined. This includes checking that every error term is
      ///        hooked up to design variables and running finite differences on error terms where this is possible.
      void checkProblemSetup();

    private:
      void evaluateGradients(size_t threadId, size_t startIdx, size_t endIdx, bool useMEstimator, RowVectorType& grad);
      void setupThreadedJob(boost::function<void(size_t, size_t, size_t, bool, RowVectorType&)> job, size_t nThreads, std::vector<RowVectorType>& out, bool useMEstimator);

    private:

      /// \brief Revert the last state update.
      void revertLastStateUpdate();

      /// \brief Apply a state update.
      double applyStateUpdate(const ColumnVectorType& dx);

      /// \brief The dense update vector.
      ColumnVectorType _dx;

      /// \brief current step-length to be performed into the negative direction of the gradient
      ColumnVectorType _delta;

      /// \brief gradient in the previous iteration
      RowVectorType _prev_gradient;

      /// \brief Current norm of the gradient
      double _curr_gradient_norm;

      /// \brief The current optimization problem.
      boost::shared_ptr<OptimizationProblemBase> _problem;

      /// \brief all design variables...first the non-marginalized ones (the dense ones), then the marginalized ones.
      std::vector<DesignVariable*> _designVariables;

      /// \brief all of the error terms involved in this problem
      std::vector<ErrorTerm*> _errorTermsS;
      std::vector<ScalarNonSquaredErrorTerm*> _errorTermsNS;

      /// \brief the current set of options
      OptimizerRpropOptions _options;

      /// \brief the total number of parameters of this problem, given by number of design variables and their dimensionality
      std::size_t _numOptParameters;

      /// \brief the total number of error terms as the sum of squared and non-squared error terms
      std::size_t _numErrorTerms;

      /// \brief Whether the optimizer is correctly initialized
      bool _isInitialized;

      /// \brief How many iterations the solver has run
      std::size_t _nIterations;

    };

  } // namespace backend
} // namespace aslam

#endif /* ASLAM_BACKEND_OPTIMIZER_RPROP_HPP */