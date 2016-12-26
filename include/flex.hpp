#ifndef FLEX_HPP
#define FLEX_HPP

typedef struct yy_buffer_state* YY_BUFFER_STATE;
extern int yyparse();
#if ((FLEX_VERSION_MAJOR == 2 && FLEX_VERSION_MINOR == 6 && FLEX_VERSION_PATCH == 0) || \
    (FLEX_VERSION_MAJOR == 2 && FLEX_VERSION_MINOR == 5 && FLEX_VERSION_PATCH > 35))
extern YY_BUFFER_STATE yy_scan_bytes(const char* bytes, size_t len);
#else
extern YY_BUFFER_STATE yy_scan_bytes(const char* bytes, int len);
#endif
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

#endif
