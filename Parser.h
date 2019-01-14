#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <list>
#include "lexer.h"

using namespace std;


LexTok currPair = { "","" }; // hold the lextok pair 
string curToken; // holds current token	
string curLexe; // hold current lexeme
int countLex; // gets the current count of lexemes in order to help show position in file when error occurs
int registers[10];
string identifiers[50];
int regNum = 0; // holds number of registers + 1 but also last register used(goes in order 1 to 10)
int idNum = 0; //number of identifiers
int savedReg;
int endIfs = 1;
int totalElses = 1;
string ifs;
string elses;
string relationOp;
string currID;
string temp;
string convert; //converts are used to convert ints to strings
string convert1;
string convert2;
vector <string> outPut;
int pos = 0;
int ifPos;
int elsePos; //hold branch positions
int elsePos1; 
int elseIfPos;
int elseIfPos1;//holds branch condition position
string branch; // hold the branch registers for elsepos1;


//function prototypes
bool checkIdent(string);
int addReg();
int getReg();
bool deleteReg(int);
void parser(ifstream & src);
void getLexTok(ifstream & src);
bool acceptTok(string, ifstream &);
bool acceptLex(string, ifstream &);
void expectTok(string, ifstream &);
void expectLex(string, ifstream &);
void writeError(ifstream & src);
void program(ifstream & src);
void DeclList(ifstream & src);
void Decl(ifstream & src);
void Type(ifstream & src);
void VarList(ifstream & src);
void StmtList(ifstream & src);
void Stmt(ifstream & src);
void Assign(ifstream & src);
void Read(ifstream & src);
void Write(ifstream & src);
void If(ifstream & src);
void While(ifstream & src);
void Expr(ifstream & src);
void Term(ifstream & src);
void Factor(ifstream & src);
void Cond(ifstream & src);
void RelOp(ifstream & src);

bool checkIdent(string id)
{
	for (int i = 0; i < idNum; i++)
	{
		if (identifiers[i] == id)
		{
			return true;
		}
	}
	return false;
}
int getReg()
{
	addReg();
	return regNum - 1;
}
int addReg()// adds register to array
{
	if (regNum == 10)
	{
		cout << "Error: Registers are full\n";
		return 10; // array full
	}
	registers[regNum] =  regNum;
	regNum++;
	return regNum;
}
bool deleteReg(int reg) // deletes registers in array
{
	if (regNum == 0)
	{
		cout << "Registers 0 is available \n";
		return false;
	}
	/*
	if (regNum == 1)
	{
	registers[regNum] = NULL;
	regNum--;
	return true;
	}
	*/
	for (int i = 0; i < regNum; i++)
	{
		if (registers[i] == reg)
		{
			for (int j = i; j < regNum - 1; j++)
			{
				registers[j] = registers[j + 1];
			}
			registers[regNum - 1] = NULL;
			regNum--;
			return true;
		}
	}
	return false;
}
void parser(ifstream & src) // parser calls first lex tok pair and then program
{
	getLexTok(src);
	program(src);
}
void getLexTok(ifstream & src) //call lexer function gets lexeme and token
{
	if (curToken != "eof" || curToken != "error")
	{
		currPair = lexer(src);
		curToken = currPair.token;
		curLexe = currPair.lexeme;
		countLex += 1;
	}
}
bool acceptTok(string s, ifstream & src) // checks token true or false gets next
{
	if (s == curToken)
	{
		getLexTok(src);
		return true;
	}
	return false;
}
bool acceptLex(string s, ifstream & src) // checks lexeme true or false gets next
{
	if (s == curLexe)
	{
		getLexTok(src);
		return true;
	}
	return false;
}
void expectTok(string s, ifstream & src) // expect token else error
{
	if (!acceptTok(s, src))
	{
		writeError(src);
	}
}
void expectLex(string s, ifstream & src) //expect lexeme otherwise error
{
	if (!acceptLex(s, src))
	{
		writeError(src);
	}
}
void writeError(ifstream & src) //print lexeme causing error and lexeme number, close file and exit program **trying to give info as to where the error is, so i keep a count of lexeme**
{
	cout << "Error: Unexpected string: " << "Lexeme #" << countLex << ": " << curLexe << " -- caused the error! \n";
	src.close();
	exit(1);
}
void program(ifstream & src) // call DecList, Funclist, Stmtlist if accepted 
{
	expectLex("program", src);
	outPut.push_back("	.data");
	pos++;
	if (curToken == "Type")
	{
		DeclList(src);
	}
	expectLex("begin", src);
	outPut.push_back("	.text");
	pos++;
	outPut.push_back("main:");
	pos++;
	if (curToken == "Ident" || curLexe == "read" || curLexe == "write" || curLexe == "if" ||
		curLexe == "while")
	{
		StmtList(src);
	}
	expectLex("end", src);
	expectLex(".", src);
	outPut.push_back("	li $v0, 10");
	pos++;
	outPut.push_back("	syscall");
	pos++;
	for (auto &i : outPut)
	{
		std::cout << i << endl;
	}
	//cout << "Program => program [DeclList][FuncList] begin [StmtList] end.\n";
}
void DeclList(ifstream & src) // call Decl then DeclList if accepted else print
{
	if (curToken == "Type")
	{
		Decl(src);
		DeclList(src);
	}
	else
	{
		//cout << "DecList => Decl {Decl} \n";
	}
}
void Decl(ifstream & src) // call Type and Varlist //maybe change later
{
	Type(src);
	//Varlist(src);
	if (currPair.token == "Ident")
	{
		if (!checkIdent(currPair.lexeme))
		{
			identifiers[idNum] = currPair.lexeme;
			idNum++;
			currPair.lexeme += ":	.word 0";
			outPut.push_back(currPair.lexeme);
			pos++;
			expectTok("Ident", src); // gets next pair, used for error purposes also
		}
		else
		{
			cout << "Error: Identifer - " << currPair.lexeme << " - is already in use\n";
			src.close();
			exit(1);
		}
	}
	while (acceptLex(",", src))
	{
		if (currPair.token == "Ident")
		{
			if (!checkIdent(currPair.lexeme))
			{
				identifiers[idNum] = currPair.lexeme;
				idNum++;
				currPair.lexeme += ":	.word 0";
				outPut.push_back(currPair.lexeme);
				pos++;
				expectTok("Ident", src);
			}
			else
			{
				cout << "Error: Identifer - " << currPair.lexeme << " - is already in use\n";
				src.close();
				exit(1);
			}
		}
	}
	expectLex(";", src);
	//cout << "Decl => Type VarList\n";
}
void Type(ifstream & src) // check type
{
	expectTok("Type", src);
	//cout << "Type => int | real | string \n";
}
void VarList(ifstream & src)// check ident loop while next lex is ,
{
	//expectTok("Ident", src);
	if (currPair.token == "Ident")
	{
		if (checkIdent(currPair.lexeme))
		{
			expectTok("Ident", src); // gets next pair, used for error purposes also
		}
		else
		{
			cout << "Error: Identifer - " << currPair.lexeme << " - not declared\n";
			src.close();
			exit(1);
		}
	}
	while (acceptLex(",", src))
	{
		if (currPair.token == "Ident")
		{
			if (checkIdent(currPair.lexeme))
			{
				expectTok("Ident", src);
			}
			else
			{
				cout << "Error: Identifer - " << currPair.lexeme << " - not declared\n";
				src.close();
				exit(1);
			}
		}
	}
	//cout << "VarList => Ident{,Ident} \n";
}
void StmtList(ifstream & src) // check that next lex or tok is acceptable then call else print
{
	if (curToken == "Ident" || curLexe == "read" || curLexe == "write" || curLexe == "if" ||
		curLexe == "while")
	{
		Stmt(src);
		StmtList(src);
	}
	else
	{
		//cout << "StmtList => Stmt {Stmt} \n";
	}

}
void Stmt(ifstream & src) // check ident, read, write, if, while, do and call appropriate function
{
	if (currPair.token == "Ident")
	{
		currID = currPair.lexeme;
		acceptTok("Ident", src);
		Assign(src);
		//cout << "Stmt => Assign \n";
	}
	else if (acceptLex("read", src))
	{
		Read(src);
		//cout << "Stmt => Read \n";
	}
	else if (acceptLex("write", src))
	{
		Write(src);
		//cout << "Stmt => Write \n";
	}
	else if (acceptLex("if", src))
	{
		If(src);
		//cout << "Stmt => If \n";
	}
	else if (acceptLex("while", src))
	{
		While(src);
		//cout << "Stmt => While \n";
	}
	else
	{

	}
}
void Assign(ifstream & src) // call expr
{
	expectLex(":=", src);
	Expr(src);
	expectLex(";", src);
	temp = ", " + currID;
	convert = to_string(regNum - 1);
	outPut.push_back("	sw $t" + convert + temp);
	pos++;
	deleteReg(regNum - 1);
	//cout << "Assign => Ident := Expr; \n";
}
void Read(ifstream & src) // call varList
{
	expectLex("(", src);
	VarList(src);
	for (int i = 0; i < idNum; i++)
	{
		outPut.push_back("	li $v0, 5");
		pos++;
		outPut.push_back("	syscall");
		pos++;
		outPut.push_back("	sw $v0, " + identifiers[i]);
		pos++;
	}
	expectLex(")", src);
	expectLex(";", src);
	//cout << "Read => read ( VarList ); \n";
}
void Write(ifstream & src) // call Expr loop while next lex is ,
{
	expectLex("(", src);
	Expr(src);
	convert = "$t" + to_string(regNum - 1);
	outPut.push_back("	li $v0, 1");
	pos++;
	outPut.push_back("	move $a0, " + convert);
	pos++;
	outPut.push_back("	syscall");
	pos++;
	deleteReg(regNum - 1);
	while (acceptLex(",", src))
	{
		Expr(src);
		convert = "$t" + to_string(regNum - 1);
		outPut.push_back("	li $v0, 1");
		pos++;
		outPut.push_back("	move $a0, " + convert);
		pos++;
		outPut.push_back("	syscall");
		pos++;
		deleteReg(regNum - 1);
	}
	expectLex(")", src);
	expectLex(";", src);
	//cout << "Write => write (Expr {, Expr}); \n";
}
void If(ifstream & src) //call cond and StmtList, check if elsif and/or else
{
	expectLex("(", src);
	Cond(src);
	expectLex(")", src);
	expectLex("begin", src);
	StmtList(src);
	outPut.push_back("	b " + ifs); // maybe take out
	pos++;
	expectLex("end", src);
	outPut.push_back(ifs + ":");
	elsePos = pos;
	pos++;
	while (acceptLex("elsif", src))
	{
		//convert = to_string(totalElses);
		//elses = "elseif" + convert;
		//outPut[elsePos1] = branch + elses;
		//pos++;
		expectLex("(", src);
		Cond(src);
		expectLex(")", src);
		expectLex("begin", src);
		StmtList(src);
		expectLex("end", src);
		convert = to_string(totalElses);
		elses = "elseif" + convert;
		outPut[elsePos] = elses;
		pos++;
		totalElses++;
		if (acceptLex("else", src))
		{
			convert = to_string(totalElses);
			elses = "else" + convert;
			outPut[elsePos1] = branch + elses; // get it terms of else ifs
			pos++;
			convert = to_string(totalElses);
			elses = "else:" + convert;
			outPut[elsePos] = elses; // in terms of else ifs
			pos++;
			totalElses++;
			expectLex("begin", src);
			StmtList(src);
			expectLex("end", src);
		}
	}
	if (acceptLex("else", src))
	{
		convert = to_string(totalElses);
		elses = "else" + convert;
		outPut[elsePos1] = branch + elses;
		pos++;
		convert = to_string(totalElses);
		elses = "else:" + convert;
		outPut[elsePos] = elses;
		pos++;
		totalElses++;
		expectLex("begin", src);
		StmtList(src);
		expectLex("end", src);
	}
	outPut.push_back(ifs + ":");
	//cout << "If => if (Cond) begin StmtList end{ elsif(Cond) begin StmtList end }[else begin StmtList end] \n";
}
void While(ifstream & src) // call cond then stmtList
{
	expectLex("(", src);
	Cond(src);
	expectLex(")", src);
	expectLex("begin", src);
	StmtList(src);
	expectLex("end", src);
	//cout << "While => while (cond) begin [StmtList] end \n";
}
void Expr(ifstream & src) // call term loop if + or - are next lexeme
{
	Term(src);
	while (currPair.lexeme == "+" || currPair.lexeme == "-")
	{
		if (currPair.lexeme == "+")
		{
			acceptLex("+", src);
			Term(src);
			convert = to_string(regNum - 2);
			convert1 = ", $t" + to_string(regNum - 2);
			convert2 = ", $t" + to_string(regNum - 1);
			outPut.push_back("	add $t" + convert + convert1 + convert2);
			pos++;
			deleteReg(regNum - 1);
		}
		else if (currPair.lexeme == "-")
		{

			acceptLex("-", src);
			Term(src);
			convert = to_string(regNum - 2);
			convert1 = ", $t" + to_string(regNum - 2);
			convert2 = ", $t" + to_string(regNum - 1);
			outPut.push_back("	sub $t" + convert + convert1 + convert2);
			pos++;
			deleteReg(regNum - 1);
		}
	}
	//cout << "Expr => Term {(+|-) Term} \n";
}
void Term(ifstream & src) //call factor look if * or / are next lexeme
{
	Factor(src);
	while (currPair.lexeme == "*" || currPair.lexeme == "/")
	{
		if (currPair.lexeme == "*")
		{
			acceptLex("*", src);
			Factor(src);
			convert = to_string(regNum - 2) + ", $t" + to_string(regNum - 1);
			convert1 = to_string(regNum - 2);
			outPut.push_back("	mult $t" + convert);
			pos++;
			outPut.push_back("	mflo $t" + convert1);
			pos++;
			deleteReg(regNum - 1);
		}
		else if (currPair.lexeme == "/")
		{
			acceptLex("/", src);
			Factor(src);
			convert = to_string(regNum - 2) + ", $t" + to_string(regNum - 1);
			convert1 = to_string(regNum - 2);
			outPut.push_back("	div $t" + convert);
			pos++;
			outPut.push_back("	mflo $t" + convert1);
			pos++;
			deleteReg(regNum - 1);
		}
	}
	//cout << "Term => Factor {(*|/) Factor} \n";
}
void Factor(ifstream & src) // check ident, intconst, expr 
{
	if (currPair.token == "Ident")
	{
		if (checkIdent(currPair.lexeme))
		{
			temp = ", " + currPair.lexeme;
			convert = to_string(getReg());
			outPut.push_back("	lw $t" + convert + temp);
			pos++;
			acceptTok("Ident", src); // gets next pair, used for error purposes also
		}
		else
		{
			cout << "Error: Identifer - " << currPair.lexeme << " - not declared\n";
			src.close();
			exit(1);
		}
		//cout << "Factor => Ident \n";
	}
	else if (currPair.token == "IntConst")
	{

		temp = ", " + currPair.lexeme;
		convert = to_string(getReg());
		outPut.push_back("	li $t" + convert + temp);
		pos++;
		acceptTok("IntConst", src);
		//cout << "Factor => IntConst \n";
	}
	else if (acceptLex("(", src))
	{
		//temp = ", $t " + regNum - 2;
		//convert = to_string(getReg());
		//outPut.push_back("	move $t" + convert + temp);
		//deleteReg(regNum - 2); // maybe fix
		Expr(src);
		expectLex(")", src);
		//cout << "Factor => (Expr) \n";
	}

}
void Cond(ifstream & src) //call expr relop expr
{
	Expr(src);
	RelOp(src);
	Expr(src);
	regNum--; //fix position, maybe change
	if (relationOp == ">")
	{
		acceptLex(">", src);
		// may have to change back to regnum -2 and regnum - 1 for all operators.
		convert = to_string(regNum - 1) + ", $t" + to_string(regNum) + ", "; 
		convert1 = to_string(endIfs);
		ifs = "endIf" + convert1;
		outPut.push_back("	ble $t" + convert + ifs);
		branch = "	ble $t" + convert;
		elsePos1 = pos;
		pos++;
		endIfs++;
		regNum--; //fix position again, maybe change
	}
	else if (relationOp == "<")
	{
		acceptLex("<", src);
		convert = to_string(regNum - 1) + ", $t" + to_string(regNum) + ", ";
		convert1 = to_string(endIfs);
		ifs = "endIf" + convert1;
		outPut.push_back("	bge $t" + convert + ifs);
		branch = "	bge $t" + convert;
		elsePos1 = pos;
		pos++;
		endIfs++;
		regNum--;
	}
	else if (relationOp == ">=")
	{
		acceptLex(">=", src);
		convert = to_string(regNum - 1) + ", $t" + to_string(regNum) + ", ";
		convert1 = to_string(endIfs);
		ifs = "endIf" + convert1;
		outPut.push_back("	blt $t" + convert + ifs);
		branch = "	blt $t" + convert;
		elsePos1 = pos;
		pos++;
		endIfs++;
		regNum--;
	}
	else if (relationOp == "<=")
	{
		acceptLex("<=", src);
		convert = to_string(regNum - 1) + ", $t" + to_string(regNum) + ", ";
		convert1 = to_string(endIfs);
		ifs = "endIf" + convert1;
		outPut.push_back("	bgt $t" + convert + ifs);
		branch = "	bgt $t" + convert;
		elsePos1 = pos;
		pos++;
		endIfs++;
		regNum--;
	}
	else if (relationOp == "=")
	{
		acceptLex("=", src);
		convert = to_string(regNum - 1) + ", $t" + to_string(regNum) + ", ";
		convert1 = to_string(endIfs);
		ifs = "endIf" + convert1;
		outPut.push_back("	bne $t" + convert + ifs);
		branch = "	bne $t" + convert;
		elsePos1 = pos;
		pos++;
		endIfs++;
		regNum--;
	}
	else if (relationOp == "<>")
	{
		acceptLex("<>", src);
		convert = to_string(regNum - 1) + ", $t" + to_string(regNum) + ", ";
		convert1 = to_string(endIfs);
		ifs = "endIf" + convert1;
		outPut.push_back("	beg $t" + convert + ifs);
		branch = "	beg $t" + convert;
		elsePos1 = pos;
		pos++;
		endIfs++;
		regNum--;
	}
	//cout << "Cond => Expr RelOp Expr \n";
}
void RelOp(ifstream & src) //check if relop
{
	if (currPair.token == "RelOp")
	{
		relationOp = currPair.lexeme;
		acceptTok("RelOp", src);
	}
	else
	{
		writeError(src);
	}
}