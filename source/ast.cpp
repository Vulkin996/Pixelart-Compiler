#include "../header/ast.hpp"

using namespace std;

extern int registers[];


extern void yyerror(string s);

LLVMContext TheContext;
IRBuilder<> Builder(TheContext);
Module* TheModule;
map<string, AllocaInst*> NamedValues;
legacy::FunctionPassManager* TheFPM;

extern Function* PrintfFun;
extern Function* MainFun;
extern Value* StrPrintNum;
extern Value* StrPrintChar;

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

Value* NumberExprAST::codegen() const {
		return ConstantInt::get(TheContext, APInt(32, Val));
}

Value* VarExprAST::codegen() const{
	string name("r" + to_string(adr));
	AllocaInst* tmp = NamedValues[name];
	if (tmp == nullptr)
    yyerror("Invalid Address!");
	return Builder.CreateLoad(tmp, name);
}

Value* SeqExprAST::codegen() const{
	Value* tmp = NULL;
	for(auto ins : Vec){
		tmp = ins->codegen();
		if(tmp == NULL)
			return NULL;
	}
	return tmp;
}

Value* PrintNumExprAST::codegen() const {
	Value* toPrint = Vec[0]->codegen();
	if(toPrint == NULL)
		return NULL;

	vector<Value *> ArgsV;
	ArgsV.push_back(StrPrintNum);
	ArgsV.push_back(toPrint);
	return Builder.CreateCall(PrintfFun, ArgsV, "printfCall");
}

Value* PrintCharExprAST::codegen() const {
	Value* toPrint = Vec[0]->codegen();
	if(toPrint == NULL)
		return NULL;

	vector<Value *> ArgsV;
	ArgsV.push_back(StrPrintChar);
	ArgsV.push_back(toPrint);
	return Builder.CreateCall(PrintfFun, ArgsV, "printfCall");
}

Value* AddExprAST::codegen() const {
	Value *l = Vec[0]->codegen();
  Value *r = Vec[1]->codegen();
  if (!l || !r)
    return nullptr;

	string reg_adr("r" + to_string(adr));
	AllocaInst* alloca = NamedValues[reg_adr];
	if (alloca == nullptr){
		 alloca = CreateEntryBlockAlloca(MainFun, reg_adr);
		 NamedValues[reg_adr] = alloca;
	}
	Value* Val = Builder.CreateAdd(l, r, "addtmp");
	return Builder.CreateStore(Val, alloca);
}

Value* MulExprAST::codegen() const {
	Value *l = Vec[0]->codegen();
  Value *r = Vec[1]->codegen();
  if (!l || !r)
    return nullptr;

	string reg_adr("r" + to_string(adr));
	AllocaInst* alloca = NamedValues[reg_adr];
	if (alloca == nullptr){
		 alloca = CreateEntryBlockAlloca(MainFun, reg_adr);
		 NamedValues[reg_adr] = alloca;
	}
	Value* Val = Builder.CreateMul(l, r, "multmp");
	return Builder.CreateStore(Val, alloca);
}

Value* IncExprAST::codegen() const {
	string reg_adr("r" + to_string(adr));
	AllocaInst* alloca = NamedValues[reg_adr];
	if (alloca == nullptr)
    yyerror("Invalid Address!");

	Value* Val = Builder.CreateLoad(alloca, reg_adr);

	Value* NewVal = Builder.CreateMul(Val, ConstantInt::get(TheContext, APInt(32, 1)), "inctmp");
	return Builder.CreateStore(NewVal, alloca);
}

Value* IfElseExprAST::codegen() const {
	Value* CondV = Vec[0]->codegen();
  if (CondV == nullptr)
    return nullptr;

	Value *IfCondV = Builder.CreateICmpEQ(CondV, ConstantInt::get(TheContext, APInt(32, 0)), "ifcond");

	Function* TheFunction = Builder.GetInsertBlock()->getParent();
  BasicBlock* ThenBB = BasicBlock::Create(TheContext, "then", TheFunction);
  BasicBlock* ElseBB = BasicBlock::Create(TheContext, "else");
  BasicBlock* MergeBB = BasicBlock::Create(TheContext, "ifcont");

	Builder.CreateCondBr(IfCondV, ElseBB, ThenBB);

  Builder.SetInsertPoint(ThenBB);
  Value* ThenV = Vec[1]->codegen();
  if (ThenV == nullptr)
    return nullptr;
  Builder.CreateBr(MergeBB);
  ThenBB = Builder.GetInsertBlock();

	TheFunction->getBasicBlockList().push_back(ElseBB);
  Builder.SetInsertPoint(ElseBB);
  Value* ElseV = Vec[2]->codegen();
  if (ElseV == nullptr)
    return nullptr;
  Builder.CreateBr(MergeBB);
  ElseBB = Builder.GetInsertBlock();

  TheFunction->getBasicBlockList().push_back(MergeBB);
  Builder.SetInsertPoint(MergeBB);
  PHINode *PHI = Builder.CreatePHI(Type::getInt32Ty(TheContext), 2, "iftmp");
  PHI->addIncoming(ThenV, ThenBB);
  PHI->addIncoming(ElseV, ElseBB);

  return PHI;
}

Value* WhileExprAST::codegen() const {
		BasicBlock* CondBB = BasicBlock::Create(TheContext, "cond");
	  BasicBlock* LoopBB = BasicBlock::Create(TheContext, "loop");
	  BasicBlock* AfterLoopBB = BasicBlock::Create(TheContext, "afterloop");
	  Function* TheFunction = Builder.GetInsertBlock()->getParent();

		Builder.CreateBr(CondBB);
		TheFunction->getBasicBlockList().push_back(CondBB);
	  Builder.SetInsertPoint(CondBB);

		Value* CondVal = Vec[0]->codegen();
	  if (CondVal == nullptr){
	    return nullptr;
		}
		Value* NewCondVal = Builder.CreateICmpEQ(CondVal, ConstantInt::get(TheContext, APInt(32, 0)), "whilecond");
		Builder.CreateCondBr(NewCondVal, AfterLoopBB, LoopBB);

		TheFunction->getBasicBlockList().push_back(LoopBB);
	  Builder.SetInsertPoint(LoopBB);

		Value* BodyVal = Vec[1]->codegen();
	  if (BodyVal == nullptr){
			//cout << "BODY MISSING!" << endl;
	    return nullptr;
		}

		Builder.CreateBr(CondBB);

		TheFunction->getBasicBlockList().push_back(AfterLoopBB);
	  Builder.SetInsertPoint(AfterLoopBB);

		return ConstantInt::get(TheContext, APInt(32, 0));
}
//*********************************************************************

int NumberExprAST::interpret() const {
	return Val;
}

int VarExprAST::interpret() const{
	return registers[adr];
}

int SeqExprAST::interpret() const {
	int tmp;
	for(auto ins : Vec){
		tmp = ins->interpret();
	}
	return tmp;
}

int PrintNumExprAST::interpret() const {
  printf("%d\n", Vec[0]->interpret());
	return 0;
}

int PrintCharExprAST::interpret() const {
	printf("%c\n", Vec[0]->interpret());
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
	int cond = Vec[0]->interpret();
	if(cond){
		Vec[1]->interpret();
	}
	else{
		Vec[2]->interpret();
	}
	return 0;
}

int WhileExprAST::interpret() const {
	int cond = Vec[0]->interpret();
	while(cond){
		cond = Vec[0]->interpret();
		Vec[1]->interpret();
	}
	return 0;
}
//*********************************************************************

AllocaInst *CreateEntryBlockAlloca(Function *TheFunction, const string &VarName) {
  IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
  return TmpB.CreateAlloca(Type::getInt32Ty(TheContext), 0, VarName.c_str());
}

void InitializeModuleAndPassManager(void){
	TheModule = new Module("pixel_module", TheContext);

  // Create a new pass manager attached to it.
  TheFPM = new legacy::FunctionPassManager(TheModule);

  // Do simple "peephole" optimizations and bit-twiddling optzns.
  TheFPM->add(createInstructionCombiningPass());
  // Reassociate expressions.
  TheFPM->add(createReassociatePass());
  // Eliminate Common SubExpressions.
  TheFPM->add(createNewGVNPass());
  // Simplify the control flow graph (deleting unreachable blocks, etc).
  TheFPM->add(createCFGSimplificationPass());
  TheFPM->add(createPromoteMemoryToRegisterPass());

  TheFPM->doInitialization();
}
