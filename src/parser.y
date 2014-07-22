%{
    #include "nsscompiler.hpp"
    #include <cstdio>
    #include <cstdlib>

    Program* pRoot;
    extern int yylex();
    void yyerror(const char* s) { std::printf("Error: %s\n", s); std::abort(); }
%}

%union
{
    Node* node;
    Program* program;
    Block* block;
    Statement* stmt;
    Argument* arg;
    Expression* expr;
    Subroutine* subr;
    std::vector<Argument*>* argvec;
    std::vector<Expression*>* expvec;
    std::string* string;
    int token;
}

%token <string> TIDENTIFIER TFLOAT TINTEGER TXML TSTRING THEX
%token <token> TLPAREN TRPAREN TLBRACE TRBRACE TFUNCTION TSEMICOLON TEQUAL TCOMMA TQUOTE TCHAPTER TSCENE
%token <token> TADD TSUB TMUL TDIV TIF TWHILE TLESS TGREATER TEQUALEQUAL TNEQUAL TGEQUAL TLEQUAL TAND TOR TNOT
%token <token> TRETURN TCALLCHAPTER TCALLSCENE TELSE TSELECT TCASE TCOLON TAT TDOLLAR THASH

%type <program> start program
%type <arg> arg 
%type <argvec> func_args
%type <expvec> func_exps
%type <block> stmts block
%type <stmt> stmt call cond
%type <subr> func_decl
%type <expr> expr

%left TPLUS TMINUS
%left TMUL TDIV

%start start

%%

start : program { pRoot = $1; }
      ;

program : func_decl { $$ = new Program; $$->Subroutines.push_back($1); }
        | program func_decl { $1->Subroutines.push_back($2); }
        ;

stmts : stmt { $$ = new Block(); $$->Statements.push_back($<stmt>1); }
      | stmts stmt { $1->Statements.push_back($<stmt>2); }
      ;

stmt : call | cond
     | arg TEQUAL expr TSEMICOLON { $$ = new Assignment(*$<arg>1, *$3, MAGIC_ASSIGN); }
     | arg TADD TEQUAL expr TSEMICOLON { $$ = new Assignment(*$<arg>1, *$4, MAGIC_ADD_ASSIGN); }
     | arg TSUB TEQUAL expr TSEMICOLON { $$ = new Assignment(*$<arg>1, *$4, MAGIC_SUB_ASSIGN); }
     ;

block : TLBRACE stmts TRBRACE { $$ = $2; }
      | TLBRACE TRBRACE { $$ = new Block(); }
      ;

func_decl : TFUNCTION arg TLPAREN func_args TRPAREN block { $2->Data = string("function.") + $2->Data; $$ = new Function(*$2, *$4, *$6); delete $4; }
          | TCHAPTER arg block { $2->Data = string("chapter.") + $2->Data; $$ = new Chapter(*$2, *$3); }
          | TSCENE arg block { $2->Data = string("scene.") + $2->Data; $$ = new Scene(*$2, *$3); }
          ;

func_args : { $$ = new ArgumentList(); }
          | arg { $$ = new ArgumentList(); $$->push_back($<arg>1); }
          | func_args TCOMMA arg { $1->push_back($<arg>3); }
          ;

func_exps : { $$ = new ExpressionList(); }
          | expr { $$ = new ExpressionList(); $$->push_back($<expr>1); }
          | func_exps TCOMMA expr { $1->push_back($<expr>3); }
          ;

arg : TDOLLAR TIDENTIFIER { $$ = new Argument('$' + *$2, ARG_VARIABLE); delete $2; }
      | THASH TIDENTIFIER { $$ = new Argument('#' + *$2, ARG_VARIABLE); delete $2; }
      | THEX { $$ = new Argument(*$1, ARG_STRING); delete $1; }
      | TIDENTIFIER { $$ = new Argument(*$1, ARG_STRING); delete $1; }
      | TSTRING { $$ = new Argument($1->substr(1, $1->size() - 2), ARG_STRING); delete $1; }
      | TINTEGER { $$ = new Argument(*$1, ARG_INT); delete $1; }
      ;

call : arg TLPAREN func_exps TRPAREN TSEMICOLON { $$ = new CallStatement(*$1, *$3, MAGIC_CALL_FUNCTION); delete $3; }
     | TRETURN TSEMICOLON
          {
               ExpressionList Args;
               Argument* Arg = new Argument("Return", ARG_FUNCTION);
               $$ = new Call(*Arg, Args, 0);
          }
     | TXML {
               ExpressionList Args;
               Args.push_back(new Argument("TODO", ARG_STRING));
               Args.push_back(new Argument("TODO", ARG_STRING));
               Args.push_back(new Argument(*$1, ARG_STRING));
               Argument* Arg = new Argument("ParseText", ARG_FUNCTION);
               $$ = new Call(*Arg, Args, 0);
            }
     | TCALLCHAPTER TIDENTIFIER TSEMICOLON { $$ = MakeCall(*$2, MAGIC_CALL_CHAPTER); delete $2; }
     | TCALLCHAPTER TDOLLAR TIDENTIFIER TSEMICOLON { $$ = MakeCall('$' + *$3, MAGIC_CALL_CHAPTER); delete $3; }
     | TCALLSCENE TIDENTIFIER TSEMICOLON { $$ = MakeCall(*$2, MAGIC_CALL_SCENE); delete $2; }
     | TCALLSCENE TDOLLAR TIDENTIFIER TSEMICOLON { $$ = MakeCall('$' + *$3, MAGIC_CALL_SCENE); delete $3; }
     ;

expr : arg { $<arg>$ = $1; }
     | expr TMUL expr { $$ = new BinaryOperator(*$1, MAGIC_MULTIPLY, *$3); }
     | expr TDIV expr { $$ = new BinaryOperator(*$1, MAGIC_DIVIDE, *$3); }
     | expr TADD expr { $$ = new BinaryOperator(*$1, MAGIC_ADD, *$3); }
     | expr TSUB expr { $$ = new BinaryOperator(*$1, MAGIC_SUBSTRACT, *$3); }
     | expr TLESS expr { $$ = new BinaryOperator(*$1, MAGIC_LOGICAL_LESS, *$3); }
     | expr TGREATER expr { $$ = new BinaryOperator(*$1, MAGIC_LOGICAL_GREATER, *$3); }
     | expr TEQUALEQUAL expr { $$ = new BinaryOperator(*$1, MAGIC_LOGICAL_EQUAL, *$3); }
     | expr TNEQUAL expr { $$ = new BinaryOperator(*$1, MAGIC_LOGICAL_NOT_EQUAL, *$3); }
     | expr TGEQUAL expr { $$ = new BinaryOperator(*$1, MAGIC_LOGICAL_GREATER_EQUAL, *$3); }
     | expr TLEQUAL expr { $$ = new BinaryOperator(*$1, MAGIC_LOGICAL_LESS_EQUAL, *$3); }
     | expr TAND expr { $$ = new BinaryOperator(*$1, MAGIC_LOGICAL_AND, *$3); }
     | expr TOR expr { $$ = new BinaryOperator(*$1, MAGIC_LOGICAL_OR, *$3); }
     | TNOT expr { $$ = new UnaryOperator(MAGIC_LOGICAL_NOT, *$2); }
     | TSUB expr { $$ = new UnaryOperator(MAGIC_NEGATIVE, *$2); }
     | TAT expr { $$ = new UnaryOperator(MAGIC_PLACEHOLDER_PARAM, *$2); }
     | arg TLPAREN func_exps TRPAREN { $$ = new Call(*$1, *$3, MAGIC_CALL_FUNCTION); delete $3; }
     ;

cond : TIF TLPAREN expr TRPAREN block { $$ = new Condition(*$5, *$3, MAGIC_IF); }
     | TELSE TIF TLPAREN expr TRPAREN block { $$ = new Condition(*$6, *$4, MAGIC_IF); }
     | TELSE block { $$ = new Else(*$2); }
     | TSELECT block { $$ = new Select(*$2); }
     | TCASE TIDENTIFIER TCOLON block { $$ = new Case(*$2, *$4); delete $2; }
     | TWHILE TLPAREN expr TRPAREN block { $$ = new Condition(*$5, *$3, MAGIC_WHILE); }
     ;

%%
