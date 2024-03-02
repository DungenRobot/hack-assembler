/* strop.c (short for STRing OPerations)
 * Created by Anders Olsen
*/


#include <stdbool.h>
#include <ctype.h>
#include "map.h"
#include "map.c"


/* Takes in an input string.
 * 
 * Fills output_str with a copy of input_str that does not include any spaces or comments.
 * 
 * @returns int - 
 * 0 if string is just whitespace and comments, 1 if there are any instructions
 */
int rm_space_and_comm(char* const input_str, char* output_str)
{
    char* sel = input_str;

    char* start_of_out = output_str;

    while (*sel != 0)
    {
        if (!isspace(*sel))
        {
            //stop copying if the current and next character are both '/'
            bool is_comment = (*sel == '/') & (*sel == *(sel + 1));
            if (is_comment) { break; }

            *output_str = *sel;
            output_str++;
        }
        sel++;
    }

    *output_str = 0; //add our null terminator
    
    //return whether we have an empty string or not
    return (start_of_out != output_str);
}

/* Takes in a string. Returns the map index of the string.
 * If the string is not a key in the map, add it as a key and return it's index
 * All indexes are added to 16 to offset them properly.
 */


int get_set_map(map* table, char* key_name, int val) {
    int index = containsKey(*table, key_name);
    if (index != -1) {
        char* s = lookupIndex(*table, index);
        char* end = strchr(s, '\0');
        return (int) strtol(s, &end, 10);
    }

    char str_val[100];

    sprintf(str_val, "%d", val); //thank you coulton
    insertKey(*table, key_name, str_val);
    return -1;
}

