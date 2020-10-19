#ifndef PCRE2_CODE_UNIT_WIDTH
#define PCRE2_CODE_UNIT_WIDTH 8
#endif

#include <ncurses.h>
#include <locale.h>
#include <pcre2.h>
#include <stdio.h>
#include <string.h>

#define DX 3
#define ONLELINE 3

#ifndef PCRE2_UCP_FLAG
#define PCRE2_UCP_FLAG 0
#endif

#define MAX_STR_LENGTH 50

typedef struct Match {
	char str[MAX_STR_LENGTH];
    struct Match *next;
    int index;
    int width;
} Match;

typedef enum Status {
    Success,
    CompileError,
    NoMatchError,
    MatchError,
} Status;

Status try_find_match (char *patternStr, char *subjectStr, Match **match);
Match *find_next_match(int index, int width, char *buf);
void free_match (Match *match);

void main() {
	setlocale(LC_ALL, "");
	initscr();
	cbreak();
	printw("Input:");
	refresh();

	int middle = (COLS-2*DX)/2;
	WINDOW *winPattern, *winSubject, *winReport;
	char patternStr[MAX_STR_LENGTH], subjectStr[MAX_STR_LENGTH];

	winPattern = newwin(ONLELINE, middle, DX, DX);
	keypad(winPattern, TRUE);
	box(winPattern, 0, 0);
	wrefresh(winPattern);

	winSubject = newwin(ONLELINE, middle, DX, DX+middle);
	keypad(winSubject, TRUE);
	box(winSubject, 0, 0);
	wrefresh(winSubject);

	winReport = newwin(LINES-ONLELINE-DX*2, (COLS-2*DX), DX+ONLELINE, DX);
	scrollok (winReport, TRUE);
	wmove(winReport, 1, 0);
	box(winReport, 0, 0);
	wrefresh(winReport);

	mvwgetnstr(winPattern, 1, 1, patternStr, MAX_STR_LENGTH);
	while(*patternStr) {
		mvwgetnstr(winSubject, 1, 1, subjectStr, MAX_STR_LENGTH);

		wprintw(winReport, "  `%s`: `%s`\n", patternStr, subjectStr);
		Match *match = NULL;
		int status = try_find_match(patternStr, subjectStr, &match);
		switch(status) {
			case Success:
				for (Match *c = match; c != NULL; c=c->next) {
					wprintw(winReport, "  %d: %.*s\n", c->index, c->width, c->str);
				}
				break;
			case CompileError:
				wprintw(winReport, "  Compile error\n\n");
				break;
			case NoMatchError:
				wprintw(winReport, "  No matches\n\n");
				break;
			case MatchError:
				wprintw(winReport, "  Matching error\n\n");
				break;
		}

		free_match(match);
		wprintw(winReport, "\n");
		box(winReport, 0, 0);
		wrefresh(winReport);

		werase(winPattern);
		werase(winSubject);
		box(winPattern, 0, 0);
		box(winSubject, 0, 0);
		wrefresh(winPattern);
		wrefresh(winSubject);

		mvwgetnstr(winPattern, 1, 1, patternStr, MAX_STR_LENGTH);
	}

	endwin();
}

Status try_find_match (char *patternStr, char *subjectStr, Match **match) {
    pcre2_code *re;
    PCRE2_SPTR pattern;     /* PCRE2_SPTR is a pointer to unsigned code units of */
    PCRE2_SPTR subject;     /* the appropriate width (in this case, 8 bits). */

    int errnum;
    int i, rc;

    PCRE2_SIZE erroffs;
    PCRE2_SIZE *ovector;
    PCRE2_SIZE subject_length;

    pcre2_match_data *match_data;

    pattern = (PCRE2_SPTR)patternStr;
    subject = (PCRE2_SPTR)subjectStr;
    subject_length = (PCRE2_SIZE)strlen((char *)subject);

    if (PCRE2_UCP_FLAG == 1) {
        re = pcre2_compile(pattern, PCRE2_ZERO_TERMINATED, PCRE2_UCP, &errnum, &erroffs, NULL);
    } else {
        re = pcre2_compile(pattern, PCRE2_ZERO_TERMINATED, 0, &errnum, &erroffs, NULL);
    }

    if (re == NULL) {
        PCRE2_UCHAR buffer[256];
        pcre2_get_error_message(errnum, buffer, sizeof(buffer));
        return CompileError;
    }

    match_data = pcre2_match_data_create_from_pattern(re, NULL);

    rc = pcre2_match(re, subject, subject_length, 0, 0, match_data, NULL);

    if (rc < 0) {
        Status status;
        switch(rc) {
        case PCRE2_ERROR_NOMATCH:
            status = NoMatchError;
            break;
        default:
            status = MatchError;
            break;
        }
        pcre2_match_data_free(match_data);   /* Release memory used for the match */
        pcre2_code_free(re);                 /*   data and the compiled pattern. */
        return status;
    }

    ovector = pcre2_get_ovector_pointer(match_data);
    Match *curr = NULL;
    for (i = 0; i < rc; i++) {
        char buf[MAX_STR_LENGTH] = {0};
        snprintf(buf, sizeof(buf), "%s", subject + ovector[2*i]);
        int width = (int)(ovector[2*i+1] - ovector[2*i]);
        if (curr == NULL) {
            *match = find_next_match(ovector[2*i], width, buf);
            curr = *match;
        } else {
            curr->next = find_next_match(ovector[2*i], width, buf);
            curr = curr->next;
        }
    }

    pcre2_match_data_free(match_data);  /* Release the memory that was used */
    pcre2_code_free(re);                /* for the match data and the pattern. */

    return Success;
}

Match *find_next_match(int index, int width, char *buf) {
    Match *match = calloc(1, sizeof(Match));
    match->next = NULL;
    match->index = index;
    match->width = width;
    memcpy(match->str, buf, MAX_STR_LENGTH);
    
    return match;
}

void free_match (Match *match) {
    Match *cur = match;
    while (cur != NULL) {
        Match *tmp = cur->next;
        free(cur);
        cur = tmp;
    }
}