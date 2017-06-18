#ifndef ASLAM_BACKEND_OPTIMIZATION_PROBLEM_SIMPLE2
#define ASLAM_BACKEND_OPTIMIZATION_PROBLEM_SIMPLE2

#include "OptimizationProblemBase.hpp"
#include <boost/shared_ptr.hpp>
#include <vector>

#include <unordered_map>

namespace aslam {
  namespace backend {

    /**
     * \class SimpleOptimizationProblem
     *
     * \brief a simple implementation of the optimzation problem that
     *        only stores containers of design variables and error terms.
     *        This container owns the design variables and error terms and
     *        it will call delete on them when it goes out of scope.
     */
    class SimpleOptimizationProblem : public OptimizationProblemBase {
    public:
      SimpleOptimizationProblem();
      ~SimpleOptimizationProblem() override;

      /// \brief Add a design variable to the problem. If the second
      /// argument is true, the design variable will be deleted
      /// when the problem is cleared or goes out of scope.
      void addDesignVariable(DesignVariable* dv, bool problemOwnsVariable);

      /// \brief Add a design variable to the problem.
      void addDesignVariable(boost::shared_ptr<DesignVariable> dv);

      /// \brief Add an error term to the problem. If the second
      /// argument is true, the error term will be deleted when the
      /// problem is cleared or goes out of scope.
      void addErrorTerm(ErrorTerm* dv, bool problemOwnsVariable);
      void addErrorTerm(ScalarNonSquaredErrorTerm* dv, bool problemOwnsVariable);

      /// \brief Add an error term to the problem
      void addErrorTerm(const boost::shared_ptr<ErrorTerm> & et) override;
      virtual void addErrorTerm(const boost::shared_ptr<ScalarNonSquaredErrorTerm> & et);

      /// \brief clear the design variables and error terms.
      void clear();

      /// \brief used for debugging...is the design variable in the problem.
      bool isDesignVariableInProblem(const DesignVariable* dv);
    protected:
      size_t numDesignVariablesImplementation() const override;
      DesignVariable* designVariableImplementation(size_t i) override;
      const DesignVariable* designVariableImplementation(size_t i) const override;

      size_t numErrorTermsImplementation() const override;
      size_t numNonSquaredErrorTermsImplementation() const override;
      ErrorTerm* errorTermImplementation(size_t i) override;
      ScalarNonSquaredErrorTerm* nonSquaredErrorTermImplementation(size_t i) override;
      const ErrorTerm* errorTermImplementation(size_t i) const override;
      const ScalarNonSquaredErrorTerm* nonSquaredErrorTermImplementation(size_t i) const override;

      void getErrorsImplementation(const DesignVariable* dv, std::set<ErrorTerm*>& outErrorSet) override;
      void getNonSquaredErrorsImplementation(const DesignVariable* dv, std::set<ScalarNonSquaredErrorTerm*>& outErrorSet) override;

      // \todo Replace these std::vectors by something better. The underlying algorithms that this object
      //       supports suck with these containers. Blerg. See "removeDesignVariable()" for an example of
      //       just how bad this is.
      std::vector< boost::shared_ptr<DesignVariable> > _designVariables;
      std::vector< boost::shared_ptr<ErrorTerm> > _errorTerms;
      std::vector< boost::shared_ptr<ScalarNonSquaredErrorTerm> > _sNSErrorTerms;
    };


  } // namespace backend
} // namespace aslam


#endif /* ASLAM_BACKEND_OPTIMIZATION_PROBLEM_SIMPLE2 */
