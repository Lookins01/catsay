#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/time.h>
#include <sys/select.h>


#define SEP_LINE 50


int len(char *str) {
    /*
      Take from StackOverflow :^)
      https://stackoverflow.com/questions/32523898/c-reading-non-ascii-characters 
    */
    int len = 0;
    int i = 0;
    int charSize = 0; // Size of the current char in byte

    if (!str) 
        return -1;

    while (str[i])
    {
        if (charSize == 0)
        {
            ++len;
            if (!(str[i] >> 7 & 1)) // ascii char
                charSize = 1;
            else if (!(str[i] >> 5 & 1))
                charSize = 2;
            else if (!(str[i] >> 4 & 1))
                charSize = 3;
            else if (!(str[i] >> 3 & 1))
                charSize = 4;
            else 
                return -1; // not supposed to happen
        }
        else if ((str[i] >> 6 & 3) != 2)
            return -1;
        --charSize;
        ++i;
    }
    return len;
}


int is_stdin_empty()
{
    // init stdin fd
    fd_set stdin_fd;
    FD_ZERO(&stdin_fd);
    FD_SET(0, &stdin_fd);
    // init variable for timeout
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    // make select with 0 timeout and get result
    int result = select(1, &stdin_fd, NULL, NULL, &timeout);
    return result;
}


void replace_new_line(char *line) 
{
    int i = 0;
    while (line[i]) 
    {
        if (line[i] == '\n') {
          line[i] = ' ';
        }
        i += 1;
    }
}


int main(int argc, char **argv) 
{
    int raw_output_len = 0;
    char *output;
    if (argc == 1) {
        int stdin_empty = !is_stdin_empty();  
        if (stdin_empty == 1) {
            // if stdin empty take to 10'000 symbols from user
            size_t output_cap = 10;
            size_t readed_line_size;
            output = malloc(output_cap * sizeof(char));
            readed_line_size = getline(&output, &output_cap, stdin);
            for (size_t i = 0; i < readed_line_size; i++) {
                if (output[i] == '\n') {
                    output[i] = ' ';
                }
            }

        } else {
            // else read all from stdin  
            char *cur_line; 
            size_t output_cap = 10;
            size_t output_actual_cap = 10;
            size_t readed_line_size = 0;
            size_t accum_cap = 0;
            cur_line = malloc(output_cap * sizeof(char));
            output = malloc(output_cap * sizeof(char));
            while (readed_line_size != (size_t) -1) {
                readed_line_size = getline(&cur_line, &output_cap, stdin);
                if (readed_line_size == (size_t) -1) {
                    break;
                }
                if (accum_cap + readed_line_size >= output_actual_cap) {
                    output_actual_cap += readed_line_size;
                    output = realloc(output, output_actual_cap * sizeof(char));
                    //accum_cap = 0;
                }
                accum_cap += readed_line_size;
                strcat(output, cur_line);
            }
        }   
    } else {
        for (int i = 1; i < argc; i++) {
            raw_output_len += strlen(argv[i]);
            if (i < argc - 1) raw_output_len += 1; 
        }
        output = malloc(raw_output_len * sizeof(char));
        
        for (int i = 1; i < argc; i++)  {
            strcat(output, argv[i]);
            if (i < argc - 1) strcat(output, " ");  
        }
    }
    
    replace_new_line(output); 
    int output_len = len(output);
    
    if (output_len == -1) {
        printf("ERROR: Can't read input text!\n");
        return -1;
    }
    
    int frame_lenght = output_len <= 42 ? output_len : SEP_LINE;
    char *frame = malloc((frame_lenght + 2) * sizeof(char));
    for (int i = 0; i < frame_lenght + 2; i++) {
        frame[i] = '-';
    }

    printf(" %s\n", frame); 
    
    if (output_len <= 42) 
    {
        printf("< %s >\n", output);
    }
    else 
    {
        int i = 0;
        int len = 0;
        int charSize = 0;
        while (output[i])
        { 
            if (len == 0 && charSize == 0) {
                  printf("| ");
              }
                
              printf("%c", output[i]);
                
              
            if (charSize == 0)
            {
                ++len;
                if (!(output[i] >> 7 & 1)) // ascii char
                    charSize = 1;
                else if (!(output[i] >> 5 & 1))
                    charSize = 2;
                else if (!(output[i] >> 4 & 1))
                    charSize = 3;
                else if (!(output[i] >> 3 & 1))
                    charSize = 4;
                else
                    return -1; // not supposed to happen
             }
             else if ((output[i] >> 6 & 3) != 2)
                return -1;
             --charSize;
             ++i;
             // ахахах лох 
             if (len == SEP_LINE && charSize == 0) 
              {
                  printf(" |\n");
                  len = 0;
              }
                
        }
        
        if (len != SEP_LINE) {
            for (int i = 0; i < SEP_LINE - len; i++) {
                printf(" ");
            }
            printf(" |\n");
        }
        
    }

   printf(" %s\n", frame); 

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
   
    free(frame);
    free(output);
    return 0; 
}
