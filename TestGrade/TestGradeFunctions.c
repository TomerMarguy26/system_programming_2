#include "TestGradeIncludesAndDefines.h"
#include"TestGradeFunctions.h"

/*
Description: This is the function that will write the students final grade to a .txt file in his grades folder.
Parameters:  char *path - the initial given in the argument path.
			 int final_grade - the calculated final grade of the student (after rounding up).
Returns:	 exit program with 0 if everything went ok, with -1 else.
*/

int print2student(char *path, int final_grade) {
	char id[ID_LENGTH] = "";
	char *local_path=NULL; //will contain the path to the .txt file that will contain the student's final grade
	int capacity = PATH_LENGTH + strlen(path);
	local_path = (char*)malloc(capacity);
	if (NULL == local_path) {
		printf("Memory allocation has failed!");
		return(ERROR_CODE);
	}

	int index_of_end = 0, c = 0, i=0; //c is the index for crating the id

	while( path[index_of_end+1] != '\0')
		index_of_end++;

	for (int i = index_of_end-8 ; i <= index_of_end; i++) {
		id[c] = path[i];
		c++;
	}
	id[9] = '\0'; //the id has been extracted by the upper for loop

	/* path creation */

	sprintf(local_path, "%s", path);
	strcat(local_path, "/final_");
	strcat(local_path, id);
	strcat(local_path, ".txt");

	/* grade writing */

	FILE  *final_fp = fopen(local_path, "w");
	if (NULL == final_fp)
	{
		printf("Failed to open file. \n");
		return ERROR_CODE; //in case of failure the function will return -1
	}
	else {
		fprintf(final_fp, "%d%c", final_grade, '\n');
		fclose(final_fp);
		free(local_path);
		return SUCCESS_CODE;
	}
}

/*
Description: This is the function that claculates the final grade of the student with the current id (from
			 the list). The calaculation is done by the given instructions in our project brief.
Parameters:  float ex[] - an array with all the ex. (1-10) grades of the student.
			 float mid - the midterm exam grade of the student.
			 float moedA - the Moed A exam grade of the student.
			 float moedB - the Moed B exam grade of the student.
Returns:	 exit program with the calculated final grade.
*/

int calculate_final_grade(float ex[], float mid, float moedA, float moedB) {
	float final_moed = 0, final_ex = 0, sum_ex = 0, residue = 0, final_grade = 0;
	int min = ex[0], min_index = 0;

	/* reset to 0 all grades under 60 */

	for (int i = 0; i < 10; i++) {
		if (ex[i] < 60)
			ex[i] = 0;
	}

	/* checks which moed is chosen */

	if (moedB < 60 && moedB >0) {
		moedB = 0;
		final_moed = moedB;
	}
	else {
		if (moedB >= 60)
			final_moed = moedB;
		else //moedB grade is 0, the student did just moedA
			final_moed = moedA;
	}

	if (mid < 60)
		mid = 0;

	/* reset to 0 the two lowest grades so in my calc just the max 8 will left */

	for (int run = 0; run < 2; run++) {
		for (int i = 1; i < 10; i++) {
			if (ex[i] < min) {
				min = ex[i];
				min_index = i;
			}
		}
		ex[min_index] = 101;
		min = ex[min_index];
		min_index = 0;
	}

	/* calculate just ex final */

	for (int i = 0; i < 10; i++) {
		if (ex[i] != 101)
			sum_ex += ex[i];
	}
	final_ex = sum_ex / 8.0;

	final_grade = (final_moed * 0.6) + (final_ex * 0.2) + (mid * 0.2); //calculate the final grade in float

	/* If the final grade is not an integer we need to round him upwards */

	residue = final_grade - (int)final_grade;
	if (residue > 0)
		final_grade++;

	return (int)final_grade;
}

/* Threads functions: */

static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPVOID p_thread_parameters,
	LPDWORD p_thread_id)
{
	HANDLE thread_handle;

	if (NULL == p_start_routine)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
	}

	if (NULL == p_thread_id)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
	}

	thread_handle = CreateThread(
		NULL,            //  default security attributes 
		0,               //  use default stack size 
		p_start_routine, //  thread function 
		p_thread_parameters, //  argument to thread function 
		0,               //  use default creation flags 
		p_thread_id);    //  returns the thread identifier 
	
	if (NULL == thread_handle)
	{
		printf("Couldn't create thread\n");
	}

	return thread_handle;
} 

/* function to take out the grade, will be used in CreateThread */

DWORD WINAPI extract_grade(LPVOID lpParam) {
	Sleep(10);
	char *file_name = (char*)lpParam;
	FILE *fp = NULL;
	char grade[4] = "";
	if (NULL == (fp = fopen(file_name, "r"))) {
		printf("Unable to open file!");
		return ERROR_CODE;
	}
	fgets(grade, sizeof(grade), fp);
	fclose(fp);
	return atoi(grade);
}
