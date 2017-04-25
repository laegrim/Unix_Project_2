#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h> //For directory entries
#include <string.h> //Manipulate strings
#include <getopt.h> //For getopt function
#include <sys/stat.h> //For file information
#include <sys/types.h> //For additional data types
#include <stdbool.h> //Boolean types and values
#include <time.h> //For Time types
#include <sysexits.h> //For preferred exit codes
#include <pwd.h> //For Password structure
#include <grp.h> //For group structure

//global delcarion directory
static char *GLOBALDIR = ".";
//stucture declaration for option l. Can be expanded for additional options
struct Options {
	bool option_l;
};

//option initialization
static void initializeOptions (struct Options *opts) {
	opts->option_l = false;
}

//Structure to get options.  Would expand case arguements for additional option flexibility
struct Options getOption(int count, char*args[]) {
	struct Options opts;
	initializeOptions(&opts);
	int opt;
	
	//This while loop will determimne if the option in use is -l.
	while ((opt = getopt(count, args, "l")) != -1) {
		switch (opt) {
			case 'l':
				opts.option_l = true; break;
			case '?':
				//Exit to be called for wrong number of arguments or bad syntax in parameter opt
				exit(EX_USAGE);
		}
	}
	return opts;
}

static void printFiletype(mode_t mode) {
	//Determine which char to write to stdout
	//S_IFMT is the bit mask we will use to determine file type with bitwise &
	switch(mode & S_IFMT) {
		case S_IFREG: putchar('-'); break;
		case S_IFDIR: putchar('d'); break;
	}
} 

static void printPermissions(mode_t mode) {
	//Determine permissions for file with info stored
	//Place a char based on outcome of &
	putchar((mode & S_IRUSR) ? 'r' : '-');
	putchar((mode & S_IWUSR) ? 'w' : '-');
	putchar((mode & S_IXUSR) ? 'x' : '-');
	putchar((mode & S_IRGRP) ? 'r' : '-');
	putchar((mode & S_IWGRP) ? 'w' : '-');
	putchar((mode & S_IXGRP) ? 'x' : '-');
	putchar((mode & S_IROTH) ? 'r' : '-');
	putchar((mode & S_IWOTH) ? 'w' : '-');
	putchar((mode & S_IXOTH) ? 'x' : '-');
}

//Function to display time in correct format
void printTime (time_t mod_time) {
	time_t currTime;
	time(&currTime);
	struct tm *t = localtime(&currTime);
	//Get current month
	const int currMonth = t->tm_mon;
	//Get current year
	const int currYear = 1970 + t->tm_year;
	
	// get mod time and year
   	 t = localtime(&mod_time);
    	const int mod_mon = t->tm_mon;
    	const int mod_yr = 1970 + t->tm_year;

	//Create format
	const char *format = ((mod_yr == currYear) &&(mod_mon >= (currMonth -6))) ? "%b %e %H:%M" : "%b %e %Y";
	char timeBuff[128];
	strftime(timeBuff, sizeof(timeBuff), format, t);
	printf("%s", timeBuff);
}

//This structure will be called if additional information for files in directory is required
struct stat getStats(const char *file) {
	char path[1024];
	//sprintf will generate formatted path with directory and file
	sprintf(path, "%s/%s", GLOBALDIR, file);
	//Use of stat structure to be used to get info of file
	struct stat sb;
	
	//If statement to catch if we can get specified directory
	if (stat(path, &sb) < 0) {
		perror("Error in getstats\n");
		exit(EX_IOERR);
	}
	
	return sb;
}

//Return boolean value to determine if file is a directory
bool isDir(const char *file) {
		//Set sb structure info to the info found for file
		struct stat sb = getStats(file);
		
		//Determine if info obtained for sb is typical of a directory
		if (stat(file, &sb) < 0) {
			perror("not a directory");
			return false;
		}

		//S_IFDIR to check if file type saved in st_mode is a directory
		//Ternary conditional to return true or false depending on what is saved in st_mode
		return (sb.st_mode & S_IFDIR) ? true : false;
}

//Function to retrun boolean value set to whether specified file is in directory
bool checkDir(const char *dir, const char *file) {
	//Open a directory stream	
	DIR *pdir = opendir(dir);
	
	//Check if we successufly obtained a directory stream
	if(!pdir) {
		perror("Not directory");
		return false;
	}
	
	//Get the info of a directory pointer in a directory stream
	struct dirent *dirp = readdir(pdir);
	
	while (dirp) {
		//Determine if file is in the specified directory
		if (strcmp(file, dirp->d_name) == 0) {
			closedir(pdir);
			return true;
		}
		//Get the next director entry
		dirp = readdir(pdir);
	}
	//File is not found
	fprintf(stderr, "file '%s\' not found\n", file);
	//Close opened directory
	closedir(pdir);
	return false;
}

//To compare two strings in order to sort files by lex
static int compareLex(const void *p1, const void *p2) {
	const char *str1 = *(const void **)p1;
	const char *str2 = *(const void **)p2;
	//Will return an integer value signifying greater than, equal to, or less than 0
	return strcasecmp(str1, str2);
}

// Function that will obtain directory, file, and designated options (if any) and then print the info required
void printStats(char *dir, char *file, struct Options opts) {
	//Function will not print if specified file is not in directory	
	if (!checkDir(dir, file)) {
		return;
	}
	
	//If -l option is not in use then we will just print file name
	if (!opts.option_l) {
		printf("%s  ", file);
		return;
	}
	
	//Set global directory
	GLOBALDIR = dir;
	
	//Store the infe for file to be displayed
	struct stat sb = getStats(file);
	
	//Print a '-' or 'd' determined by either file or directory
	printFiletype(sb.st_mode);
	//Print permissions with the help of designated headers
	printPermissions(sb.st_mode);
	//print number of hard links
	printf(" %ld", sb.st_nlink);
	//Search and print for user entry with matching user id in sb structure and access to username pw_name
	printf(" %s ", getpwuid(sb.st_uid)->pw_name);
	//Search and print for group entry with matching grup id in sb structure and point to group gr_name
	printf("%s ", getgrgid(sb.st_gid)->gr_name);
	
	//Print size of designated file in long
	printf("%ld ", (long)sb.st_size);
	
	printTime(sb.st_mtime);
	printf(" %s", file);
	putchar('\n');
}

//Function to print what lies in directory
void printDir(char *dir, struct Options opts) {
	//Open directory stream	
	DIR *pdir = opendir(dir);
	//Prepare to examine contets of directory
	struct dirent *dirp = readdir(pdir);
	long alloc = 30000;
	//Prepare to dynamically allocate for directory contents
	char **dirArray = malloc(alloc * sizeof(char*));
	long int count = 0;
	long int i;
	//Exit program if we could not make dirArray
	if (!dirArray) {
		abort();
	}
	
	while (dirp) {
		//Do not show hidden files because we are not using -a
		const bool noHidden = dirp->d_name[0] == '.';
	
		if (!noHidden) {
			//Allocation for dirArray
			if (count >= alloc) {
				alloc *= 2;
				dirArray = realloc(dirArray, alloc * sizeof(char*));
				if (!dirArray) {
					abort();
				}
			}
			//Get file name and store in dirArray
			dirArray[count] = dirp->d_name;
			count++;
		}
		dirp = readdir(pdir);
	}
	
	GLOBALDIR = dir;
	
	//Sort strings held in dirArray
	qsort(dirArray, count, sizeof(char*), compareLex);
	
	//Print the info for or specified directory
	for (i = 0; i < count; i++) {
		printStats(dir, dirArray[i], opts);
	}
	closedir(pdir);
	free(dirArray);
}
void readDirs(int count, char *args[], struct Options opts) {
	int i;
	
	//optind is the index of the element to be processed
	//Print contents of current directory if no optioned presented
	if (optind == count) {
		printDir(".", opts);
	}

	//boolean to determine if there are multiple directories to display
	const bool multipleDirectories = (count - optind) >= 2;
	
	for (i = optind; i < count; i++) {
		if (!isDir(args[i])) {
			printStats(".", args[i], opts);
			//Move to the next iteration of i in for loop
			continue;
		}
		
		if(multipleDirectories) {
			printf("\n%s:\n", args[i]);
		}
		
		if(!checkDir(".", args[i])) {
			continue;
		}
		printDir(args[i], opts);
	}
}

//argc passed as count and argv as array to be optioned
int main(int argc, char *argv[]) {
	readDirs(argc, argv, getOption(argc, argv));
	putchar('\n');
	return 0;
}
