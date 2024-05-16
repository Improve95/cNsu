#include <assert.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define LENGTH      64
#define WLENGTH     (LENGTH / 8 * 3)
#define ALPHABET    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"\
                    "abcdefghijklmnopqrstuvwxyz"\
                    "0123456789"\
                    "+/"
#define ENDLINE     "=\n"

enum MAIN_ERRORS {
    MAIN_OK,
    MAIN_INVALID_COMMAND_LINE_ARGUMENTS,
    MAIN_ALLOCATION_FAILED,
    MAIN_INVALID_INPUT,
};

typedef struct
{
    char*       str;
    wchar_t*    wstr;
} Text;

size_t my_strcspn(const char* src, char c)
{
    if (0 == c) return 0;
    char str[2] = {c};

    return strcspn(src, str);
}

void foo(const char* restrict src, wchar_t* restrict dest, size_t pos)
{
    assert(NULL != src);
    assert(NULL != dest);

    static char alphabet[] = ALPHABET;

    const int diff = pos / 3 * 8;

    switch(pos % 3)
    {
        case 0:
            dest[pos] =
                    my_strcspn(alphabet, src[diff]) << 10 |
                    my_strcspn(alphabet, src[diff + 1]) << 4 |
                    my_strcspn(alphabet, src[diff + 2]) >> 2;
            break;
        case 1:
            dest[pos] =
                    (my_strcspn(alphabet, src[diff + 2]) & 0x3) << 14 |
                    my_strcspn(alphabet, src[diff + 3]) << 8 |
                    my_strcspn(alphabet, src[diff + 4]) << 2 |
                    my_strcspn(alphabet, src[diff + 5]) >> 4;
            break;
        case 2:
            dest[pos] =
                    (my_strcspn(alphabet, src[diff + 5]) & 0xf) << 12 |
                    my_strcspn(alphabet, src[diff + 6]) << 6 |
                    my_strcspn(alphabet, src[diff + 7]);
            break;
        default:
            assert(false);
            break;
    }
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    int error = MAIN_OK;
    Text text = {
            .str = NULL,
            .wstr = NULL
    };

    if (1 < argc)
    {
        error = MAIN_INVALID_COMMAND_LINE_ARGUMENTS;
        goto ERROR_HANDLER;
    }

    text.str = (char*)calloc(LENGTH + 1, sizeof(*text.str));
    text.wstr = (wchar_t*)calloc(WLENGTH + 1, sizeof(*text.wstr));
    if (NULL == text.str || NULL == text.wstr)
    {
        error = MAIN_ALLOCATION_FAILED;
        goto ERROR_HANDLER;
    }

    size_t len = 0;
    size_t len_without_endlines = 0;

    if (NULL == fgets(text.str, LENGTH + 1, stdin)
        || (len = strlen(text.str)) != strspn(text.str, ALPHABET ENDLINE)
        || (len + 7) % 8
        || len - (len_without_endlines = strcspn(text.str, ENDLINE)) > 8)
    {
        error = MAIN_INVALID_INPUT;
        goto ERROR_HANDLER;
    }
    len = strcspn(text.str, ENDLINE);
    memset(text.str + len, 0, LENGTH + 1 - len);

    const size_t len_new = (len * 3 + 7) / 8;

    for (size_t i = 0; i < len_new; i++)
    {
        foo(text.str, text.wstr, i);
    }

    ERROR_HANDLER:
    switch(error)
    {
        case MAIN_OK:
            fputws(text.wstr, stdout);
            fputwc(L'\n', stdout);
            break;
        case MAIN_INVALID_INPUT:
            printf("Wrong. Try again.\n");
            break;
        default:
            printf("Error code is %d\n", error);
            break;
    }
    free(text.str);
    free(text.wstr);

    return error;
}
