#ifndef __AST_HPP__
#define __AST_HPP__ 1

#include <vector>
#include <iostream>

using namespace std;

#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Support/FileSystem.h"

using namespace llvm;
using namespace llvm::legacy;

class ExprAST {
public:
  virtual Value* codegen() const = 0;
	virtual int interpret() const = 0;
  virtual ~ExprAST() {

  }
};

class NumberExprAST : public ExprAST {
public:
  NumberExprAST(int v)
    :Val(v)
  {}
  Value* codegen() const;
	int interpret() const;
private:
  int Val;
};

class VarExprAST : public ExprAST {
public:
	VarExprAST(int adr) : adr(adr) {}
	Value* codegen() const;
	int interpret() const;
private:
	int adr;
};

//*****************************************************************************************
class InnerExprAST : public ExprAST {
public:
  InnerExprAST(vector<ExprAST*> v)
    :Vec(v)
  {}
  InnerExprAST(ExprAST* e1);
  InnerExprAST(ExprAST* e1, ExprAST* e2);
  InnerExprAST(ExprAST* e1, ExprAST* e2, ExprAST* e3);
  InnerExprAST(ExprAST* e1, ExprAST* e2, ExprAST* e3, ExprAST* e4);
  ~InnerExprAST();
private:
  InnerExprAST(const InnerExprAST&);
  InnerExprAST& operator=(const InnerExprAST&);
protected:
  vector<ExprAST*> Vec;
};
//*****************************************************************************************

class SeqExprAST : public InnerExprAST {
public:
	SeqExprAST(vector<ExprAST*> ve): InnerExprAST(ve) {}
	Value* codegen() const;
	int interpret() const;
};

class PrintNumExprAST : public InnerExprAST {
public:
	PrintNumExprAST(ExprAST* e): InnerExprAST(e){}
	Value* codegen() const;
	int interpret() const;
};

class PrintCharExprAST : public InnerExprAST {
public:
	PrintCharExprAST(ExprAST* e): InnerExprAST(e){}
	Value* codegen() const;
	int interpret() const;
};

class AddExprAST : public InnerExprAST {
public:
	AddExprAST(ExprAST* l, ExprAST* r, int a): InnerExprAST(l,r), adr(a){}
	Value* codegen() const;
	int interpret() const;
private:
	int adr;
};

class MulExprAST : public InnerExprAST {
public:
	MulExprAST(ExprAST* l, ExprAST* r, int a): InnerExprAST(l,r), adr(a){}
	Value* codegen() const;
	int interpret() const;
private:
	int adr;
};

class LtExprAST : public InnerExprAST {
public:
	LtExprAST(ExprAST* l, ExprAST* r, int a): InnerExprAST(l,r), adr(a){}
	Value* codegen() const;
	int interpret() const;
private:
	int adr;
};

class EqExprAST : public InnerExprAST {
public:
	EqExprAST(ExprAST* l, ExprAST* r, int a): InnerExprAST(l,r), adr(a){}
	Value* codegen() const;
	int interpret() const;
private:
	int adr;
};

class IncExprAST : public ExprAST {
public:
	IncExprAST(int adr): adr(adr){}
	Value* codegen() const;
	int interpret() const;
private:
	int adr;
};

class IfElseExprAST : public InnerExprAST {
public:
	IfElseExprAST(ExprAST* cond, ExprAST* ifBlock, ExprAST* elseBlock): InnerExprAST(cond, ifBlock, elseBlock){}
	Value* codegen() const;
	int interpret() const;
};

class WhileExprAST : public InnerExprAST {
public:
  WhileExprAST(ExprAST* cond, ExprAST* whileBlock): InnerExprAST(cond, whileBlock) {}
	Value* codegen() const;
	int interpret() const;

};

//*********************************************************************************************************
void InitializeModuleAndPassManager(void);
AllocaInst *CreateEntryBlockAlloca(Function *TheFunction, const string &VarName);

#endif
