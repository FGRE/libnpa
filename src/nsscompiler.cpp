#include <cstring>
#include "nsscompiler.hpp"
#include "scriptfile.hpp"
#include "parser.hpp"
#include "buffer.hpp"
using namespace NpaPrivate;

class Program;
extern Program* pRoot;
extern int yyparse();
typedef struct yy_buffer_state* YY_BUFFER_STATE;
extern int yyparse();
#if ((FLEX_VERSION_MAJOR) > 2 || \
    ((FLEX_VERSION_MAJOR == 2 && FLEX_VERSION_MINOR > 5) || \
    ((FLEX_VERSION_MAJOR == 2 && FLEX_VERSION_MINOR == 5 && FLEX_VERSION_PATCH > 35))))
extern YY_BUFFER_STATE yy_scan_bytes(const char* bytes, size_t len);
#else
extern YY_BUFFER_STATE yy_scan_bytes(const char* bytes, int len);
#endif
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

static uint32_t Counter = 1;
static Buffer* Output;
static Buffer* MapOutput;

const char* ArgumentTypes[] =
{
    "INT",
    "STRING",
    "FLOAT"
};

Call* MakeCall(string Name, uint16_t Magic)
{
    ArgumentList Args;
    Args.push_back(new Argument(Name, ARG_STRING));
    Argument* Arg = new Argument(Nsb::StringifyMagic(Magic), ARG_FUNCTION);
    return new Call(*Arg, Args, Magic);
}

void Node::Compile(uint16_t Magic, uint16_t NumParams)
{
    Output->Write((char*)&Counter, sizeof(uint32_t));
    Output->Write((char*)&Magic, sizeof(uint16_t));
    Output->Write((char*)&NumParams, sizeof(uint16_t));
    ++Counter;
}

void Argument::Compile()
{
    // Value
    if (Type == ARG_VARIABLE)
        Node::Compile(MAGIC_GET, 1);
    // Variable
    else
    {
        Node::Compile(MAGIC_LITERAL, 2);
        std::string Data = NpaFile::FromUtf8(ArgumentTypes[Type]);
        uint32_t TypeSize = Data.size();
        Output->Write((char*)&TypeSize, sizeof(uint32_t));
        Output->Write(Data.c_str(), TypeSize);
    }
    CompileRaw();
}

void Argument::CompileRaw()
{
    uint32_t Size = Data.size();
    Output->Write((char*)&Size, sizeof(uint32_t));
    Output->Write(Data.c_str(), Size);
}

void Call::Compile()
{
    uint16_t NumParams = Arguments.size();
    uint32_t BuiltinMagic = Nsb::MagicifyString(Name.Data.c_str());

    // Parameters
    if (BuiltinMagic != MAGIC_PARSE_TEXT &&
        BuiltinMagic != MAGIC_CALL_SCENE &&
        BuiltinMagic != MAGIC_CALL_CHAPTER)
        for (auto i = Arguments.begin(); i != Arguments.end(); ++i)
            (*i)->Compile();

    // Builtin function
    if (BuiltinMagic != 0)
        Node::Compile(BuiltinMagic, NumParams);
    // Script function
    else
    {
        NumParams += 1;
        Node::Compile(Magic, NumParams);
        Name.CompileRaw();
    }
    // Arguments
    for (auto i = Arguments.begin(); i != Arguments.end(); ++i)
        (*i)->CompileRaw();

    if (BuiltinMagic != MAGIC_PARSE_TEXT &&
        BuiltinMagic != MAGIC_CALL_SCENE &&
        BuiltinMagic != MAGIC_CALL_CHAPTER)
        Node::Compile(MAGIC_CLEAR_PARAMS, 0);
}

void Block::Compile()
{
    Node::Compile(MAGIC_SCOPE_BEGIN, 0);
    for (auto i = Statements.begin(); i != Statements.end(); ++i)
        (*i)->Compile();
    Node::Compile(MAGIC_SCOPE_END, 0);
}

void Subroutine::CompilePrototype(uint16_t BeginMagic, uint32_t NumBeginParams)
{
    // Write symbol to .map
    uint32_t Pos = Output->Size();
    uint16_t Size = Name.Data.size();
    MapOutput->Write((char*)&Pos, sizeof(uint32_t));
    MapOutput->Write((char*)&Size, sizeof(uint16_t));
    MapOutput->Write(Name.Data.c_str(), Size);

    // Compile
    Node::Compile(BeginMagic, NumBeginParams);
    Name.CompileRaw();
}

void Subroutine::Compile()
{
    SubroutineBlock.Compile();
}

void Subroutine::CompileReturn(uint16_t EndMagic)
{
    Node::Compile(EndMagic, NumEndParams);
}

void Program::Compile()
{
    for (auto i = Subroutines.begin(); i != Subroutines.end(); ++i)
        (*i)->Compile();
}

void Function::Compile()
{
    CompilePrototype(MAGIC_FUNCTION_BEGIN, Arguments.size() + 1);
    for (auto i = Arguments.begin(); i != Arguments.end(); ++i)
        (*i)->CompileRaw();
    Subroutine::Compile();
    CompileReturn(MAGIC_FUNCTION_END);
}

void Chapter::Compile()
{
    CompilePrototype(MAGIC_CHAPTER_DECLARATION, 1);
    Subroutine::Compile();
    CompileReturn(MAGIC_END_CHAPTER);
}

void Scene::Compile()
{
    CompilePrototype(MAGIC_SCENE_DECLARATION, 1);
    Subroutine::Compile();
    CompileReturn(MAGIC_END_SCENE);
}

void Assignment::Compile()
{
    Rhs.Compile();
    Node::Compile(Magic, 1);
    Name.CompileRaw();
    Node::Compile(MAGIC_CLEAR_PARAMS, 0);
}

void BinaryOperator::Compile()
{
    Lhs.Compile();
    Rhs.Compile();
    Node::Compile(Magic, 0);
}

void UnaryOperator::Compile()
{
    Rhs.Compile();
    Node::Compile(Magic, 0);
}

void Condition::Compile()
{
    Expr.Compile();
    Node::Compile(Magic, 0);
    ConditionBlock.Compile();
}

void Else::Compile()
{
    ElseBlock.Compile();
}

void Select::Compile()
{
}

void Case::Compile()
{
}

namespace Nss
{

void Compile(char* pBuffer, uint32_t Length, Buffer* NsbBuffer, Buffer* MapBuffer)
{
    Output = NsbBuffer;
    MapOutput = MapBuffer;
    YY_BUFFER_STATE buffer = yy_scan_bytes(pBuffer, Length);
    yyparse();
    pRoot->Compile();
    yy_delete_buffer(buffer);
}

}
