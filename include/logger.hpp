#pragma once
#include <iostream> 
#include <ctime>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

 FILE *fd;

enum LogLevel {logError, logWarning, logInfo, logDebug};
enum outputStream {term, file, both};

void _write(FILE *fd, char *buffer){
       // current date/time based on current system
   time_t now = time(0);

   tm *ltm = localtime(&now);
    
    int cx = fprintf (fd, "%i.%i.%i %i:%i:%i %s\n",ltm->tm_mday,1 + ltm->tm_mon,1900 + ltm->tm_year,ltm->tm_hour,ltm->tm_min,ltm->tm_sec, buffer);
    }

//Вывод в терминал всего сообщения
#define terminalOutput(Error, error_message)\
    cout << #Error << " ";\
    _write(stdout, error_message);

#define fileOutput(Error, str)\
    fprintf (fd, "%s ", #Error);\
    _write(fd, str);

#define log(Error, error_message, choice)\
    if (choice == term){\
        terminalOutput(Error, error_message);\
    }\
    else if (choice == file){\
        fileOutput(Error, error_message);\
    }\
    else {\
        terminalOutput(Error, error_message);\
        fileOutput(Error, error_message);\
    }
    
void initLogger(){
    fd = fopen("log.log", "a");
    
    if (!fd) {
        cout << "Файл не открыт\n\n";
        exit(1);
    }
}
