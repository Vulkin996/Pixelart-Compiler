#include "../header/ast.hpp"

using namespace std;

extern int registers[];

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

void IfElseExprAST::codegen() const {
  cout << "If " << adr <<  endl;
  cout << "Then" << endl;
	Vec[0]->codegen();
  cout << "Else" << endl;
  Vec[1]->codegen();
}

void WhileExprAST::codegen() const {
  cout << "While " << adr <<  endl;
	Vec[0]->codegen();
}
//*********************************************************************

int NumberExprAST::interpret() const {
	return Val;
}

int VarExprAST::interpret() const{
	return registers[adr];
}

int PrintNumExprAST::interpret() const {
  printf("Print number: %d\n", Vec[0]->interpret());
	return 0;
}

int PrintCharExprAST::interpret() const {
	printf("Print number: %c\n", Vec[0]->interpret());
	return 0;
}

int AddExprAST::interpret() const {
	int l = Vec[0]->interpret();
	int r = Vec[1]->interpret();
	registers[adr] = l + r;
	return registers[adr];
}

int MulExprAST::interpret() const {
	int l = Vec[0]->interpret();
	int r = Vec[1]->interpret();
	registers[adr] = l * r;
	return registers[adr];
}

int IncExprAST::interpret() const {
	registers[adr]++;
	return registers[adr];
}

int IfElseExprAST::interpret() const {
  if (registers[adr] == 0){
    Vec[0]->interpret();
  }
  else{
    Vec[1]->interpret();
  }

  return 0;
}

int WhileExprAST::interpret() const {
  while (registers[adr] == 0){
    Vec[0]->interpret();
  }

  return 0;
}
//*********************************************************************
