//
// Created by TJ George on 11/14/24.
//

// Checker that detects pthread_create instances
#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/StaticAnalyzer/Frontend/CheckerRegistry.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CallEvent.h"

#include "clang/AST/Expr.h"
#include "clang/AST/Decl.h"
#include "clang/Basic/SourceManager.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;
using namespace ento;

#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CallDescription.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CallEvent.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"

using namespace clang;
using namespace ento;

class PthreadCreateChecker : public Checker<check::PostCall> {
  const CallDescription CreateFN{ CDM::CLibrary, {"pthread_create"}, 4 };
public:
  void checkPostCall(const CallEvent &Call, CheckerContext &C) const;
};

void PthreadCreateChecker::checkPostCall(const CallEvent &Call,
                                         CheckerContext &C) const {
  if (!CreateFN.matches(Call))
    return;

  // Get the symbolic value corresponding to the file handle.
  SymbolRef FileDesc = Call.getReturnValue().getAsSymbol();
  if (!FileDesc)
    return;

  llvm::errs() << "Detected pthread_create() call at " << FileDesc << "\n";
}


void ento::registerPthreadCreateChecker(CheckerManager &mgr) {
  mgr.registerChecker<PthreadCreateChecker>();
}

bool ento::shouldRegisterPthreadCreateChecker(const CheckerManager &mgr) {
  return true;
}