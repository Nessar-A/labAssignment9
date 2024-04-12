#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
};

// Define a node for linked list in each hash bucket
struct HashNode
{
    struct RecordType record;
    struct HashNode *next;
};

// Define the hash table
struct HashType
{
    struct HashNode *bucket;
};

// Compute the hash function
int hash(int x)
{
    return x % 23; // Adjusted the hash function to fit within the range of 0 to 30
}

// parses input file to an array of RecordType
int parseData(char *inputFileName, struct RecordType **ppData)
{
    FILE *inFile = fopen(inputFileName, "r");
    if (inFile == NULL)
    {
        printf("Error opening file.\n");
        exit(EXIT_FAILURE);
    }

    int dataSz;
    fscanf(inFile, "%d\n", &dataSz);

    *ppData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);
    if (*ppData == NULL)
    {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < dataSz; ++i)
    {
        fscanf(inFile, "%d %c %d", &(*ppData)[i].id, &(*ppData)[i].name, &(*ppData)[i].order);
    }

    fclose(inFile);

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    printf("Records:\n");
    for (int i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
}

// display records in the hash structure
void displayRecordsInHash(struct HashType *pHashArray[], int hashSz)
{
    for (int i = 0; i < hashSz; ++i)
    {
        if (pHashArray[i] != NULL)
        {
            printf("Index %d -> ", i);
            struct HashNode *current = pHashArray[i]->bucket;
            while (current != NULL)
            {
                printf("%d %c %d", current->record.id, current->record.name, current->record.order);
                current = current->next;

                if (current != NULL) 
                    printf("-> ");
                else
                    break;
            }
            printf("\n");
        }
    }
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    // Parse data from file
    recordSz = parseData("input_lab_9.txt", &pRecords);

    // Print parsed records
    printRecords(pRecords, recordSz);

    // Initialize hash table
    struct HashType *hashTable[23] = {NULL}; // Array of pointers to HashType
    for (int i = 0; i < recordSz; ++i)
    {
        int index = hash(pRecords[i].id);
        if (hashTable[index] == NULL)
        {
            hashTable[index] = (struct HashType *)malloc(sizeof(struct HashType));
            hashTable[index]->bucket = NULL;
        }
        struct HashNode *newNode = (struct HashNode *)malloc(sizeof(struct HashNode));
        newNode->record = pRecords[i];
        newNode->next = hashTable[index]->bucket;
        hashTable[index]->bucket = newNode;
    }

    // Display records in the hash table
    displayRecordsInHash(hashTable, 23);

    // Free allocated memory
    for (int i = 0; i < 23; ++i)
    {
        if (hashTable[i] != NULL)
        {
            struct HashNode *current = hashTable[i]->bucket;
            while (current != NULL)
            {
                struct HashNode *temp = current;
                current = current->next;
                free(temp);
            }
            free(hashTable[i]);
        }
    }
    free(pRecords);

    return 0;
}
