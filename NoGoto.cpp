#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"


using namespace clang;

class GotoStmtVisitor
  : public RecursiveASTVisitor<GotoStmtVisitor> {
public:
  explicit GotoStmtVisitor(ASTContext *Context)
    : Context(Context) {}

  bool VisitStmt(Stmt *S) {
    
      if(isa<GotoStmt> (S) || isa<IndirectGotoStmt>(S)){
     FullSourceLoc FL = Context->getFullLoc(S->getBeginLoc());
      if (FL.isValid())
        llvm::outs() << "NO GOTO at " << FL.getSpellingLineNumber() << ":" << FL.getSpellingColumnNumber() << "\n"; 
    }
    

    return true;
  }

private:
  ASTContext *Context;
};

class NoGotoConsumer : public clang::ASTConsumer {
public:
  explicit NoGotoConsumer(ASTContext *Context)
    : Visitor(Context) {}

  virtual void HandleTranslationUnit(clang::ASTContext &Context) override {
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }
private:
  GotoStmtVisitor Visitor;
};

class NoGotoAction : public clang::ASTFrontendAction {
public:
  virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
    clang::CompilerInstance &Compiler, llvm::StringRef InFile) override {
    return std::make_unique<NoGotoConsumer>(&Compiler.getASTContext());
  }
};

int main(int argc, char **argv) {
  if (argc > 1) {
    clang::tooling::runToolOnCode(std::make_unique<NoGotoAction>(), argv[1]);
  }
}