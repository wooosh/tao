// Standard Libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
// External Libraries
#include <magic.h>

typedef struct KVPair {
  char* key;
  char* val;
  FILE* file;
} KVPair;

KVPair initParser(char* filename) {
  return (KVPair){malloc(1), malloc(1), fopen(filename, "r")};
};

int endswith(char* str, char* substr, int dir) {
    if (dir)
      return strncmp(str, substr, strlen(substr)) == 0;
    else {
      size_t slen = strlen(str);
      size_t sublen = strlen(substr);
      return (slen >= sublen) && (memcmp(substr, str + slen  - sublen, sublen) == 0);
    }
}

char* stripstr(char* str, int direction, char delimeter) {
  size_t len = strlen(str);
  int index = direction ? 0 : len - 1;
  while (0 <= index && index <= len && str[index] == delimeter)
    direction ? index++ : index--;
  
  if (!direction)
    str[index + 1] = '\0';
  else
    memmove(str, str + index, len - index + 1);
  return str;
}

void nextPair(KVPair* pair) {
  char* line = NULL;
  size_t len = 0;
  // Find a line that isn't a comment or blank line until we hit the end of the file
  while (line == NULL || line[0] == '#' || line[0] == '\n')
    if (getline(&line, &len, pair -> file) == -1) {
      fclose(pair -> file);
      pair -> file = NULL;
      free(line);
      return;
    }
  len = strlen(line);
  //  Remove newline from getline
  if (line[len-1] == '\n')
    line[len-1] = '\0';
  
  // If the line does not contain a "=" mark it as an invalid line
  if (strchr(line, '=') == NULL) {
    printf("Invalid Configuration:\n '%s'", line);
    exit(EXIT_FAILURE);
  }
  
  char* key = stripstr(strtok(line, "="), 0, ' ');
  char* val = stripstr(strtok(NULL, ""), 1, ' ');
   
  // Allocate more memory if the structure members are not large enough
  if (sizeof(pair -> key) < strlen(key) + 1) 
    pair -> key = realloc(pair -> key, strlen(key) + 1);
  if (sizeof(pair -> val) < strlen(val) + 1)
    pair -> val = realloc(pair -> val, strlen(val) + 1);
  
  strcpy(pair -> key, key);
  strcpy(pair -> val, val);
  free(line);
};

void cleanKV(KVPair* pair) {
  free(pair -> key);
  free(pair -> val);
  if (pair -> file) {
    fclose(pair -> file);
  }
}

void runCommand(const char* file, KVPair config, struct magic_set* magic) {
}

int main(int argc, char **argv) {
  // Make sure we get an argument
  if (argc < 2) {
    printf("Not enough arguments\n");
    return EXIT_FAILURE;
  }
  const char* file = argv[1];
  
  if (access(file, R_OK) == -1) {
    perror("Cannot read file");
    return EXIT_FAILURE;
  }

  // Get the files mime type
  struct magic_set* magic = magic_open(MAGIC_MIME_TYPE | MAGIC_CHECK);
  magic_load(magic, NULL);
  const char* mimetype = magic_file(magic, file);
  
  // Get the program associated with the mime type from the config file
  char* configLocation = malloc(strlen(getenv("HOME")) + strlen("/.config/tao.conf") + 1);
  sprintf(configLocation, "%s/.config/tao.conf", getenv("HOME"));
  KVPair config = initParser(configLocation);
  free(configLocation);

  // If the file is not found, throw an error and exit
  if (config.file == NULL) {
    printf("Cannot locate config file at '~/.config/tao.conf'\n");
    magic_close(magic);
    return EXIT_FAILURE;
  }
  
  char ptype[5];

  for (;;) {
    nextPair(&config);
    if (strchr(config.key, ':') == NULL) {
      printf("Missing colon in key '%s'", config.key);
      continue;
    }
    
    // TODO: Stop reallocating for every rule 
    char* kdata = strdup(config.key);
    strncpy(ptype, strtok(kdata, ":"), 5);
    strcpy(kdata, strtok(NULL, ""));
    if (
        (strncmp(ptype, "mime", 4) == 0 && strcmp(kdata, mimetype) == 0) ||
        ((strncmp(ptype, "ends", 4) == 0) && endswith((char*)file, kdata, 0))   ||
        ((strncmp(ptype, "strt", 4) == 0) && endswith((char*)file, kdata, 1))
    ) {
      // Run the associated program with the filename as an argument
      char* cmd = malloc(strlen(config.val) + strlen(file) + 2);
      sprintf(cmd, "%s %s", config.val, file);
      system(cmd);
      free(cmd);
      free(kdata);
      cleanKV(&config);
      magic_close(magic);
      exit(EXIT_SUCCESS);
    }

    free(kdata);
    if (config.file == NULL) 
      break;
  }
  
  printf("File does not match any patterns", mimetype);
  // Cleanup and exit
  cleanKV(&config);
  magic_close(magic);
  return EXIT_FAILURE;
}
