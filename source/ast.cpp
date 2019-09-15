#include "../header/ast.hpp"

using namespace std;

//*********************************************************************
InnerExprAST::~InnerExprAST() {
  for (auto i : Vec)
    delete i;
}

InnerExprAST::InnerExprAST(ExprAST *e1) {
  Vec.push_back(e1);
}

InnerExprAST::InnerExprAST(ExprAST *e1, ExprAST *e2) {
  Vec.push_back(e1);
  Vec.push_back(e2);
}

InnerExprAST::InnerExprAST(ExprAST *e1, ExprAST *e2, ExprAST *e3) {
  Vec.push_back(e1);
  Vec.push_back(e2);
  Vec.push_back(e3);
}

InnerExprAST::InnerExprAST(ExprAST *e1, ExprAST *e2, ExprAST *e3, ExprAST *e4) {
  Vec.push_back(e1);
  Vec.push_back(e2);
  Vec.push_back(e3);
  Vec.push_back(e4);
}
//*********************************************************************

void NumberExprAST::codegen() const {
	cout << Val << endl;
}

void VarExprAST::codegen() const{
	cout << "Addr:" << adr << endl;
}

void PrintNumExprAST::codegen() const {
	cout << "Print number" << endl;
	Vec[0]->codegen();
}

void PrintCharExprAST::codegen() const {
	cout << "Print character" << endl;
	Vec[0]->codegen();
}

void AddExprAST::codegen() const {
	cout << "Add" << endl;
	Vec[0]->codegen();
	Vec[1]->codegen();
}

void MulExprAST::codegen() const {
	cout << "Mul" << endl;
	Vec[0]->codegen();
	Vec[1]->codegen();
}

void IncExprAST::codegen() const {
	cout << "Inc" << endl;
}
