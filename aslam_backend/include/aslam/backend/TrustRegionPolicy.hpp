#ifndef ASLAM_BACKEND_TRUST_REGION_POLICY_HPP
#define ASLAM_BACKEND_TRUST_REGION_POLICY_HPP

#include <aslam/backend/LinearSystemSolver.hpp>
#include <boost/shared_ptr.hpp>
#include "Optimizer2Options.hpp"
#include <sm/eigen/assert_macros.hpp>
#include <aslam/Exceptions.hpp>

namespace aslam {
    namespace backend {
        
        class TrustRegionPolicy
        {
        public:
            TrustRegionPolicy();
            virtual ~TrustRegionPolicy();
            
            /// \brief called by the optimizer when an optimization is starting
            virtual void optimizationStarting(double J);
            
            /// \brief Returns true if the solution was successful
            virtual bool solveSystem(double J, bool previousIterationFailed, int nThreads, Eigen::VectorXd& outDx);

            /// \brief get the linear system solver
            boost::shared_ptr<LinearSystemSolver> getSolver();

            /// \brief set the linear system solver
            virtual void setSolver(boost::shared_ptr<LinearSystemSolver> solver);
            
            /// \brief should the optimizer revert on failure? You should probably return true (the default implementation does this)
            virtual bool revertOnFailure();

            /// \brief print the current state to a stream (no newlines).
            virtual std::ostream & printState(std::ostream & out) const = 0;
            virtual std::string name() const = 0;
            virtual bool requiresAugmentedDiagonal() const = 0;
        protected:
            double get_dJ();
            bool isFirstIteration(){ return _isFirstIteration; }

            /// \brief called by the optimizer when an optimization is starting
            virtual void optimizationStartingImplementation(double J) = 0;
            
            /// \brief Returns true if the solution was successful
            virtual bool solveSystemImplementation(double J, bool previousIterationFailed, int nThreads, Eigen::VectorXd& outDx) = 0;

            boost::shared_ptr<LinearSystemSolver> _solver;
            
        private:
            /// \brief the linear system solver.
            double _J;
            double _p_J;
            bool _isFirstIteration;
        };

    } // namespace backend
} // namespace aslam


#endif /* ASLAM_BACKEND_TRUST_REGION_POLICY_HPP */
