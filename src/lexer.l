%{
#include <string>
#include "nsscompiler.hpp"
#include "parser.hpp"
#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)
%}

%option noyywrap

%%

"/*"((\*+[^/*])|([^*]))*\**"*/" ;
"<PRE"(.|[ \t\v\n])*"</PRE>" SAVE_TOKEN; return TXML;
[ \t\n\r]               ;
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
"break"                 return TOKEN(TBREAK);
"\""                    return TOKEN(TQUOTE);
[a-zA-Zａ-ｚＡ-Ｚあ-ゖァ-ヺ一-龯_][/.a-zA-Zａ-ｚＡ-Ｚあ-ゖァ-ヺ一-龯０-９0-9_]*  SAVE_TOKEN; return TIDENTIFIER;
\"([^\\\"]|\\.)*\"      SAVE_TOKEN; return TSTRING;
[0-9]+\.[0-9]*          SAVE_TOKEN; return TFLOAT;
[0-9]+                  SAVE_TOKEN; return TINTEGER;
"#"[0-9A-Fa-f]{6}       SAVE_TOKEN; return THEX;
","                     return TOKEN(TCOMMA);
";"                     return TOKEN(TSEMICOLON);
":"                     return TOKEN(TCOLON);
"$"                     return TOKEN(TDOLLAR);
"#"                     return TOKEN(THASH);
"="                     return TOKEN(TEQUAL);
"("                     return TOKEN(TLPAREN);
")"                     return TOKEN(TRPAREN);
"{"                     return TOKEN(TLBRACE);
"}"                     return TOKEN(TRBRACE);
"["                     return TOKEN(TLABRACE);
"]"                     return TOKEN(TRABRACE);
"+"                     return TOKEN(TADD);
"-"                     return TOKEN(TSUB);
"/"                     return TOKEN(TDIV);
"%"                     return TOKEN(TMOD);
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
"@"                     return TOKEN(TAT);
"#include"              return TOKEN(TINCLUDE);
"BezierMove".*
"//".*
".".*
.                       printf("Unknown token!\n"); yyterminate();

%%
