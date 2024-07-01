#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

// Definitions and structures
struct currentID {
    int year;
    char branch[8];
    int totalVoters;
};
typedef struct member {
    int cid;
    char cname[20];
    int votes;
} MEMBER;

// GLOBALS
struct currentID currentID; // Stores current Valid user ID parameters
MEMBER memberArray[20]; // Stores information of all members
int numberOfmembers; // Total number of members standing for election
char studentVotes[200]; // Stores information of votes given by each student

// Function prototypes
int extractYear(char userID[15]);
int extractRollNo(char userID[15]);
int checkBranchCode(char userID[15]);
int authenticateAdmin();
void banID();
void creatememberFiles();
void deleteIllegalVote(char userID[15]);
int winnerResult();
void initiateNewElection();
void saveElectionInfoInFile();
void loadElectionInfoFromFile();
// Function implementations (same as provided)
int extractYear(char userID[15]) {
    int year = 0 ;
    for (int i = 0; i < 4; i++) {
        year = (year * 10) + (userID[i] - '0');
    }
    return year;
}

int extractRollNo(char userID[15]) {
    int rollno = 0;
    for (int i = 9; i < 14; i++) {
        rollno = (rollno * 10) + (userID[i] - '0');
    }
    return rollno;
}

int checkBranchCode(char userID[15]) {
    char branchCode[6];
    for (int i = 4; i < 9; i++) {
        branchCode[i-4] = userID[i];
    }
    branchCode[5] = '\0';
    if (strcmp(branchCode, currentID.branch) == 0)
        return 1;
    else
        return 0;
}

int authenticateAdmin() {
    char username[15], password[6];
    
    printf("\nEnter username: ");
    scanf("%s", username);
    if ((strcmp(username, "Kirankumar")) != 0)
        return 0;
    else {
        printf("Enter Password: ");
        for (int i = 0; i < 5; i++) {
            password[i] = getch();
            printf("*");
        }
        password[5] = '\0';
        if ((strcmp(password, "kk980")) != 0)
            return 0;
    }
    return 1;
}

void banID() {
    printf("\nCreating Banned.txt...\n");
    FILE *fp = fopen("Banned.txt", "w");
    if (fp == NULL) {
        printf("Error: Banned.txt not created.\n");
        return;
    }
    printf("Just Enter last roll no to ban\nPress 0 to exit... ");
    int input;
    while (1) {
        printf("\nEnter Number: ");
        scanf("%d", &input);
        if (input == 0)
            break;
        studentVotes[input-1] = '$';
        fprintf(fp, "%d\n", input);
    }
    fclose(fp);
    printf("Created Successfully\n");
}

void createMemberFiles() {
    printf("\nCreating member files...\n");
    FILE *fp;
    char filename[20];
    for (int i = 1; i <= numberOfmembers; i++) {
        sprintf(filename, "member%d.txt", i);
        fp = fopen(filename, "w");
        if (fp == NULL) {
            printf("Error: member file not created.\n");
            return;
        }
        fprintf(fp, "0\n%s", memberArray[i-1].cname);
        fclose(fp);
    }
    printf("Created Files successfully\n");
}

void deleteIllegalVote(char userID[15]) {
    FILE *fp, *fcp;
    char filename[20];
    char line[20];

    int location = extractRollNo(userID);
    sprintf(filename, "member%d.txt", memberArray[studentVotes[location-1]-49].cid);
    memberArray[studentVotes[location-1]-49].votes--;
    studentVotes[location-1] = '0';
    if ((fp = fopen(filename, "r")) == NULL) {    
        printf("\nFile cannot be opened...Operation Failed");
        return;
    }
    printf("\nDeleting in process...\n");    
    if ((fcp = fopen("tmp.txt", "w")) == NULL) {    
        printf("\nFile cannot be opened...Operation Failed");
        fclose(fp);
        return;
    }
    
    while (fgets(line, sizeof(line), fp) != NULL) {
        fprintf(fcp, "%s", line);
    }
    fclose(fp);
    fclose(fcp);
    if ((fp = fopen(filename, "w")) == NULL) {    
        printf("\nFile cannot be opened...Operation Failed");
        return;
    }
    int numFromFile;
    char cnameFromFile[20];
    fcp = fopen("tmp.txt", "r");  
    fscanf(fcp, "%d", &numFromFile);
    fprintf(fp, "%d", numFromFile-1);
    fscanf(fcp, "%s", cnameFromFile);
    fprintf(fp, "\n%s", cnameFromFile);
    while (fscanf(fcp, "%d", &numFromFile) != EOF) {
        if (numFromFile != location)
            fprintf(fp, "\n%d", numFromFile);
    }
    fclose(fp);
    fclose(fcp);
    remove("tmp.txt");
    printf("\nVote deleted successfully\nPress any key to continue...");
    getch();
}

int winnerResult() {
    int maxV = -1;
    int winnerCid;
    for (int i = 0; i < numberOfmembers; i++) {
        if (memberArray[i].votes > maxV) {
            winnerCid = memberArray[i].cid;
            maxV = memberArray[i].votes;      
        } else if (memberArray[i].votes == maxV) {
            return -1;
        }
    }
    return winnerCid;
}

void initiateNewElection() {
    printf("\nNew Election Initiation:\n");
    
    printf("\nElections for which Year: ");
    scanf("%d", &currentID.year);
    printf("Enter branch code: ");
    scanf("%s", currentID.branch);
    printf("Enter max roll no.: ");
    scanf("%d", &currentID.totalVoters);
    printf("Enter the no. of members: ");
    scanf("%d", &numberOfmembers);

    for (int i = 0; i < currentID.totalVoters; i++) {
        studentVotes[i] = '0';
    }

    for (int i = 0; i < numberOfmembers; i++) {
        memberArray[i].cid = i+1;
        printf("Enter name of member %d: ", i+1);
        scanf("%s", memberArray[i].cname);
        memberArray[i].votes = 0;
    }
}

void saveElectionInfoInFile() {
    printf("Saving Election Info in File...\n");
    FILE *fp = fopen("ElectionInfo.txt", "w");
    if (fp == NULL) {
        printf("\nError in file creation\n");
        return;
    }
    fprintf(fp, "%d\n%s\n%d\n%d",
        currentID.year,
        currentID.branch,
        currentID.totalVoters,
        numberOfmembers
    );
    fclose(fp);
    printf("Saved Successfully :)\n");
}

void loadElectionInfoFromFile() {
    FILE *f1, *f2, *f3;
    f1 = fopen("ElectionInfo.txt", "r");
    if (f1 == NULL) {
        printf("Not Exist");
        return;
    }
    fscanf(f1, "%d", &currentID.year);
    fscanf(f1, "%s", currentID.branch);
    fscanf(f1, "%d", &currentID.totalVoters);
    fscanf(f1, "%d", &numberOfmembers);
    fclose(f1);
   
    // Load members info and student votes
    for (int i = 0; i < currentID.totalVoters; i++) {
        studentVotes[i] = '0';
    }
    for (int i = 1; i <= numberOfmembers; i++) {
        int location;
        char filename[20];
        sprintf(filename, "member%d.txt", i);
        f2 = fopen(filename, "r+");
        if (f2 == NULL) {
            printf("Error: member file not found.\n");
            continue;
        }
        memberArray[i-1].cid = i;
        fscanf(f2, "%d", &memberArray[i-1].votes);
        fscanf(f2, "%s", memberArray[i-1].cname);
        while (fscanf(f2, "%d", &location) != EOF) {
            studentVotes[location-1] = i + '0';
        }
        fclose(f2);
    }                                  

    // Load banned votes
    int location;
    f3 = fopen("Banned.txt", "r+");
    if (f3 != NULL) {
        while (fscanf(f3, "%d", &location) != EOF) {
            studentVotes[location-1] = '$';
        }
    }
}
