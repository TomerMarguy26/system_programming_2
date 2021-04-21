/*
Author: Tomer Marguy...........id_number: 205705874
Project name: "TestGrade"
*/

#include "TestGradeIncludesAndDefines.h"
#include"TestGradeFunctions.h"

/* Global variables use for threads */

HANDLE p_thread_handles[NUM_THREADS];
DWORD p_thread_ids[NUM_THREADS];
DWORD wait_code;
BOOL ret_val;
size_t i; //the iter. index over the (13) threads
int grades[NUM_THREADS] = { -1 }, final_grade = -1, mid = -1, moedA = -1, moedB = -1;
float grades_float[NUM_THREADS] = { -1 }, grades_ex[10] = { -1 };

/*
Description: This is the main function of TestGrade. It will execute when TestManager will create a new process.
			 argv[1] will be the <grade_path>, the path the specific file with the grade that the thread will test.
			 The final grade file of the specific student will appear in <grade_path> folder.
Parameters:  int argc - the number of strings pointed to by argv.
			 char *argv[] - an array of strings representing the individual arguments provided on the command line.
Returns:	 Exit program with 0 if everything went ok.
*/

int main(int argc, char *argv[]) {
	char *path = NULL; //path will contain the given (argument) path
	int capacity = PATH_LENGTH + strlen(argv[1]);
	path = (char*)malloc(capacity);
	if (NULL == path) {
		printf("Memory allocation has failed!");
		return(ERROR_CODE);
	}
	strcpy(path, argv[1]);

	ret_val = fill_array(path, p_thread_handles, p_thread_ids); //fill array with relevant thraed handles information

	/* checks that everything went ok with threads */
	if (ret_val != SUCCESS_CODE) {
		return ret_val;
	}

	/* casting grades array from int to float*/
	for (i = 0; i < NUM_THREADS; i++) {
		grades_float[i] = (float)grades[i];
		/* for the calc function */
		if (i<10)
			grades_ex[i] = (float)grades[i];
	}

	final_grade = calculate_final_grade(grades_ex, grades_float[10], grades_float[11], grades_float[12]); // calc final grade of a specific student
	if (print2student(path, final_grade) == ERROR_CODE) { //if condition is satisfied, we have a problem
		printf("Failed to open final grades file. \n");
		return ERROR_CODE;
	}
	free(path);
	return SUCCESS_CODE;
}

/* Function That Uses Global Variables: */

/*
Description: This is the function that will be called from the main. This function fills an array p_thread_handles
		     which contains information about each grade of all the exercises and exams, of a specific student
			 (with the current ID).
Parameters:  char *path - the initial given in the argument path.
Returns:	 exit program with 0 if everything went ok, with -1 else.
*/

int fill_array(char * path) {
	int i = 0;
	char current_path1[(2 * PATH_LENGTH)] = "", current_path2[(2 * PATH_LENGTH)] = "", current_path3[(2 * PATH_LENGTH)] = "", current_path4[(2 * PATH_LENGTH)] = "";
	char current_path5[(2 * PATH_LENGTH)] = "", current_path6[(2 * PATH_LENGTH)] = "", current_path7[(2 * PATH_LENGTH)] = "", current_path8[(2 * PATH_LENGTH)] = "";
	char current_path9[(2 * PATH_LENGTH)] = "", current_path10[(2 * PATH_LENGTH)] = "", current_path_mid[(2 * PATH_LENGTH)] = "", current_path_A[(2 * PATH_LENGTH)] = "";
	char current_path_B[(2 * PATH_LENGTH)] = ""; //malloc also can be used x 13 times, took a max. value based on given assumptions.

	/* creting full path to each grade */

	sprintf(current_path1, "%s%s", path, "/ex1.txt");
	sprintf(current_path2, "%s%s", path, "/ex2.txt");
	sprintf(current_path3, "%s%s", path, "/ex3.txt");
	sprintf(current_path4, "%s%s", path, "/ex4.txt");
	sprintf(current_path5, "%s%s", path, "/ex5.txt");
	sprintf(current_path6, "%s%s", path, "/ex6.txt");
	sprintf(current_path7, "%s%s", path, "/ex7.txt");
	sprintf(current_path8, "%s%s", path, "/ex8.txt");
	sprintf(current_path9, "%s%s", path, "/ex9.txt");
	sprintf(current_path10, "%s%s", path, "/ex10.txt");
	sprintf(current_path_mid, "%s%s", path, "/midterm.txt");
	sprintf(current_path_A, "%s%s", path, "/moedA.txt");
	sprintf(current_path_B, "%s%s", path, "/moedB.txt");

	p_thread_handles[0] = CreateThreadSimple(extract_grade, current_path1, &p_thread_ids[0]);
	p_thread_handles[1] = CreateThreadSimple(extract_grade, current_path2, &p_thread_ids[1]);
	p_thread_handles[2] = CreateThreadSimple(extract_grade, current_path3, &p_thread_ids[2]);
	p_thread_handles[3] = CreateThreadSimple(extract_grade, current_path4, &p_thread_ids[3]);
	p_thread_handles[4] = CreateThreadSimple(extract_grade, current_path5, &p_thread_ids[4]);
	p_thread_handles[5] = CreateThreadSimple(extract_grade, current_path6, &p_thread_ids[5]);
	p_thread_handles[6] = CreateThreadSimple(extract_grade, current_path7, &p_thread_ids[6]);
	p_thread_handles[7] = CreateThreadSimple(extract_grade, current_path8, &p_thread_ids[7]);
	p_thread_handles[8] = CreateThreadSimple(extract_grade, current_path9, &p_thread_ids[8]);
	p_thread_handles[9] = CreateThreadSimple(extract_grade, current_path10, &p_thread_ids[9]);
	p_thread_handles[10] = CreateThreadSimple(extract_grade, current_path_mid, &p_thread_ids[10]);
	p_thread_handles[11] = CreateThreadSimple(extract_grade, current_path_A, &p_thread_ids[11]);
	p_thread_handles[12] = CreateThreadSimple(extract_grade, current_path_B, &p_thread_ids[12]);

	for (i = 0; i < NUM_THREADS; i++) {
		if (NULL == p_thread_handles[i])
		{
			//need to return the exact thread number
			printf("Error when creating thread: %d\n", GetLastError());
			for (i = 0; i < NUM_THREADS; i++)
			{
				ret_val = CloseHandle(p_thread_handles[i]);
				if (FALSE == ret_val)
				{
					printf("Error when closing thread: %d\n", GetLastError());
					return ERROR_CODE;
				}
			}
			return ERROR_CODE;
		}
	}

	/* Wait for all threads to finish and check */

	wait_code = WaitForMultipleObjects(NUM_THREADS, p_thread_handles, TRUE, INFINITE);
	if (WAIT_OBJECT_0 != wait_code) {
		printf("Error when waiting\n");
		for (i = 0; i < NUM_THREADS; i++)
		{
			ret_val = CloseHandle(p_thread_handles[i]);
			if (FALSE == ret_val)
			{
				printf("Error when closing thread: %d\n", GetLastError());
				return ERROR_CODE;
			}
		}
		return ERROR_CODE;
	}

	/* extrct exit code (the grade from .txt) from each thread into grades[] array with all grades */

	for (i = 0; i < NUM_THREADS; i++) {
		ret_val = GetExitCodeThread(p_thread_handles[i], &grades[i]);
		if (0 == ret_val) {
			printf("Error when getting thread exit code\n");
			for (i = 0; i < NUM_THREADS; i++)
			{
				ret_val = CloseHandle(p_thread_handles[i]);
				if (FALSE == ret_val)
				{
					printf("Error when closing thread: %d\n", GetLastError());
					return ERROR_CODE;
				}
			}
			return ERROR_CODE;
		}
	}

	/* Close thread handles */

	for (i = 0; i < NUM_THREADS; i++)
	{
		ret_val = CloseHandle(p_thread_handles[i]);
		if (FALSE == ret_val)
		{
			printf("Error when closing thread: %d\n", GetLastError());
			return ERROR_CODE;
		}
	}
}
