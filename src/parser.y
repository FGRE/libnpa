%{
    #include "nsscompiler.hpp"
    #include <cstdio>
    #include <cstdlib>
    #include <boost/algorithm/string/replace.hpp>

    Call* MakeCall(string Name, uint16_t Magic);
    void WriteSymbol(const string& Symbol);

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
%token <token> TLPAREN TRPAREN TLBRACE TRBRACE TFUNCTION TSEMICOLON TEQUAL TCOMMA TQUOTE TCHAPTER TSCENE TINCLUDE
%token <token> TADD TSUB TMUL TDIV TIF TWHILE TLESS TGREATER TEQUALEQUAL TNEQUAL TGEQUAL TLEQUAL TAND TOR TNOT
%token <token> TRETURN TCALLCHAPTER TCALLSCENE TELSE TSELECT TCASE TCOLON TAT TDOLLAR THASH TBREAK TLABRACE TRABRACE

%type <program> start
%type <arg> arg array
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

start : { pRoot = new Program; } program { }
      ;

program : func_decl { pRoot->Subroutines.push_back($1); }
        | program func_decl { pRoot->Subroutines.push_back($2); }
        | include
        | program include
        ;

include : TINCLUDE TSTRING { WriteSymbol("include." + $2->substr(1, $2->size() - 2)); delete $2; }
        ;

stmts : stmt { $$ = new Block(); $$->Statements.push_back($<stmt>1); }
      | stmts stmt { $1->Statements.push_back($<stmt>2); }
      ;

stmt : call | cond
     | arg TEQUAL expr TSEMICOLON { $$ = new Assignment($1, $3, MAGIC_ASSIGN); }
     | arg TADD TEQUAL expr TSEMICOLON { $$ = new Assignment($1, $4, MAGIC_ADD_ASSIGN); }
     | arg TSUB TEQUAL expr TSEMICOLON { $$ = new Assignment($1, $4, MAGIC_SUB_ASSIGN); }
     | arg TADD TADD TSEMICOLON { $$ = new UnaryStatement(MAGIC_INCREMENT, $1); }
     | arg TSUB TSUB TSEMICOLON { $$ = new UnaryStatement(MAGIC_DECREMENT, $1); }
     | TIDENTIFIER TCOLON { $$ = new Label(*$1); delete $1; }
     ;

block : TLBRACE stmts TRBRACE { $$ = $2; }
      | TLBRACE TRBRACE { $$ = new Block(); }
      ;

func_decl : TFUNCTION arg TLPAREN func_args TRPAREN block { $2->Data = string("function.") + $2->Data; $$ = new Function($2, *$4, $6); delete $4; }
          | TCHAPTER arg block { $2->Data = string("chapter.") + $2->Data; $$ = new Chapter($2, $3); }
          | TSCENE arg block { $2->Data = string("scene.") + $2->Data; $$ = new Scene($2, $3); }
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
      | TSTRING { std::string str = $1->substr(1, $1->size() - 2); boost::replace_all(str, "\\n", "\n"); $$ = new Argument(str, ARG_STRING); delete $1; }
      | TINTEGER { $$ = new Argument(*$1, ARG_INT); delete $1; }
      | array { $$ = $1; }
      ;

array : TDOLLAR TIDENTIFIER TLABRACE arg TRABRACE { $$ = new Array('$' + *$2); ((Array*)$$)->Arguments.push_back($4); delete $2; }
      | THASH TIDENTIFIER TLABRACE arg TRABRACE { $$ = new Array('#' + *$2); ((Array*)$$)->Arguments.push_back($4); delete $2; }
      | array TLABRACE arg TRABRACE { $$ = $1; ((Array*)$$)->Arguments.push_back($3); }

call : arg TLPAREN func_exps TRPAREN TSEMICOLON { $$ = new CallStatement($1, *$3, MAGIC_CALL_FUNCTION); delete $3; }
     | TRETURN TSEMICOLON
          {
               ExpressionList Args;
               Argument* Arg = new Argument("Return", ARG_FUNCTION);
               $$ = new Call(Arg, Args, 0);
          }
     | TBREAK TSEMICOLON
          {
               ExpressionList Args;
               Argument* Arg = new Argument("Break", ARG_FUNCTION);
               $$ = new Call(Arg, Args, 0);
          }
     | TXML {
               ExpressionList Args;
               Args.push_back(new Argument("TODO", ARG_STRING));
               Args.push_back(new Argument("TODO", ARG_STRING));
               Args.push_back(new Argument(*$1, ARG_STRING));
               Argument* Arg = new Argument("ParseText", ARG_FUNCTION);
               $$ = new Call(Arg, Args, 0);
               delete $1;
            }
     | TCALLCHAPTER TIDENTIFIER TSEMICOLON { $$ = MakeCall(*$2, MAGIC_CALL_CHAPTER); delete $2; }
     | TCALLCHAPTER TDOLLAR TIDENTIFIER TSEMICOLON { $$ = MakeCall('$' + *$3, MAGIC_CALL_CHAPTER); delete $3; }
     | TCALLSCENE TIDENTIFIER TSEMICOLON { $$ = MakeCall(*$2, MAGIC_CALL_SCENE); delete $2; }
     | TCALLSCENE TDOLLAR TIDENTIFIER TSEMICOLON { $$ = MakeCall('$' + *$3, MAGIC_CALL_SCENE); delete $3; }
     ;

expr : arg { $<arg>$ = $1; }
     | expr TMUL expr { $$ = new BinaryOperator($1, MAGIC_MUL_EXPRESSION, $3); }
     | expr TDIV expr { $$ = new BinaryOperator($1, MAGIC_DIV_EXPRESSION, $3); }
     | expr TADD expr { $$ = new BinaryOperator($1, MAGIC_ADD_EXPRESSION, $3); }
     | expr TSUB expr { $$ = new BinaryOperator($1, MAGIC_SUB_EXPRESSION, $3); }
     | expr TLESS expr { $$ = new BinaryOperator($1, MAGIC_CMP_LESS, $3); }
     | expr TGREATER expr { $$ = new BinaryOperator($1, MAGIC_CMP_GREATER, $3); }
     | expr TEQUALEQUAL expr { $$ = new BinaryOperator($1, MAGIC_CMP_EQUAL, $3); }
     | expr TNEQUAL expr { $$ = new BinaryOperator($1, MAGIC_LOGICAL_NOT_EQUAL, $3); }
     | expr TGEQUAL expr { $$ = new BinaryOperator($1, MAGIC_LOGICAL_GREATER_EQUAL, $3); }
     | expr TLEQUAL expr { $$ = new BinaryOperator($1, MAGIC_LOGICAL_LESS_EQUAL, $3); }
     | expr TAND expr { $$ = new BinaryOperator($1, MAGIC_CMP_LOGICAL_AND, $3); }
     | expr TOR expr { $$ = new BinaryOperator($1, MAGIC_CMP_LOGICAL_OR, $3); }
     | TNOT expr { $$ = new UnaryOperator(MAGIC_LOGICAL_NOT, $2); }
     | TSUB expr { $$ = new UnaryOperator(MAGIC_NEGA_EXPRESSION, $2); }
     | TAT expr { $$ = new UnaryOperator(MAGIC_AT_EXPRESSION, $2); }
     | arg TLPAREN func_exps TRPAREN { $$ = new Call($1, *$3, MAGIC_CALL_FUNCTION); delete $3; }
     ;

cond : TIF TLPAREN expr TRPAREN block { $$ = new If($5, $3); }
     | TELSE TIF TLPAREN expr TRPAREN block { $$ = new If($6, $4); }
     | TELSE block { $$ = new Else($2); }
     | TSELECT block { $$ = new Select($2); }
     | TCASE TIDENTIFIER TCOLON block { $$ = new Case(*$2, $4); delete $2; }
     | TWHILE TLPAREN expr TRPAREN block { $$ = new While($5, $3); }
     ;

%%
