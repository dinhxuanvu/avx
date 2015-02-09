#ifndef MACROS_H
#define MACROS_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#define RESET	  	"\033[0m"
#define BLACK	  	"\033[30m"
#define RED 		"\033[31m"
#define GREEN	  	"\033[32m"
#define YELLOW	  	"\033[33m"
#define BLUE	  	"\033[34m"
#define MAGENTA	  	"\033[35m"
#define CYAN	  	"\033[36m"
#define WHITE	  	"\033[37m"
#define CLEAR		"\033[2J"

#if DEBUG
#define LOG_MESSAGE(fmt, ...) printf(GREEN "(%s:%s:%d)\n\t" RESET fmt "\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#define LOG_WARNING(fmt, ...) printf(YELLOW "(%s:%s:%d)\n\t" RESET fmt , __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#define LOG_ERROR(fmt, ...) printf(RED "(%s:%s:%d)\n\t" RESET fmt , __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#else
#define LOG_MESSAGE(fmt, ...) 	do{} while (0)
#define LOG_WARNING(fmt, ...) 	do{} while (0)
#define LOG_ERROR(fmt, ...) 	do{} while (0)
#endif

#define CLEAR_SCREEN	std::system("clear");

#endif /* MACROS_H */
