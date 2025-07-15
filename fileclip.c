#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define FILECLIP_VERSION "0.1.0"

void printHelp();
void copy(const char *arg, const char *cwd);
void paste();
int isDir(const char *path);
void showClipboardContents();
void clearClipboard();

/**
 * @brief Main entry point for the fileclip utility.
 * 
 * Handles command-line arguments and dispatches to the appropriate functionality.
 * 
 * @param argc Argument count
 * @param argv Argument vector
 * @return int Exit code
 */
int main(int argc, char *argv[]) {
  char cwd[PATH_MAX];

  // Get current working directory
  if (getcwd(cwd, sizeof(cwd)) == NULL) {
    perror("fileclip: getcwd");
    return 2;
  }

  // Check for valid argument count
  if (argc < 2 || argc > 3) {
    fprintf(stderr, "fileclip: invalid arguments\n");
    printHelp();
    return 1;
  }

  if (argc == 2) {
    // Handle one-argument commands
    if (strcmp(argv[1], "-c") == 0) {
      copy("null", cwd); // Default to current directory
    } else if (strcmp(argv[1], "-p") == 0) {
      paste();
    } else if (strcmp(argv[1], "-h") == 0) {
      printHelp();
    } else if (strcmp(argv[1], "-r") == 0) {
      clearClipboard();
      printf("fileclip: clipboard cleared\n");
    } else if (strcmp(argv[1], "-d") == 0) {
      showClipboardContents();
    } else if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
      printf("fileclip version %s\n", FILECLIP_VERSION);
    } else {
      fprintf(stderr, "fileclip: unknown argument\n");
      return 1;
    }
  } else if (argc == 3) {
    // Handle copy with file argument
    if (strcmp(argv[1], "-c") == 0) {
      copy(argv[2], cwd);
    } else {
      fprintf(stderr, "fileclip: invalid usage\n");
      printHelp();
      return 1;
    }
  }

  return 0;
}

/**
  * @brief Prints usage information for the fileclip utility. 
 */
void printHelp() {
  printf("Usage: fileclip [-c] [-p] [-r] [-d] [-v] [file]\n");
  printf("  -c [file]   Copy file or current directory to clipboard\n");
  printf("  -p          Paste copied file or directory into current directory\n");
  printf("  -d          Show clipboard contents\n");
  printf("  -r          Clear clipboard\n");
  printf("  -v          Show version\n");
  printf("  -h          Show this help message\n");
}

/**
 * @brief Copies a file or directory to the clipboard.
 * 
 * Uses `pbcopy` to store the absolute path of the file or directory.
 * 
 * @param arg File or directory to copy (or "null" for current directory)
 * @param cwd Current working directory
 */
void copy(const char *arg, const char *cwd) {
  char path[PATH_MAX];
  char absPath[PATH_MAX];

  // Determine the target path
  if (strcmp(arg, "null") == 0) {
    strcpy(path, cwd);
  } else {
    snprintf(path, sizeof(path), "%s/%s", cwd, arg);
  }

  // Check if file exists
  if (access(path, F_OK) != 0) {
    fprintf(stderr, "fileclip: file does not exist: %s\n", path);
    exit(2);
  }

  // Resolve to absolute path
  if (realpath(path, absPath) == NULL) {
    perror("fileclip: realpath");
    exit(2);
  }

  // Copy path to clipboard
  FILE *fp = popen("pbcopy", "w");
  if (!fp) {
    perror("fileclip: pbcopy");
    exit(2);
  }

  fprintf(fp, "%s", absPath);
  pclose(fp);

  printf("fileclip copied: %s\n", absPath);
}

/**
 * @brief Checks whether the given path is a directory.
 * 
 * @param path Path to check
 * @return int True if directory, false otherwise
 */
int isDir(const char *path) {
  struct stat statbuf;
  return stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode);
}

/**
 * @brief Pastes the copied file or directory into the current directory.
 * 
 * Reads from the clipboard and uses `cp` to perform the paste.
 */
void paste() {
  char path[PATH_MAX];

  // Read path from clipboard
  FILE *fp = popen("pbpaste", "r");
  if (!fp) {
    perror("fileclip: pbpaste");
    exit(2);
  }

  if (!fgets(path, PATH_MAX, fp)) {
    fprintf(stderr, "fileclip: clipboard is empty or unreadable\n");
    pclose(fp);
    exit(1);
  }
  pclose(fp);

  // Remove trailing newline
  path[strcspn(path, "\n")] = '\0';

  // Check that the copied path still exists
  if (access(path, F_OK) != 0) {
    fprintf(stderr, "fileclip: copied path no longer exists: %s\n", path);
    exit(2);
  }

  // Prepare the appropriate cp command
  bool dir = isDir(path);
  char *cmd = malloc(PATH_MAX + 20); // Enough for "cp -R " + path + " ."

  if (!cmd) {
    perror("fileclip: malloc");
    exit(2);
  }

  snprintf(cmd, PATH_MAX + 20, "cp %s \"%s\" .", dir ? "-R" : "", path);

  int result = system(cmd);
  free(cmd);

  if (result != 0) {
    fprintf(stderr, "fileclip: paste failed\n");
    exit(3);
  }

  printf("fileclip pasted: %s\n", path);
}

/**
 * @brief Displays the current contents of the clipboard
 */
void showClipboardContents() {
  char path[PATH_MAX];

  FILE *fp = popen("pbpaste", "r");
  if (!fp) {
    perror("fileclip: pbpaste");
    exit(2);
  }

  if (!fgets(path, PATH_MAX, fp)) {
    fprintf(stderr, "fileclip: clipboard is empty\n");
    pclose(fp);
    return;
  }
  pclose(fp);

  // Remove newline character
  path[strcspn(path, "\n")] = '\0';
  printf("fileclip: %s\n", path);
}

/**
 * @brief Clears the clipboard contents.
 * 
 * Sends an empty string to `pbcopy`.
 */
void clearClipboard() {
  FILE *fp = popen("pbcopy", "w");
  if (fp) {
    fputs("", fp);
    pclose(fp);
  }
}
