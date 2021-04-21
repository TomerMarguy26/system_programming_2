#pragma once
#define _CRT_SECURE_NO_WARNINGS 
#define _CRT_NONSTDC_NO_DEPRECATE 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#define PATH_LENGTH 256 //give assumption
#define ID_LENGTH 10 //9 digits ID number (+'\0')
#define TIMEOUT_IN_MILLISECONDS 2500
#define BRUTAL_TERMINATION_CODE 0x55
#define ERROR_CODE ((int)(-1))
#define SUCCESS_CODE ((int)(0))