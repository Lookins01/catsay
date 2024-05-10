#define _XOPEN_SOURCE
#include <wchar.h>

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>


#define RAW_OUTPUT_CAP (1024*1024*8)
#define BUFFER_CAP (1024*1024)

#define MAX_LINE_LEN 50


int min(int a, int b) {
    return a < b ? a : b;
}


int main(int argc, char **argv)
{
    setlocale(LC_ALL, "");

    size_t actual_size = 0;
    size_t allocated_size = RAW_OUTPUT_CAP;
    wchar_t *raw_output = malloc((RAW_OUTPUT_CAP + 1)*sizeof(wchar_t));
    if (raw_output == NULL) {
        fprintf(stderr, "ERROR: Buy more RAM!!!\n");
        return 1;
    }
    memset(raw_output, 0, RAW_OUTPUT_CAP*sizeof(wchar_t));

    if (argc == 1) {
        wchar_t buffer[BUFFER_CAP];
        while (fgetws(buffer, BUFFER_CAP, stdin) != NULL) {
            actual_size += wcslen(buffer);
            if (allocated_size < actual_size) {
                raw_output = realloc(raw_output, (actual_size + RAW_OUTPUT_CAP + 1) * sizeof(wchar_t));
                allocated_size = actual_size + RAW_OUTPUT_CAP;
                if (raw_output == NULL) {
                    fprintf(stderr, "ERROR: Buy more RAM!!!\n");
                    return 1;
                }
            }
            raw_output = wcscat(raw_output, buffer);
        }
    } else {
        for (int i = 1; i < argc; ++i) {
            size_t arg_len = mbstowcs(NULL, argv[i], 0);
            actual_size += arg_len + 1;
            if (allocated_size < actual_size) {
                raw_output = realloc(raw_output, (actual_size + RAW_OUTPUT_CAP + 1) * sizeof(wchar_t));
                allocated_size = actual_size + RAW_OUTPUT_CAP;
                if (raw_output == NULL) {
                    fprintf(stderr, "ERROR: Buy more RAM!!!\n");
                    return 1;
                }
            }
            wchar_t *temp = malloc((arg_len + 1) * sizeof(wchar_t));
            if (mbstowcs(temp, argv[i], arg_len + 1) == (size_t) -1) {
                fprintf(stderr, "ERROR: failed convert!\n");
                return 1; 
            }
            raw_output = wcscat(raw_output, temp);
            if (i < argc - 1) raw_output = wcscat(raw_output, L" ");
        }
    } 
    
    for (size_t i = 0; i < wcslen(raw_output); i++) {
        if (raw_output[i] == L'\n') raw_output[i] = L' '; 
    } 
    
    size_t frame_len = min(MAX_LINE_LEN, wcslen(raw_output)) + 2; 

    printf("/"); 
    for (size_t i = 0; i < frame_len; i++) printf("-");
    printf("\\\n");

    size_t lines = wcslen(raw_output) / MAX_LINE_LEN + 1;
    for (size_t line = 0; line < lines; ++line) {
        size_t offset = line * MAX_LINE_LEN;
        size_t end = offset + MAX_LINE_LEN; 
        if (end > wcslen(raw_output)) end = wcslen(raw_output);

        wchar_t buffer[MAX_LINE_LEN+1];
        memset(buffer, 0, sizeof(wchar_t)*(MAX_LINE_LEN+1));
        memcpy(buffer, raw_output+offset, (end-offset)*sizeof(wchar_t));
        printf("| %ls%-*s |\n", buffer, (int)(frame_len - 2 - wcslen(buffer)), "");
    }
    printf("\\"); 
    for (size_t i = 0; i < frame_len; i++) printf("-");
    printf("/\n");

    printf("  \\\n");
    printf("   \\    ,_     _             \n");
    printf("    \\   |\\\\_,-~/           \n");
    printf("        / _  _ |    ,--.     \n");
    printf("       (  @  @ )   / ,-'     \n");
    printf("        \\  _T_/-._( (       \n");
    printf("        /         `. \\      \n");
    printf("       |         _  \\ |     \n");
    printf("        \\ \\ ,  /      |    \n");
    printf("         || |-_\\__   /      \n");
    printf("        ((_/`(____,-'        \n");

    free(raw_output);
    return 0;
}
