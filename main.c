#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_CHAR 256
int countBoyer= 0;
int countHorspool= 0;
int comparisonHorspool =0;
int comparisonBoyer = 0;

#define MAX_FILE_SIZE 1048576
#define MAX_PATTERN_SIZE 256
#define MAX_PATTERNS 10

void BruteForce(char* text, char* pattern, char* output) {
    int M = strlen(pattern);
    int N = strlen(text);
    int occurence = 0;
    int comparison = 0;
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    FILE *fp = fopen(output, "w");
    if (fp == NULL) {
        printf("Could not open file %s", output);
        return;
    }

    for (int i = 0; i <= N - M; i++) {
        int j;
        for (j = 0; j < M; j++) {
            comparison++;
            if (text[i + j] != pattern[j])
                break;
        }

        if (j == M) { // pattern found
            occurence++;
            fprintf(fp, "<mark>%s</mark>", pattern);
            i = i + M - 1;
        } else {
            fputc(text[i], fp);
        }
    }
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("----- Brute Force algorithm -----\n\n");
  printf("Pattern => %s\n", pattern);
    printf("Number of character comparisons: %d\n", comparison);
    printf("Number of occurrences: %d\n", occurence);
    printf("Time taken: %f ms\n\n", cpu_time_used*1000);
    fclose(fp);
}




void markPattern(char file[], long start, long end, long n) {
   // long sizeofFile = strlen(file) ;
    char *temp = malloc(n *sizeof(char));
   // char temp[10000000];
    strcpy(temp, "<mark>");
    strncat(temp, file + start, end - start + 1);
    strcat(temp, "</mark>");
    
    char *remaining = malloc(n *sizeof(char));
  // char remaining[10000000];
    strcpy(remaining, file + end + 1);

    strcpy(file + start, temp);
    strcat(file, remaining);
     free(temp);
     free(remaining);
}

// void markPattern(char *file, long start, long end, long n) {
//     int pattern_length = end - start + 1;

//     char *start_tag = "<mark>";
//     char *end_tag = "</mark>";

//     long start_tag_length = strlen(start_tag);
//     long end_tag_length = strlen(end_tag);

//     long file_length = strlen(file);
//     long new_length = file_length + start_tag_length + end_tag_length;

//     char *temp = (char *)malloc(new_length + 1);
//     if (temp == NULL) {
//         printf("Memory allocation failed.\n");
//         return;
//     }

//     strncpy(temp, file, start);
//     strncpy(temp + start, start_tag, start_tag_length);
//     strncpy(temp + start + start_tag_length, file + start, pattern_length);
//     strncpy(temp + start + start_tag_length + pattern_length, end_tag, end_tag_length);
//     strcpy(temp + start + start_tag_length + pattern_length + end_tag_length, file + end + 1);

//     strcpy(file, temp);

//     free(temp);
// }
// void markPattern(char *file, long start, long end, long n) {
//     const char *start_tag = "<mark>";
//     const char *end_tag = "</mark>";
//     const size_t start_tag_length = strlen(start_tag);
//     const size_t end_tag_length = strlen(end_tag);

//     const size_t pattern_length = end - start + 1;
//     const size_t file_length = strlen(file);

//     const size_t new_length = file_length + start_tag_length + end_tag_length;

//     char *temp = (char *)malloc(new_length + 1);
//     if (temp == NULL) {
//         printf("Memory allocation failed.\n");
//         return;
//     }

//     memcpy(temp, file, start);
//     memcpy(temp + start, start_tag, start_tag_length);
//     memcpy(temp + start + start_tag_length, file + start, pattern_length);
//     memcpy(temp + start + start_tag_length + pattern_length, end_tag, end_tag_length);
//     memcpy(temp + start + start_tag_length + pattern_length + end_tag_length,
//            file + end + 1,
//            file_length - end - 1);

//     temp[new_length] = '\0';
//     strcpy(file, temp);

//     free(temp);
// }

void shiftTables(char pattern[], int patternLength, int shiftTable[]) {
    for (int i = 0; i < MAX_CHAR; i++) {
        shiftTable[i] = patternLength;
    }

    for (int i = 0; i < patternLength - 1; i++) {
        shiftTable[pattern[i]] = patternLength - 1 - i;
        
    }
    printf("Shift Table\n");
    printf(" Pattern => %s\n", pattern);
    for (int i = 0; i < MAX_CHAR; i++) {
        for (int j = 0; j < patternLength; j++) {
            
            if( (char)i ==  pattern[j]){
                printf("  (%c-", i);
                printf("%d)", shiftTable[i]);
                printf("\n");
                break;
            }
        }
       
    }
    printf("  (Other characters-%d)", patternLength);
    printf("\n");
}


void horspoolSearch(char text[], long textLength, char pattern[], long patternLength) {
    int shiftTable[MAX_CHAR];
    shiftTables(pattern, patternLength, shiftTable);
    long n = strlen(text);
    long i = patternLength - 1;
    while (i < textLength) {
        long j = patternLength - 1;
        long k = i;

        while (j >= 0 && text[k] == pattern[j]) {
            comparisonHorspool++;
            k--;
            j--;
        }

        if (j == -1) {
            countHorspool++;
         //   printf("Desen '%s' metinde bulundu, baslangic indeksi: %d\n", pattern, k + 1);
            markPattern(text,k+1,k+patternLength,n);
            k +=13;
            i +=13;
            textLength +=13;
            i += patternLength;
        } else {
            comparisonHorspool++;
            i += shiftTable[text[i]];
        }
    }
}

void searchPatterns(char text[], long textLength, char patterns[][100], long numPatterns) {
    
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    
    printf("----- Horspool’s algorithm -----\n");
    
    for (int i = 0; i < numPatterns; i++) {
        
        char* pattern = patterns[i];
        long patternLength = strlen(pattern);
        horspoolSearch(text, textLength, pattern, patternLength);
    }
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    printf("Number of character comparisons = %d\n", comparisonHorspool);
    printf("Number of occurrences = %d\n", countHorspool);
    printf("Time taken: %f ms\n\n", cpu_time_used*1000);
}


long max(long a, long b) {
    return (a > b) ? a : b;
}
void shiftTable(char *pattern, int *shift) {
    
    int m = strlen(pattern);
    int i, j;
    
    for(i=0; i<MAX_CHAR; i++) {
        shift[i] = m;
    }
    
    for(j=0; j<m-1; j++) {
        shift[pattern[j]] = m-j-1;
    }
    
    printf("Bad Symbol Table\n");
    printf(" Pattern => %s\n", pattern);
    for (int i = 0; i < MAX_CHAR; i++) {
        for (int j = 0; j < m; j++) {
            
            if( (char)i ==  pattern[j]){
                printf("  (%c-", i);
                printf("%d)", shift[i]);
                printf("\n");
                break;
            }
        }
       
    }
    printf("  (Other characters-%d)", m);
    printf("\n");
}

void badCharTable(char *pattern, int *badChar) {
    long m = strlen(pattern);
    long i;
    
    for(i=0; i<MAX_CHAR; i++) {
        badChar[i] = -1;
    }
    
    for(i=0; i<m; i++) {
        badChar[(int)pattern[i]] = i;
    }
}

void suffixes(char *pattern, long *suff) {
    long m = strlen(pattern);
    long f, g, i;
    
    suff[m-1] = m;
    g = m-1;
    
    for(i=m-2; i>=0; i--) {
        if(i > g && suff[i+m-1-f] < i-g) {
            suff[i] = suff[i+m-1-f];
        } else {
            if(i < g) {
                g = i;
            }
            f = i;
            while(g >= 0 && pattern[g] == pattern[g+m-1-f]) {
                g--;
            }
            suff[i] = f-g;
        }
    }
}

void goodSuffixTable(char *pattern, long *goodSuff) {// orjinal
    long m = strlen(pattern);
    long i, j;
    long suff[m];

    suffixes(pattern, suff);

    for(i=0; i<m; i++) {
        goodSuff[i] = m;
    }

    j = 0;
    for(i=m-1; i>=-1; i--) {
        if(i == -1 || suff[i] == i+1) {
            for(; j<m-1-i; j++) {
                if(goodSuff[j] == m) {
                    goodSuff[j] = m-1-i;
                }
            }
        }
    }

    for(long i=0; i<=m-2; i++) {
        goodSuff[m-1-suff[i]] = m-1-i;
    }

    int k = 1;
    for (long i = m-2; i >= 0 ; i--) {
        printf("  k=%d => ", k);
        printf("%d\n", goodSuff[i]);
        k++;
    }
}


void boyerMooreSearch(char *text, char *pattern) {
    int patternLength = strlen(pattern);
    long n = strlen(text);
    long m = strlen(pattern);

    // shift table oluşturma
    int shift[MAX_CHAR];
    shiftTable(pattern, shift);

    // bad character table oluşturma
    int badChar[MAX_CHAR];
    badCharTable(pattern,badChar);

    // good suffix table oluşturma
    long *suff = (long*)malloc(sizeof(long) * (m + 1));
    long *goodSuff = (long*)malloc(sizeof(long) * (m + 1));
    long *preSuff = (long*)malloc(sizeof(long) * (m + 1));
    suffixes(pattern,suff);
    printf("Good Suffix Table\n");
    printf(" Pattern => %s\n", pattern);
    goodSuffixTable(pattern,goodSuff);

    long j = 0; // textteki index
    while (j <= n - m) {
        long i = m - 1; // patterndeki index

        while (i >= 0 && pattern[i] == text[i + j]) {
                  comparisonBoyer++;
                  i--;
              }

        if (i < 0) { // eşleşme bulundu
            countBoyer++;
           // printf("Eşleşme bulundu: %d\n", j);
            markPattern(text, j, j+patternLength-1, n);
            j = j + 13;
            i = i + 13;
            n = n + 13;
            j += goodSuff[0];
        } else { // eşleşme yok
            comparisonBoyer++;
            long bcShift = i - badChar[(unsigned char)text[i+j]];
            long gsShift = 0;
            if (i < m - 1) { // good suffix varsa

                gsShift = goodSuff[i+1];
            }
            j += max(bcShift, gsShift);
        }
    }

    free(suff);
    free(goodSuff);
    free(preSuff);


}


void searchPatternsBoyer_Moore(char text[], char patterns[][100], int numPatterns) {
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    
    printf("----- Boyer-Moore algorithm -----\n");
    int i;
    for (i = 0; i < numPatterns; i++) {
        char* pattern = patterns[i];
        boyerMooreSearch(text, pattern);
    }
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    printf("Number of character comparisons = %d\n", comparisonBoyer);
    printf("Number of occurrences = %d\n", countBoyer);
    printf("Time taken: %f ms\n\n", cpu_time_used*1000);
}

int main() {
    char* output = "output.html"; 
  char patterns[][100] = {"AT_THAT"};
    
    
    int numPatterns = sizeof(patterns) / sizeof(patterns[0]);
    FILE *fp = fopen("input.html", "r");
  
  if (fp == NULL) {
    printf("Could not open file");
    return 1;
  }
 

  fseek(fp, 0, SEEK_END);
  long sizeOfFile = ftell(fp);
  fseek(fp, 0, SEEK_SET);

    char *file1 = malloc(sizeOfFile * sizeof(char));
      char character1;
      long i = 0;
      while ((character1 = fgetc(fp)) != EOF) {
        file1[i] = character1;
        i++;
      }
      file1[i] = '\0';
    
    
    char *file2 = malloc(sizeOfFile * sizeof(char));
      for(int j =0;j<sizeOfFile;j++){
            if(file1[j]=='\0'){
                file2[j]='\0';
                break;
            }
            file2[j]=file1[j];
      }
  char *file3 = malloc(sizeOfFile * sizeof(char));
      for(int j =0;j<sizeOfFile;j++){
            if(file1[j]=='\0'){
                file3[j]='\0';
                break;
            }
            file3[j]=file1[j];
      }
    
  

       for (int i = 0; i < numPatterns; i++) {
        if (strlen(patterns[i]) == 0) {
            break;
        }
        BruteForce(file3, patterns[i], output);
    }
   searchPatternsBoyer_Moore(file2, patterns, numPatterns);
   searchPatterns(file1, sizeOfFile, patterns, numPatterns);

       
    
    // long size = strlen(file1);

    //     FILE *outputFile = fopen("output2.html", "w");
    //     if (outputFile == NULL) {
    //         printf("Dosya oluşturulamadı.");
    //         return 1;
    //     }

    //     for (long i = 0; i < size; i++) {
    //         fprintf(outputFile, "%c", file1[i]);
    //     }

    //     FILE *outputFile3 = fopen("output3.html", "w");
    //     if (outputFile3 == NULL) {
    //         printf("Dosya oluşturulamadı.");
    //         return 1;
    //     }

    //     for (long i = 0; i < size; i++) {
    //         fprintf(outputFile3, "%c", file2[i]);
    //     }
        
       

    // for (i =0;i<sizeOfFile+1000;i++){
    //         if(file1[i]=='\0'){
    //             break;
    //         }else{
    //            printf("%c",file1[i]);
    //         }

    //       }
    // for (i =0;i<sizeOfFile+1000;i++){
    //         if(file2[i]=='\0'){
    //             break;
    //         }else{
    //            printf("%c",file2[i]);
    //         }

    //       }
    
    //fclose(outputFile);
      fclose(fp);
      free(file1);
      free(file2);
}



