#ifndef __AST_HPP__
#define __AST_HPP__ 1

#include <vector>
#include <iostream>

using namespace std;

class ExprAST {
public:
  virtual void codegen() const = 0;
	virtual int interpret() const = 0;
  virtual ~ExprAST() {

  }
};

class NumberExprAST : public ExprAST {
public:
  NumberExprAST(int v)
    :Val(v)
  {}
  void codegen() const;
	int interpret() const;
private:
  int Val;
};

class VarExprAST : public ExprAST {
public:
	VarExprAST(int adr) : adr(adr) {}
	void codegen() const;
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

class PrintNumExprAST : public InnerExprAST {
public:
	PrintNumExprAST(ExprAST* e): InnerExprAST(e){}
	void codegen() const;
	int interpret() const;
};

class PrintCharExprAST : public InnerExprAST {
public:
	PrintCharExprAST(ExprAST* e): InnerExprAST(e){}
	void codegen() const;
	int interpret() const;
};

class AddExprAST : public InnerExprAST {
public:
	AddExprAST(ExprAST* l, ExprAST* r, int a): InnerExprAST(l,r), adr(a){}
	void codegen() const;
	int interpret() const;
private:
	int adr;
};

class MulExprAST : public InnerExprAST {
public:
	MulExprAST(ExprAST* l, ExprAST* r, int a): InnerExprAST(l,r), adr(a){}
	void codegen() const;
	int interpret() const;
private:
	int adr;
};

class IncExprAST : public ExprAST {
public:
	IncExprAST(int adr): adr(adr){}
	void codegen() const;
	int interpret() const;
private:
	int adr;
};

#endif
