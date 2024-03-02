/* main.c 
 * Created by Anders Olsen
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


#include "strop.c"
#include "ienc.c"


#define ANSI_DARK_WHITE "\x1b[90m"
#define ANSI_CYAN "\x1b[96m"
#define ANSI_RESET "\x1b[0m"





int main(int argc, char** argv) {

    //argument checking (with some fancy color codes)
    if (argc != 3)
    {
        puts( ANSI_DARK_WHITE "Expected arguments in the format: " ANSI_RESET "assembler <input file> [output filename]");
        return 0;
    }

    //setup file input
    char* input_filename = argv[1];
    char* output_filename = argv[2];
    FILE* fptr;
    fptr = fopen(input_filename, "r");

    //map for 
    map Lmap = createMap(2000);

    insertKey(Lmap, "R0", "0");
    insertKey(Lmap, "R1", "1");
    insertKey(Lmap, "R2", "2");
    insertKey(Lmap, "R3", "3");
    insertKey(Lmap, "R4", "4");
    insertKey(Lmap, "R5", "5");
    insertKey(Lmap, "R6", "6");
    insertKey(Lmap, "R7", "7");
    insertKey(Lmap, "R8", "8");
    insertKey(Lmap, "R9", "9");
    insertKey(Lmap, "R10", "10");
    insertKey(Lmap, "R11", "11");
    insertKey(Lmap, "R12", "12");
    insertKey(Lmap, "R13", "13");
    insertKey(Lmap, "R14", "14");
    insertKey(Lmap, "R15", "15");

    insertKey(Lmap, "SP", "0");
    insertKey(Lmap, "LCL", "1");
    insertKey(Lmap, "ARG", "2");
    insertKey(Lmap, "THIS", "3");
    insertKey(Lmap, "THAT", "4");

    insertKey(Lmap, "SCREEN", "16384");
    insertKey(Lmap, "KBD", "24576");


    char myString[100];
    char cleanString[100];

    int line_num = 0;

    while (fgets(myString, 100, fptr)) {
        bool line_is_blank = !rm_space_and_comm(myString, cleanString);
        if (line_is_blank){ continue; }

        bool is_L_command = (cleanString[0] == '(');

        if (is_L_command) {
            *(strchr(cleanString, 0) - 1) = 0;
            char* l_name = cleanString + 1;
            get_set_map(&Lmap, l_name, line_num);
        }
        else {line_num++; }
    }

    rewind(fptr);

    //setup file out
    FILE* outptr;
    outptr = fopen(output_filename, "w");

    //setup arrays used for loop
    char instruction[18];
    instruction[0] = 0;
    int var_space = 16;

    while (fgets(myString, 100, fptr)) {

        //clean line
        bool line_is_blank = !rm_space_and_comm(myString, cleanString);
        if (line_is_blank){ continue; }
        
        //check for A and L commands
        bool is_A_command =     (cleanString[0] == '@');
        bool is_L_command =     (cleanString[0] == '(');

        int val;

        //use two variables to place commands
        if (is_A_command) {
            val = get_A_val(cleanString);
            if (val == -1) {
                char* keyword = cleanString + 1;
                val = get_set_map(&Lmap, keyword, var_space);
                if (val == -1) { val=var_space; var_space++;}
            }
        }
        else if (is_L_command) {
            continue;
        }
        else {
            val = str_to_comp_val(cleanString);
        }

        int_to_bin(val, instruction);
        fputs(instruction, outptr);
        putc('\n', outptr);
    }

    //wrap things up
    fclose(fptr);
    fclose(outptr);
    freeMap(Lmap);

    return 0;
}