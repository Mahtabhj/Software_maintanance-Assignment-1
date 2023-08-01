#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

// Function to compare two Java files and detect changes
void compareFiles(FILE *file1, FILE *file2) {
    char line1[MAX_LINE_LENGTH], line2[MAX_LINE_LENGTH];
    int lineNum = 1;

    while (fgets(line1, MAX_LINE_LENGTH, file1) && fgets(line2, MAX_LINE_LENGTH, file2)) {
        if (strcmp(line1, line2) != 0) {
            printf("Line %d has been modified:\n", lineNum);
            printf("Version 1: %s", line1);
            printf("Version 2: %s", line2);
        }
        lineNum++;
    }

    // If one file has more lines than the other
    while (fgets(line1, MAX_LINE_LENGTH, file1)) {
        printf("Version 1: Line %d has been removed:\n", lineNum);
        printf("Version 1: %s", line1);
        lineNum++;
    }
    while (fgets(line2, MAX_LINE_LENGTH, file2)) {
        printf("Version 2: Line %d has been added:\n", lineNum);
        printf("Version 2: %s", line2);
        lineNum++;
    }
}

int main() {
    FILE *file1, *file2;
    char filename1[] = "version1.java";
    char filename2[] = "version2.java";

    file1 = fopen(filename1, "r");
    file2 = fopen(filename2, "r");

    if (file1 == NULL || file2 == NULL) {
        printf("Error opening files.\n");
        return 1;
    }

    compareFiles(file1, file2);

    fclose(file1);
    fclose(file2);

    return 0;
}

