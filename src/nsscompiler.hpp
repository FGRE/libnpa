#ifndef NSS_COMPILER_HPP
#define NSS_COMPILER_HPP

#include <vector>
#include <cstdint>
#include <string>
#include "nsbmagic.hpp"
#include "npafile.hpp"
using namespace std;

class Statement;
class Argument;
class Subroutine;
typedef vector<Statement*> StatementList;
typedef vector<Argument*> ArgumentList;
typedef vector<Subroutine*> SubroutineList;

enum ArgumentType
{
    ARG_INT = 0,
    ARG_STRING,
    ARG_FLOAT,
    ARG_VARIABLE,
    ARG_FUNCTION
};

struct Node
{
    virtual ~Node() {}
    void Compile(uint16_t Magic, uint16_t NumParams);
};

struct Statement : virtual Node
{
    virtual void Compile() = 0;
};

struct Expression : virtual Node
{
    virtual void Compile() = 0;
};

struct Argument : Expression
{
    Argument(const string& Data, ArgumentType Type) : Data(NpaFile::FromUtf8(Data)), Type(Type) {}
    void CompileRaw();
    virtual void Compile();

    string Data;
    ArgumentType Type; // Todo: CallArgument : Argument?
};

struct Call : Expression, Statement
{
    Call(Argument& Name, ArgumentList& Arguments, uint16_t Magic) : Name(Name), Arguments(Arguments), Magic(Magic) {}
    virtual void Compile();

    uint16_t Magic;
    Argument& Name;
    ArgumentList Arguments;
};

struct CallStatement : Call
{
    CallStatement(Argument& Name, ArgumentList& Arguments, uint16_t Magic) : Call(Name, Arguments, Magic) {}
    virtual void Compile();
};

struct Block : Statement
{
    Block() {}
    Block(StatementList& Statements) : Statements(Statements) {}
    virtual void Compile();

    StatementList Statements;
};

struct Condition : Statement
{
    Condition(Block& ConditionBlock, Expression& Expr, uint16_t Magic) : ConditionBlock(ConditionBlock), Expr(Expr), Magic(Magic) {}
    virtual void Compile();

    uint16_t Magic;
    Expression& Expr;
    Block& ConditionBlock;
};

struct Else : Statement
{
    Else(Block& ElseBlock) : ElseBlock(ElseBlock) {}
    virtual void Compile();

    Block& ElseBlock;
};

struct Select : Statement
{
    Select(Block& SelectBlock) : SelectBlock(SelectBlock) {}
    virtual void Compile();

    Block& SelectBlock;
};

struct Case : Statement
{
    Case(const string& Name, Block& CaseBlock) : Name(NpaFile::FromUtf8(Name)), CaseBlock(CaseBlock) {}
    virtual void Compile();

    string Name;
    Block& CaseBlock;
};

struct Subroutine : Node
{
    Subroutine(Argument& Name, Block& SubroutineBlock) : Name(Name), SubroutineBlock(SubroutineBlock) {}
    void CompilePrototype(uint16_t BeginMagic, uint32_t NumBeginParams);
    virtual void Compile();
    void CompileReturn(uint16_t EndMagic);

    const uint16_t NumEndParams = 0;
    Argument& Name;
    Block& SubroutineBlock;
};

struct Program
{
    virtual void Compile();

    SubroutineList Subroutines;
};

struct Function : Subroutine
{
    Function(Argument& Name, ArgumentList& Arguments, Block& Statements) : Subroutine(Name, Statements), Arguments(Arguments) {}
    virtual void Compile();

    ArgumentList Arguments;
};

struct Chapter : Subroutine
{
    Chapter(Argument& Name, Block& Statements) : Subroutine(Name, Statements) {}
    virtual void Compile();
};

struct Scene : Subroutine
{
    Scene(Argument& Name, Block& Statements) : Subroutine(Name, Statements) {}
    virtual void Compile();
};

struct Assignment : Expression
{
    Assignment(Argument& Name, Expression& Rhs, uint16_t Magic) : Name(Name), Rhs(Rhs), Magic(Magic) {}
    virtual void Compile();

    uint16_t Magic;
    Argument& Name;
    Expression& Rhs;
};

struct BinaryOperator : Expression
{
    BinaryOperator(Expression& Lhs, uint16_t Op, Expression& Rhs) : Lhs(Lhs), Magic(Op), Rhs(Rhs) {}
    virtual void Compile();

    uint16_t Magic;
    Expression& Lhs;
    Expression& Rhs;
};

struct UnaryOperator : Expression
{
    UnaryOperator(int Op, Expression& Rhs) : Magic(Op), Rhs(Rhs) {}
    virtual void Compile();

    uint16_t Magic;
    Expression& Rhs;
};

Call* MakeCall(string Name, uint16_t Magic);

#endif
