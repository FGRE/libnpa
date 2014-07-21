%{
#include <string>
#include "nsscompiler.hpp"
#include "parser.hpp"
#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)
extern "C" int yywrap() { }
%}

%%

"/*"((\*+[^/*])|([^*]))*\**"*/" ;
"<PRE"(.|[ \t\v\n])*"</PRE>" SAVE_TOKEN; return TXML;
[ \t\n]                 ;
"call_chapter"          return TOKEN(TCALLCHAPTER);
"call_scene"            return TOKEN(TCALLSCENE);
"function"              return TOKEN(TFUNCTION);
"chapter"               return TOKEN(TCHAPTER);
"scene"                 return TOKEN(TSCENE);
"if"                    return TOKEN(TIF);
"else"                  return TOKEN(TELSE);
"while"                 return TOKEN(TWHILE);
"select"                return TOKEN(TSELECT);
"case"                  return TOKEN(TCASE);
"return"                return TOKEN(TRETURN);
"\""                    return TOKEN(TQUOTE);
[a-zA-Zａ-ｚＡ-Ｚあ-ゖァ-ヺ一-龯_][/.a-zA-Zａ-ｚＡ-Ｚあ-ゖァ-ヺ一-龯０-９0-9_]*  SAVE_TOKEN; return TIDENTIFIER;
\"([^\\\"]|\\.)*\"      SAVE_TOKEN; return TSTRING;
[0-9]+\.[0-9]*          SAVE_TOKEN; return TFLOAT;
[0-9]+                  SAVE_TOKEN; return TINTEGER;
","                     return TOKEN(TCOMMA);
";"                     return TOKEN(TSEMICOLON);
":"                     return TOKEN(TCOLON);
[$#]                    SAVE_TOKEN; return TDOLLAR;
"="                     return TOKEN(TEQUAL);
"("                     return TOKEN(TLPAREN);
")"                     return TOKEN(TRPAREN);
"{"                     return TOKEN(TLBRACE);
"}"                     return TOKEN(TRBRACE);
"+"                     return TOKEN(TADD);
"-"                     return TOKEN(TSUB);
"/"                     return TOKEN(TDIV);
"*"                     return TOKEN(TMUL);
"<"                     return TOKEN(TLESS);
">"                     return TOKEN(TGREATER);
"=="                    return TOKEN(TEQUALEQUAL);
"!="                    return TOKEN(TNEQUAL);
">="                    return TOKEN(TGEQUAL);
"<="                    return TOKEN(TLEQUAL);
"&&"                    return TOKEN(TAND);
"||"                    return TOKEN(TOR);
"!"                     return TOKEN(TNOT);
"#include".*
"//".*
".".*
.                       printf("Unknown token!\n"); yyterminate();

%%
