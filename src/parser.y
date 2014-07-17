%{
    #include "nsscompiler.hpp"
    #include <cstdio>
    #include <cstdlib>

    Block* pRoot;
    extern int yylex();
    void yyerror(const char* s) { std::printf("Error: %s\n", s); std::exit(1); }
%}

%union
{
    Node* node;
    Block* block;
    Statement* stmt;
    Argument* arg;
    Expression* expr;
    std::vector<Argument*>* argvec;
    std::string* string;
    int token;
}

%token <string> TIDENTIFIER TFLOAT TINTEGER TXML TDOLLAR
%token <token> TLPAREN TRPAREN TLBRACE TRBRACE TFUNCTION TSEMICOLON TEQUAL TCOMMA TQUOTE TCHAPTER TSCENE
%token <token> TADD TSUB TMUL TDIV TIF TWHILE TLESS TGREATER TEQUALEQUAL TNEQUAL TGEQUAL TLEQUAL TAND TOR TNOT
%token <token> TRETURN TCALLCHAPTER TCALLSCENE

%type <arg> arg 
%type <argvec> func_args
%type <block> program stmts block
%type <stmt> stmt func_decl call cond
%type <expr> expr

%left TPLUS TMINUS
%left TMUL TDIV

%start program

%%

program : stmts { pRoot = $1; }
        ;

stmts : stmt { $$ = new Block(); $$->Statements.push_back($<stmt>1); }
      | stmts stmt { $1->Statements.push_back($<stmt>2); }
      ;

stmt : func_decl | call | expr | cond
     ;

block : TLBRACE stmts TRBRACE { $$ = $2; }
      | TLBRACE TRBRACE { $$ = new Block(); }
      ;

func_decl : TFUNCTION arg TLPAREN func_args TRPAREN block { $2->Data = string("function.") + $2->Data; $$ = new Function(*$2, *$4, *$6); delete $4; }
          | TCHAPTER arg block { $2->Data = string("chapter.") + $2->Data; $$ = new Chapter(*$2, *$3); delete $3; }
          | TSCENE arg block { $2->Data = string("scene.") + $2->Data; $$ = new Scene(*$2, *$3); delete $3; }
          ;

func_args : { $$ = new ArgumentList(); }
          | arg { $$ = new ArgumentList(); $$->push_back($<arg>1); }
          | func_args TCOMMA arg { $1->push_back($<arg>3); }
          ;

arg : TDOLLAR TIDENTIFIER { $$ = new Argument(*$1 + *$2, ARG_VARIABLE); delete $1; delete $2; }
      | TIDENTIFIER { $$ = new Argument(*$1, ARG_STRING); delete $1; }
      | TQUOTE TIDENTIFIER TQUOTE { $$ = new Argument(*$2, ARG_STRING); delete $2; }
      | TINTEGER { $$ = new Argument(*$1, ARG_INT); delete $1; }
      ;

call : arg TLPAREN func_args TRPAREN TSEMICOLON { $$ = new Call(*$1, *$3, MAGIC_CALL_FUNCTION); delete $3; }
     | TRETURN TSEMICOLON
          {
               ArgumentList Args;
               Argument* Arg = new Argument("Return", ARG_FUNCTION);
               $$ = new Call(*Arg, Args, 0);
          }
     | TXML {
               ArgumentList Args;
               Args.push_back(new Argument("TODO", ARG_STRING));
               Args.push_back(new Argument("TODO", ARG_STRING));
               Args.push_back(new Argument(*$1, ARG_STRING));
               Argument* Arg = new Argument("ParseText", ARG_FUNCTION);
               $$ = new Call(*Arg, Args, 0);
            }
     | TCALLCHAPTER TIDENTIFIER TSEMICOLON { $$ = MakeCall(*$2, MAGIC_CALL_CHAPTER); delete $2; }
     | TCALLCHAPTER TDOLLAR TIDENTIFIER TSEMICOLON { $$ = MakeCall(*$2 + *$3, MAGIC_CALL_CHAPTER); delete $2; delete $3; }
     | TCALLSCENE TIDENTIFIER TSEMICOLON { $$ = MakeCall(*$2, MAGIC_CALL_SCENE); delete $2; }
     | TCALLSCENE TDOLLAR TIDENTIFIER TSEMICOLON { $$ = MakeCall(*$2 + *$3, MAGIC_CALL_SCENE); delete $2; delete $3; }
     ;

expr : arg TEQUAL expr TSEMICOLON { $$ = new Assignment(*$<arg>1, *$3); }
     | arg { $<arg>$ = $1; }
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
     ;

cond : TIF TLPAREN expr TRPAREN block { $$ = new Condition(*$5, *$3, MAGIC_IF); }
     | TWHILE TLPAREN expr TRPAREN block { $$ = new Condition(*$5, *$3, MAGIC_WHILE); }
     ;

%%
