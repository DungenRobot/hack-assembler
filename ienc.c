// ienc.c (Instruction ENCoder)
// Created by Anders Olsen

#include <ctype.h>
#include <string.h>
#include <stdio.h>


//The overall format is 


/* Takes in a cleaned A-instruction string
 * Returns the value of the integer for the A instruction
*/
int get_A_val(char* const cleanStr) {
    char* sel = cleanStr + 1;

    int val = 0;

    while (*sel != 0) {
        if (isdigit(*sel)) {
            val = (val * 10) + (*sel - 48);
        }
        else{ return -1; }
        sel++;
    }
    return val;
}

/* Takes in an integer and string pointer
 * Stores the binary representation of the integer as a string at the pointer
*/
void int_to_bin(int val, char* output) {
    output[0] = 0;
    int mask = 0b1000000000000000; //bitmask

    for (int i = 0; i < 16; i++) {
        char* i = (mask & val) ? "1" : "0"; //the character to append
        strncat(output, i, 1);
        mask = mask >> 1;
    }
}


//required for C command parsing but looks nicer if they are defined later
int C_dest(char* start, char* end);
int C_comp(char* start, char* end);
int C_jump(char* start);

/* Parses a C command into the appropriate bytes
 * Takes in a line and returns the integer value of the 
*/
int str_to_comp_val(char* const line_in){
    //get these pointers as handy references
    char* start_ptr = line_in;
    char* end_ptr = strchr(line_in, '\0');

    //find the characters that separate the different parts of the C command
    char* equals_sign = strchr(line_in, '=');
    char* semicolon   = strchr(line_in, ';');

    //if null, send to start of string
    char* comp_start = (equals_sign == NULL) ? line_in : equals_sign + 1;
    char* jump_start = (semicolon   == NULL) ? end_ptr : semicolon + 1;

    //fix any NULL pointers
    char* equals_or_start = (equals_sign == NULL) ? line_in : equals_sign;
    char* colon_or_end    = (semicolon   == NULL) ? end_ptr : semicolon;

    //setup our output
    int out = 0b111 << 13;
    out |= C_dest(line_in, equals_or_start) << 3;
    out |= C_comp(comp_start, colon_or_end) << 6;
    out |= C_jump(jump_start);
    return out;
}


int C_dest(char* start, char* end) {
    int out = 0;
    while (start != end) {
        if (*start == 'A') { out |= 0b100; }
        if (*start == 'D') { out |= 0b010; }
        if (*start == 'M') { out |= 0b001; }
        start++;
    }
    //shift over to place the bit correctly
    return out;
}

int C_comp(char* start, char* end) {
    //get our comparison as a separate string 'comp_str'
    int len = end - start;
    char comp_str[5] = {0};
    strncpy(comp_str, start, len);

    //exit early if we run into one of the following
    if (strcmp(comp_str,  "0") == 0) { return 0b101010; }
    if (strcmp(comp_str,  "1") == 0) { return 0b111111; }
    if (strcmp(comp_str, "-1") == 0) { return 0b111010; }

    //setup string and output for thorough check
    //When we get replace M with A, it allows us to simplify our problem
    int out = 0;
    char* M_ptr = strchr(comp_str, 'M');
    if (M_ptr != NULL) { *M_ptr = 'A'; out = 1 << 6; }

    
    //Easy to pick out. Helps in cases where the user switches A and D
    if (strchr(comp_str, '&') != NULL) { return out | 0b000000; }
    if (strchr(comp_str, '|') != NULL) { return out | 0b010101; }


    int has_A = (M_ptr != NULL) || (strchr(comp_str, 'A') != NULL);
    int has_D = (strchr(comp_str, 'D') != NULL);

    //the following can only be expressed one way, so we can check manually
    //breaking the checks up like this ensures that we aren't making unecessary comparisons
    if (has_A && !has_D)
    {
        if (strcmp(comp_str,   "A") == 0) {return out | 0b110000; }
        if (strcmp(comp_str,  "!A") == 0) {return out | 0b110001; }
        if (strcmp(comp_str,  "-A") == 0) {return out | 0b110011; }
        if (strcmp(comp_str, "A-1") == 0) {return out | 0b110010; }
        return out | 0b110111; //the only possibility left is A+1
    }
    if (has_D && !has_A)
    {
        if (strcmp(comp_str,   "D") == 0) {return out | 0b001100; }
        if (strcmp(comp_str,  "!D") == 0) {return out | 0b001101; }
        if (strcmp(comp_str,  "-D") == 0) {return out | 0b001111; }
        if (strcmp(comp_str, "D-1") == 0) {return out | 0b001110; }
        return out | 0b011111; //the only possibility left is D+1
    }
    else //has both
    {
        if (strcmp(comp_str, "D-A") == 0) {return out | 0b010011; }
        if (strcmp(comp_str, "A-D") == 0) {return out | 0b000111; }
        return out | 0b000010; //only possiblity left is D+A
    }
    //this solution does not account for invalid comparisons (such as 1-D) but that's not in my requirements so it's okay
}

int C_jump(char* start) {
    //this solution isn't 'optimized' but is fairly clean and I think works well
    if (strcmp(start, "JGT") == 0) {return 0b001 ;}
    if (strcmp(start, "JEQ") == 0) {return 0b010 ;}
    if (strcmp(start, "JGE") == 0) {return 0b011 ;}
    if (strcmp(start, "JLT") == 0) {return 0b100 ;}
    if (strcmp(start, "JNE") == 0) {return 0b101 ;}
    if (strcmp(start, "JLE") == 0) {return 0b110 ;}
    if (strcmp(start, "JMP") == 0) {return 0b111 ;}
    return 0b000; //if we lack jump bits,
}

