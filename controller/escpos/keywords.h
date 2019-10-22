
#ifndef KEYWORD

int do_wait(char *arg);
int do_key(char *arg);
int do_text(char *arg);
int do_null(char *arg);
int do_string(char *arg);

#define __MAKE_KEYWORD_ENUM__
#define KEYWORD(symbol, flags, func) K_##symbol,
enum {
    K_UNKNOWN,
#endif
    KEYWORD(ESC,  27,  do_key)
    KEYWORD(FS,   28,  do_key)
    KEYWORD(GS,   29,  do_key)
    KEYWORD(DLE,  16,  do_key)
    KEYWORD(EOT,  4,   do_key)
    KEYWORD(ENQ,  5,   do_key)
    KEYWORD(SP,   32,  do_key)
    KEYWORD(HT,   9,   do_key)
    KEYWORD(LF,   10,  do_key)
    KEYWORD(CR,   13,  do_key)
    KEYWORD(FF,   12,  do_key)
    KEYWORD(CAN,  24,  do_key)
    KEYWORD(STAR, -1,  do_wait)
    KEYWORD(EXC,  -1,  do_null)
    KEYWORD(DOT,  -1,  do_null)
    KEYWORD(DC1,  17,  do_key)
    KEYWORD(DC2,  18,  do_key)
    KEYWORD(DC3,  19,  do_key)
    KEYWORD(DC4,  20,  do_key)

#ifdef __MAKE_KEYWORD_ENUM__
    KEYWORD_COUNT,
};
#undef __MAKE_KEYWORD_ENUM__
#undef KEYWORD
#endif

