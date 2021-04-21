#include "TestManagerIncludesAndDefines.h"
#include "TestManagerFunctions.h"

/*
Description: This is the function that will be called from the main in every loop in the while,
			 each time when there are still student Ids left. This function will eventually print each student's
			 grade to the final grades file (with the list of student Ids and their final grades in the course).
Parameters:  char *path - the initial given in the argument path.
		     char *id - the current student's Id. Will be updated in every loop.
Returns:	 exit program with 0 if everything went ok, with -1 else.
*/

int print2final(char *path, char *id) {
	char what_to_print[ID_LENGTH + 5] = ""; //what_to_print's format is: "<ID> <FinalGrade>"
	char *path_to_openfile = NULL; //path_to_openfile will cotain the path to the .txt final grade file
	char *path_to_extract_grade = NULL; //path_to_extract_grade will contain path to a specific students grade
	int capacity = PATH_LENGTH + strlen(path);
	path_to_openfile = (char*)malloc(capacity);
	if (NULL == path_to_openfile) {
		printf("Memory allocation has failed!");
		return(ERROR_CODE);
	}
	path_to_extract_grade = (char*)malloc(capacity);
	if (NULL == path_to_extract_grade) {
		printf("Memory allocation has failed!");
		return(ERROR_CODE);
	}
	char final_grade_str[5] = ""; //max 3 digits grade + '\n' + '\0' = 5

	/* creating the relevant paths */

	strcpy(path_to_openfile, path);
	strcpy(path_to_extract_grade, path);
	strcat(path_to_openfile, "/final_grade.txt");
	strcat(path_to_extract_grade, "/grades_");
	strcat(path_to_extract_grade, id);
	strcat(path_to_extract_grade, "/final_");
	strcat(path_to_extract_grade, id);
	strcat(path_to_extract_grade, ".txt");

	/* grade extracion for a specific student (with the current ID) */

	FILE  *final_grade_extract = fopen(path_to_extract_grade, "r");
	if (NULL == final_grade_extract)
	{
		printf("Failed to open file. \n");
		return ERROR_CODE; //in case of failure the function will return -1
	}

	else
		fgets(final_grade_str, sizeof(final_grade_str), final_grade_extract);

	fclose(final_grade_extract);

	strcat(what_to_print, id);
	strcat(what_to_print, " ");
	strcat(what_to_print, final_grade_str);

	/* writing to the final file (with the list of all grades and Ids) */

	FILE  *final_grade_ptr = fopen(path_to_openfile, "a");
	if (NULL == final_grade_ptr)
	{
		printf("Failed to open file. \n");
		return ERROR_CODE; //in case of failure the function will return -1
	}

	else {
		fprintf(final_grade_ptr, "%s", what_to_print);
		fclose(final_grade_ptr);
		return SUCCESS_CODE; //all good!
	}
	free(path_to_openfile);
	free(path_to_extract_grade);
}

/*
Description: This functions were taken from the recitation files. These functions will
			 calculate the result of the current small expression that was passed from Father.
Parameters: char *small_expression - the small expression we want to calculate, includes brackets.
Returns:	The return value is the result of the expression from Father, as int.
*/
int CreateProcessSimpleMain(char *grade_path)
{
	PROCESS_INFORMATION procinfo;
	DWORD				waitcode;
	DWORD				exitcode;
	BOOL				retVal;
	CHAR				command[14 + PATH_LENGTH] = ""; /* <ISP> TCHAR is a win32  */
	sprintf(command, "TestGrade.exe %s", grade_path);	/* generic char which may be either a simple (ANSI) char or a unicode char, */
	command[strlen(command)] = '\0';									/* depending on behind-the-scenes operating system definitions. Type LPTSTR */
													/* is a string of TCHARs. Type LPCTSTR is a const string of TCHARs. */

													/*  Start the child process. */
	retVal = CreateProcessSimple(command, &procinfo);

	if (retVal == 0)
	{
		printf("Process Creation Failed!\n");
		return ERROR_CODE;
	}

	waitcode = WaitForSingleObject(
		procinfo.hProcess,
		TIMEOUT_IN_MILLISECONDS); /* Waiting 2.5 secs for the process to end */

	printf("WaitForSingleObject output: ");
	switch (waitcode)
	{
	case WAIT_TIMEOUT:
		printf("WAIT_TIMEOUT\n"); break;
	case WAIT_OBJECT_0:
		printf("WAIT_OBJECT_0\n"); break;
	default:
		printf("0x%x\n", waitcode);
	}

	if (waitcode == WAIT_TIMEOUT) /* Process is still alive */
	{
		printf("Process was not terminated before timeout!\n"
			"Terminating brutally!\n");
		TerminateProcess(
			procinfo.hProcess,
			BRUTAL_TERMINATION_CODE); /* Terminating process with an exit code of 55h */
		Sleep(10); /* Waiting a few milliseconds for the process to terminate */
	}

	GetExitCodeProcess(procinfo.hProcess, &exitcode);

	printf("The exit code for the process is 0x%x\n", exitcode);


	CloseHandle(procinfo.hProcess); /* Closing the handle to the process */
	CloseHandle(procinfo.hThread); /* Closing the handle to the main thread of the process */
	return (int)exitcode;
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

BOOL CreateProcessSimple(LPTSTR CommandLine, PROCESS_INFORMATION *ProcessInfoPtr)
{
	STARTUPINFO	startinfo = { sizeof(STARTUPINFO), NULL, 0 }; /* <ISP> here we */
															  /* initialize a "Neutral" STARTUPINFO variable. Supplying this to */
															  /* CreateProcess() means we have no special interest in this parameter. */
															  /* This is equivalent to what we are doing by supplying NULL to most other */
															  /* parameters of CreateProcess(). */

	return CreateProcess(NULL, /*  No module name (use command line). */
		CommandLine,			/*  Command line. */
		NULL,					/*  Process handle not inheritable. */
		NULL,					/*  Thread handle not inheritable. */
		FALSE,					/*  Set handle inheritance to FALSE. */
		NORMAL_PRIORITY_CLASS,	/*  creation/priority flags. */
		NULL,					/*  Use parent's environment block. */
		NULL,					/*  Use parent's starting directory. */
		&startinfo,				/*  Pointer to STARTUPINFO structure. */
		ProcessInfoPtr			/*  Pointer to PROCESS_INFORMATION structure. */
	);
}