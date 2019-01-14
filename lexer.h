#include <string>
#include <fstream>
using namespace std;
//string keyword array holding all keywords except types
string keyword[] = { "program","begin","end","function","read","write","if","else","elsif","do",
"until","while","return" };

//create LexTok struct with 2 strings lexem and token
struct LexTok
{
	string lexeme, token;
};
//lexer function to read each char at a time and find lexemes and tokens
LexTok lexer(ifstream & src)
{
	LexTok newPair; // to store new pair of lexeme and token
	string lexStr = "";
	char ch;
	while (!src.eof())
	{
		if (src >> ch)
		{	//check if letter
			lexStr += ch;
			if (isalpha(ch))
			{
				//src.get(ch);
				ch = src.peek();
				//loop getting all letters in word whether its a digit or _
				while (isalpha(ch) || isdigit(ch) || ch == '_')
				{	//add next ch to string
					src.get(ch);
					lexStr += ch;
					ch = src.peek();
				}

				//check if token is keyword
				for (int i = 0; i < 13; i++)
				{
					if (keyword[i] == lexStr)
					{
						newPair.token = "Keyword";
					}

				}
				//if not keyword and has letters then identifier or type
				if (newPair.token != "Keyword")
				{
					if (lexStr == "int")
					{
						newPair.token = "Type";
					}
					else if (lexStr == "real")
					{
						newPair.token = "Type";
					}
					else if (lexStr == "string")
					{
						newPair.token = "Type";
					}
					else
					{
						newPair.token = "Ident";
					}
				}
				newPair.lexeme = lexStr;
				return newPair;
			}
			//check if digit
			else if (isdigit(ch))
			{
				ch = src.peek();
				while (isdigit(ch)) //loop til end of digits
				{
					src.get(ch);
					lexStr += ch;
					ch = src.peek();
				}
				if (ch == '.') // if decimal check if real number 
				{
					src.get(ch);
					lexStr += ch;
					ch = src.peek();
					if (!isdigit(ch)) //if no number after decimal then its putback, fix string and token = intconst
					{
						src.putback(ch);
						lexStr.length() - 1;
						newPair.token = "IntConst";
						newPair.lexeme = lexStr;
						return newPair;

					}
					while (isdigit(ch)) //if number after decimal then get number and token = realconst
					{
						src.get(ch);
						lexStr += ch;
						ch = src.peek();
					}
					newPair.token = "RealConst";
					newPair.lexeme = lexStr;
					return newPair;
				}
				//if decimal is never encountered then token = intconst
				newPair.token = "IntConst";
				newPair.lexeme = lexStr;
				return newPair;
			}
			//not letter or digit then check all single character operators first
			// could of done for loop with array instead
			else if (ch == '"')
			{
				ch = src.peek();
				while (ch != '"')
				{
					//**if no quote at the end and whitespace then error.	
					// **not sure exactly how to handle this

					src.get(ch);
					lexStr += ch;
					ch = src.peek();
				}
				src.get();
				lexStr += ch;
				newPair.lexeme = lexStr;
				newPair.token = "StrConst";
				return newPair;
			}
			else if (lexStr == "[")
			{
				newPair.lexeme = lexStr;
				newPair.token = "Operator";
				return newPair;
			}
			else if (lexStr == "]")
			{
				newPair.lexeme = lexStr;
				newPair.token = "Operator";
				return newPair;
			}
			else if (lexStr == "(") //check (
			{
				newPair.lexeme = lexStr;
				newPair.token = "Operator";
				return newPair;
			}
			else if (lexStr == ")") // check )
			{
				newPair.lexeme = lexStr;
				newPair.token = "Operator";
				return newPair;
			}
			else if (lexStr == ";") // check ;
			{
				newPair.lexeme = lexStr;
				newPair.token = "Operator";
				return newPair;
			}
			else if (lexStr == "+")//check +
			{
				newPair.lexeme = lexStr;
				newPair.token = "Operator";
				return newPair;
			}
			else if (lexStr == "-")// check -
			{
				newPair.lexeme = lexStr;
				newPair.token = "Operator";
				return newPair;
			}
			else if (lexStr == "*")// check *
			{
				newPair.lexeme = lexStr;
				newPair.token = "Operator";
				return newPair;
			}
			else if (lexStr == "/")//check /
			{
				newPair.lexeme = lexStr;
				newPair.token = "Operator";
				return newPair;
			}
			else if (lexStr == ",")//check ,
			{
				newPair.lexeme = lexStr;
				newPair.token = "Operator";
				return newPair;
			}
			else if (lexStr == ".")//check .
			{
				newPair.lexeme = lexStr;
				newPair.token = "Operator";
				return newPair;
			}
			// check two character operator :=
			else if (lexStr == ":")
			{
				ch = src.peek();
				if (ch == '=') // check if :=
				{
					src.get(ch);
					lexStr += ch;
					newPair.lexeme = lexStr;
					newPair.token = "Operator";
				}
				else
				{
					newPair.lexeme = lexStr;
					newPair.token = "Operator";
				}
				return newPair;
			}
			//now check relation operators
			else if (lexStr == "=")
			{
				newPair.lexeme = lexStr;
				newPair.token = "RelOp";
				return newPair;
			}
			else if (lexStr == ">") // check >
			{
				ch = src.peek();
				if (ch == '=') //check if >= 
				{
					src.get(ch);
					lexStr += ch;
					newPair.lexeme = lexStr;
					newPair.token = "RelOp";
				}
				else
				{
					newPair.lexeme = lexStr;
					newPair.token = "RelOp";
				}
				return newPair;
			}
			else if (lexStr == "<")//check <
			{
				ch = src.peek();
				if (ch == '=') // check if <= 
				{
					src.get(ch);
					lexStr += ch;
					newPair.lexeme = lexStr;
					newPair.token = "RelOp";
				}
				else if (ch == '>') //check if <>
				{
					src.get(ch);
					lexStr += ch;
					newPair.lexeme = lexStr;
					newPair.token = "RelOp";
				}
				else
				{
					newPair.lexeme = lexStr;
					newPair.token = "RelOp";
				}
				return newPair;
			}
			else // character not recognized, return char, token = error, stop all output
			{
				newPair.lexeme = lexStr;
				newPair.token = "error";
				return newPair;
			}
		}
	}
	//end of file return nothing and token = eof
	newPair.lexeme = lexStr;
	newPair.token = "eof";
	return newPair;
};

