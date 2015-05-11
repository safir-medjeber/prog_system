#ifndef PARSING_H_INCLUDED

#define PARSING_H_INCLUDED

int isInt(char *buffer);
int numberArg(int argc);
char* nameFunc(int argc, char* argv[]);
int runClientAfterParse(int argc, char* argv[]);

#endif
