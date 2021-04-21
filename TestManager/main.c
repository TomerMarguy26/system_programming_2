/*
Author: Tomer Marguy...........id_number: 205705874
Project name: "TestManager"
*/

#include "TestManagerIncludesAndDefines.h"
#include "TestManagerFunctions.h"

/*
Description: This is the main function of TestManager. This is the first code to run.
			 argv[1] will be the <Grades Directory>.
			 The final file with the grades per student ID will appear in <Grades Directory> folder.
Parameters:  int argc - the number of strings pointed to by argv.
			 char *argv[] - an array of strings representing the individual arguments provided on the command line.
Returns:	 exit program with 0 if everything went ok.
*/

int main(int argc, char *argv[]) {
	int test_grade_exit_code = -1; //the exit code will be set to 0 if the process creation succeded, -1 else
	char id[ID_LENGTH] = "", pass_line[2] = ""; //id length is always 9, pass_line is for skipping '\n' in while loop
	char *path=NULL, *path_cpy=NULL, *argv_str=NULL;//path is the given argument, path_cpy is the copy that will be processed
	int capacity = PATH_LENGTH + strlen(argv[1]); //this is the size of the memory block (in bytes) when using malloc

	/* memory allocating */

	path = (char*)malloc(capacity);
	if (NULL == path){
		printf("Memory allocation has failed!");
		return(ERROR_CODE);
		}
	path_cpy = (char*)malloc(capacity);
	if (NULL == path) {
			printf("Memory allocation has failed!");
			return(ERROR_CODE);
		}
	argv_str = (char*)malloc(capacity);
	if (NULL == path) {
			printf("Memory allocation has failed!");
			return(ERROR_CODE);
		}

	strcpy(argv_str, argv[1]);
	strcpy(path, argv_str);
	strcpy(path_cpy, path);
	strcat(path, "/studentids.txt"); //creates the path to the Ids file

	/* open students Ids file and loop on it */

	FILE *fp1 = NULL; //fp1 is the pointer to the student Ids .txt file
	if (NULL == (fp1 = fopen(path, "r"))) {
		printf("Unable to open file!");
		return(ERROR_CODE);
	}

	while (fgets(id, sizeof(id), fp1) != NULL) { //while loop over all the Ids
		if (fgets(pass_line, sizeof(pass_line), fp1)); //another fgets in order to skip '\n'
		char *current_grade_path=NULL;
		current_grade_path = (char*)malloc(capacity);
		if (NULL == current_grade_path) {
			printf("Memory allocation has failed!");
			return(ERROR_CODE);
		}
		id[9] = '\0';
		strcpy(current_grade_path, argv_str);
		strcat(current_grade_path, "/grades_");
		strcat(current_grade_path, id);

		/* send to TestGrade the current student */

		test_grade_exit_code = CreateProcessSimpleMain(current_grade_path);
		if (test_grade_exit_code == ERROR_CODE) {
			printf("Error in TestGrade process.\n");
			printf("Captain, we were unable to calculate %s\n", id);
			return ERROR_CODE;
		}

		/* print student id and his final grade into the main final grades file */

		if (print2final(path_cpy, id) == -1) { //check if the print secceeded
			printf("Error while printing a grade into the main final grades.");
			return ERROR_CODE;
		}
		free(current_grade_path);
	}

	/* end of while */

	/* file closure and memory free */

	fclose(fp1);
	free(path);
	free(path_cpy);
	free(argv_str);

	printf("The grades have arrived, captain\n");
	return SUCCESS_CODE;
}
