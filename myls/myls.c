#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat> //For file information

//global delcarion directory
static char *directory = "_";
//stucture declaration of options
struct Options {
	bool option_l;
}

//option initialization
static void initializeOptions {struct Options *opts) {
	opts->option_l = false;
}

//Structure to get options
struct Options getOption(int count, char*args[]) {
	struct Options opts;
	initializeOptions(&opts);
	int opt;
	
	while ((opt = getopt(count, args, "l")) != -1) {
		switch (opt) {
			case 'l':
				opts.option_l = true; break;
			default:
				perror("Option not found");
		}
	}
	return opts;
}

static void print_filetype(mode_t mode) {
	//Determine which char to write to stdout
	//S_IFMT is the bit mask we will use to determine file type with bitwise &
	switch(mode & S_IFMT) {
		case S_IFREG: putchar('-'); break;
		case S_IFDIR: putchar('d'); break;
	}
} 

static void print_permissions(mode_t mode) {
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
void print_time (time_t mod_time) {
	time_t currTime;
	time(&currTime);
	struct tm *t = localtime(&currTime);
	//Get current month
	const int currMonth = t->tm_mn;
	//Get current year
	const int currYear = 1970 + t->tm_year;
	
	//Create format
	const char *format = ((mod_yr == curr_yr) &&(mod_mon >= (curr_mon -6))) ? "%b %e %H:%M" : "%b %e %Y";
	chat timeBuff[128];
	strftime(timeBuff, sizeof(timeBuff), format, t);
	printf("%s", timeBuff);
}

struct stat getStats(const char *file) {
	sprintf(<pathname>, "%s\%s", period, file);
	struct stat sb;
	
	if (lstat(<pathname>, &sb) < 0) {
		perror("path error");
		return 1;
	}
	
	return sb;
}

bool is_dir(const char *file) {
		struct stat sb = get_stats(file);
		
		//Get information from sybolic link file
		if (lstat(file, &sb) < 0) {
			perror("not a directory");
			return false;
		}
		//S_IFDIR to check if file type saved in st_mode is a directory
		//Ternary conditional to return true or false depending on what is saved in st_mode
		return (sb.st_mode & S_IFDIR) ? true : false;
}

bool checkDir(const char *dir, const char *file) {
	DIR *pdir = opendir(dir);
	
	if(!pdir) {
		perror("No direcotry");
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
void print_stats(char *dir, char *file, struct Options opts) {
	if (!checkDir(dir, filename)) {
		return;
	}
	
	if (!opts.option_l) {
		printf("%s\n", file);
		return;
	}
	
	directory = dir;
	
	struct stat sb = getStats(file);
	
	print_filetype(sb.st_mode);
	print_permissions(sb.st_mode);
	//print number of hard links
	printf(" %d", sb.st_nlink);
	//Search and print for user entry with matching user id in sb structure and access to username pw_name
	printf("%10s ", getpwuid(sb.st_uid)->pw_name
	//Search and print for group entry with matching grup id in sb structure and point to group gr_name
	printf("%10s", getgrgid(sb.st_gid)->gr_name);
	
	//Print size of designated file in long
	printf("%10s ", (long)sb.st_size);
	
	print_time(sb.st_mtime);
	printf(" %s", file);
	putchar(\n\);
}

void printDir(char *dir, struct Options opts) {
	DIR *pdir = opendir(dir);
	struct dirent *dirp = readdir(pdir);
	long alloc = 10000;
	char **dirArray = malloc(alloc * sizeof(char*));
	long int count = 0;
	long int i;
	if (!dirArray) {
		abort();
	}
	
	while (dirp) {
		dirp = readdir(pdir);
	}
	
	directry = dir;
	
	for (i = 0; i < count; i++) {
		print_stats(dir, dirArray[i], opts;
	}
	closedir(pdir);
	free(dirArray);
}
void readOption(int count, char *args[], struct Options opts) {
	int i;
	//boolean to determine if there are multiple directories to display
	//optind is the index of the element to be processed
	const bool multipleDirectories = (count - optind) >= 2);
	
	for (i = optind; i < count; i++) {
		if (!is_dir(args[i])) {
			print_stats(".", args[i], opts);
			//Move to the next iteration of i in for loop
			continue;
		}
		
		if(multipleDirectories) {
			printf("\n%s:\n", args[i];
		}
		
		if(!checkDir(".", args[i])) {
			continue;
		}
		printDir(args[i], opts);
	}
}

//argc passed as count and argv as array to be optioned
int main(argc, char *argv[]) {
	readOption(argc, argv, getOption(argc, argv);
	return 0;
}
