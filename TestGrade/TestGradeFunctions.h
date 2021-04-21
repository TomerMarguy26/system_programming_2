#pragma once
int calculate_final_grade(float ex[], float Fd, float moedA, float moedB);
int print2student(char *path, int final_grade);
int fill_array(char * path);
DWORD WINAPI extract_grade(LPVOID lpParam);
static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPVOID p_thread_parameters,
	LPDWORD p_thread_id);
