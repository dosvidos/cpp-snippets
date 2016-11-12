#include <stdio.h>

int main(int argc,          /* number of strings in array argv */
    char *argv[ ],             /* array of command-line argument strings */
    char **envp)             /* array of environment variable strings */
{
    int count;
	/* display each command-line argument. */
    printf("\nThe command-line arguments:\n");
    for(count = 0; count < argc; count++)
        printf("  argv[%d]   %s\n", count, argv[count]);
    /* display each environment variable.*/
    printf("\nEnvironment variables:\n");
    while(*envp != NULL)
        printf("  %s\n", *(envp++));

	return 0;
}

//The standard library function strstr(a, b) returns a pointer to the first occurrence of the string b in the string a,
//or NULL if there is none.  It is declared in <string.h> or C++ wrapper <cstring>.

int main(int argc, char *argv[], char *envp[]) {
    // the default is no line numbers.
    int LineNum = 0;   
    // if /n is passed to the .exe program, display
    // numbered listing of environment variables.
    // if program name and switch/option... AND stricmp...
    if((argc == 2) && stricmp(argv[1], "/n" ) == 0)
        LineNum = 1;
    else
        cout<<"no \'/n\' passed..."<<endl;
    for(int i = 0; envp[i] != NULL; ++i) {
        if(LineNum)
        cout<<i<<": "<<envp[i]<<"\n";
    }
    cout<<"Usage: searchpattern /n\n";
    return 0;
}

//To return an exit code when main() or wmain() is declared as void,
//you must use the exit() function from stdlib.h.

