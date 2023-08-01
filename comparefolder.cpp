#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_FILENAME_LENGTH 256

// Function to compare two files line by line and show differences
void compareFiles(const char *filename1, const char *filename2) {
    FILE *file1 = fopen(filename1, "r");
    FILE *file2 = fopen(filename2, "r");

    if (file1 == NULL || file2 == NULL) {
        printf("Error opening files.\n");
        return;
    }

    char line1[1024], line2[1024];
    int lineNum = 1;
    int differencesFound = 0;

    while (fgets(line1, sizeof(line1), file1) != NULL && fgets(line2, sizeof(line2), file2) != NULL) {
        if (strcmp(line1, line2) != 0) {
            printf("Line %d has been modified:\n", lineNum);
            printf("Version 1: %s", line1);
            printf("Version 2: %s", line2);
            differencesFound = 1;
        }
        lineNum++;
    }

    if (!differencesFound) {
        printf("No differences found between %s and %s.\n", filename1, filename2);
    }

    fclose(file1);
    fclose(file2);
}

// Function to recursively compare two folders
void compareFolders(const char *folder1, const char *folder2) {
    DIR *dir1 = opendir(folder1);
    DIR *dir2 = opendir(folder2);

    if (dir1 == NULL || dir2 == NULL) {
        printf("Error opening folders.\n");
        return;
    }

    struct dirent *entry;
    char filename1[MAX_FILENAME_LENGTH], filename2[MAX_FILENAME_LENGTH];

    // Arrays to store filenames in version1 and version2
    char *fileArray1[MAX_FILENAME_LENGTH];
    char *fileArray2[MAX_FILENAME_LENGTH];
    int fileCount1 = 0;
    int fileCount2 = 0;

    // Read filenames from version1
    while ((entry = readdir(dir1)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(filename1, MAX_FILENAME_LENGTH, "%s/%s", folder1, entry->d_name);
        struct stat stat1;
        if (stat(filename1, &stat1) != 0) {
            printf("Error getting file stats for %s.\n", filename1);
            continue;
        }

        if (S_ISREG(stat1.st_mode)) {
            fileArray1[fileCount1] = strdup(entry->d_name);
            fileCount1++;
        }
    }

    // Read filenames from version2
    while ((entry = readdir(dir2)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(filename2, MAX_FILENAME_LENGTH, "%s/%s", folder2, entry->d_name);
        struct stat stat2;
        if (stat(filename2, &stat2) != 0) {
            printf("Error getting file stats for %s.\n", filename2);
            continue;
        }

        if (S_ISREG(stat2.st_mode)) {
            fileArray2[fileCount2] = strdup(entry->d_name);
            fileCount2++;
        }
    }

    // Compare files in version1 and version2
    for (int i = 0; i < fileCount1; i++) {
        for (int j = 0; j < fileCount2; j++) {
            if (strcmp(fileArray1[i], fileArray2[j]) == 0) {
                snprintf(filename1, MAX_FILENAME_LENGTH, "%s/%s", folder1, fileArray1[i]);
                snprintf(filename2, MAX_FILENAME_LENGTH, "%s/%s", folder2, fileArray2[j]);
                compareFiles(filename1, filename2);
                break;
            }
        }
    }

    // Check for files in version2 that are not present in version1
    for (int i = 0; i < fileCount2; i++) {
        int found = 0;
        for (int j = 0; j < fileCount1; j++) {
            if (strcmp(fileArray2[i], fileArray1[j]) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("File %s is present in version2 but not in version1.\n", fileArray2[i]);
        }
    }

    // Check for files in version1 that are not present in version2
    for (int i = 0; i < fileCount1; i++) {
        int found = 0;
        for (int j = 0; j < fileCount2; j++) {
            if (strcmp(fileArray1[i], fileArray2[j]) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("File %s is present in version1 but not in version2.\n", fileArray1[i]);
        }
    }

    // Free allocated memory
    for (int i = 0; i < fileCount1; i++) {
        free(fileArray1[i]);
    }
    for (int i = 0; i < fileCount2; i++) {
        free(fileArray2[i]);
    }

    closedir(dir1);
    closedir(dir2);
}

int main() {
    const char folder1[] = "version1";
    const char folder2[] = "version2";

    compareFolders(folder1, folder2);

    return 0;
}
