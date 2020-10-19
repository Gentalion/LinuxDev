#define MAXSTR 80

typedef struct RegexMatch {
    struct RegexMatch *next;
    int index;
    int width;
    char buf[MAXSTR];
} RegexMatch;

typedef enum RegexMatchStatus {
    RegexMatchSuccess,
    RegexCompileError,
    RegexNoMatchError,
    RegexMatchError,
} RegexMatchStatus;

RegexMatch *regex_new_match(int index, int width, char *buf);
void regex_free_match(RegexMatch *match);
RegexMatchStatus regex_match(char *regex, char *str, RegexMatch **match);