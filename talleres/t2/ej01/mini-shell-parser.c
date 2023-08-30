
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

char *strrev(char *str)
{
    char *end, *wrk = str;
    {
        if(str && *str)
        {
            end = str + strlen(str) - 1;
            while(end > wrk)
            {
                char temp;

                temp = *wrk;
                *wrk++ = *end;
                *end-- = temp;
            }
        }
    }
    return str;
}

void empty_white_spaces_at_prefix_and_suffix(char* token){
	while(isspace(*token)) token++;
	strrev(token);
	while(isspace(*token)) token++;
	strrev(token);
}

char** split_in_array(const char separator, char* program){
	char** local_parameters = malloc(sizeof(char*));
	char *token = strtok(program, &separator);
	int k = 0;
	while( token != NULL ) {
		empty_white_spaces_at_prefix_and_suffix(token);
		local_parameters[k] = token;
		token = strtok(NULL, &separator);
		k++;
		local_parameters = realloc(local_parameters, sizeof(char*) * (k+1));
	}
	local_parameters[k] = NULL;
	return local_parameters;
}

int count_in_array_split(char** program){
	int k=0;
	while( program[k] != NULL ) {
		k++;
	}
	return k;
}

char*** separate_parameters(char **programs, int programs_count){
	char*** parameters = malloc(sizeof(char**));
	for(int j=0; j < programs_count; j++){
		parameters[j] = split_in_array(' ', programs[j]);
		parameters = realloc(parameters, sizeof(char**) * (j+2));
	}
	return parameters;
}

char*** parse_input(char **argv, int* programs_count){
    char** programs = split_in_array('|', argv[1]);
	*programs_count = count_in_array_split(programs);
	char*** programs_with_parameters = separate_parameters(programs, *programs_count);
	free(programs);
    return programs_with_parameters;
}