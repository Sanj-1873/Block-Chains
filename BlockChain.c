//#include "crypto.h"
#include <string.h>
#include <stdlib.h>
#include <openssl/crypto.h>
#include <stdio.h>

#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <limits.h>
//#define SHA256_DIGEST_LENGTH 32

struct block
{
    unsigned char prevHash[SHA256_DIGEST_LENGTH]; //unsigned char array
    int blockData;
    struct block *link;
} * head;
//SHA256(sre,size,dest)
//src and dest are of unsigned char *
void addBlock(int);
void verifyChain();
void alterNthBlock(int, int);
void hackChain();
int hashCompare(unsigned char *, unsigned char *);
void hashPrinter();
unsigned char *toString(struct block);
void printBlock();
void printAllBlocks();

void addBlock(int data)
{
    if (head == NULL)
    {
        head = malloc(sizeof(struct block));
        SHA256("", sizeof(""), head->prevHash);
        head->blockData = data;
        return;
    }
    struct block *currentBlock = head;
    while (currentBlock->link)
        currentBlock = currentBlock->link;
    struct block *newBlock = malloc(sizeof(struct block));
    currentBlock->link = newBlock;
    newBlock->blockData = data;
    SHA256(toString(*currentBlock), sizeof(*currentBlock), newBlock->prevHash);
}

void verifyChain()
{
    if (head == NULL)
    {
        printf("BlockChain is empty Please try again after adding in some blocks");
        return;
    }
    struct block *curr = head->link, *prev = head;
    int count = 0;
    while (curr)
    {
        printf("%d\t[%d]\t", count++, curr->blockData);
        hashPrinter(SHA256(toString(*prev), sizeof(*prev), NULL), SHA256_DIGEST_LENGTH);
        printf(" - ");
        hashPrinter(curr->prevHash, SHA256_DIGEST_LENGTH);
        if (hashCompare(SHA256(toString(*prev), sizeof(*prev), NULL), curr->prevHash))
            printf("\nVerified!");
        else
        {
            printf("Verification Failed!\n");
        }
        prev = curr;
        curr = curr->link;
    }
}
void alterNthBlock(int n, int newData)
{
    struct block *curr = head;
    int count = 0;
    if (curr == NULL)
    {
        printf("Nth block does not exist!\n");
        return;
    }
    while (count != n)
    {
        if (curr->link == NULL && count != n)
        {
            printf("Nth block does not exit\n ");
            return;
        }
        else if (count == n)
            break;
        curr = curr->link;
        count++;
    }
    printf("Before: ");
    printBlock(curr);
    curr->blockData = newData;
    printf("\nAfter: ");
    printBlock(curr);
    printf("\n");
}

void hackChain()
{
    struct block *curr = head, *prev;
    if (curr == NULL)
    {
        printf("BlockChain is empty\n");
        return;
    }
    while (1)
    {
        prev = curr;
        curr = curr->link;
        if (curr == NULL)
            return;
        if (!hashCompare(SHA256(toString(*prev), sizeof(*prev), NULL), curr->prevHash))
        {
            hashPrinter(
                SHA256(toString(*prev), sizeof(*prev), curr->prevHash),
                SHA256_DIGEST_LENGTH);
            printf("\n");
        }
    }
}

unsigned char *toString(struct block b)
{
    unsigned char *str = malloc(sizeof(unsigned char) * sizeof(b));
    memcpy(str, &b, sizeof(b));
    return str;
}

void hashPrinter(unsigned char hash[], int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%02x", hash[i]); //printing the hex number
    }
}
int hashCompare(unsigned char *str1, unsigned char *str2)
{
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        if (str1[i] != str2[i])
            return 0;
        return 1;
    }
}

void printBlock(struct block *b)
{
    printf("%p]t", b);
    hashPrinter(b->prevHash, sizeof(b->prevHash));
    printf("\t[%d]\t", b->blockData);
    printf("%p\n", b->link);
}

void printAllBlocks()
{
    struct block *curr = head;
    while (curr->link)
    {
        printBlock(curr->link);
        curr = curr->link;
    }
}

int main()
{
    int c = 1;
    int n = 0;
    int r = 0;
    printf("1) AddBLock\n 2)add n random blocks\n 3)alterNthBlock\n 4)PrintAllBLocks\n 5) verifyChain\n 6)hackChain\n");
    printf("Choice: ");
    scanf("%d", &c);
    while (c != 0)
    {
        // printf("At the start: c=%d\n", c);

        switch (c)
        {
        case 1:
        {
            printf("Enter data: ");
            // scanf("%d", &n);
            int num;
            char term;
            if (scanf("%d%c", &num, &term) != 2 || term != '\n')
            {
                printf("failure\n");
                exit(1);
            }
            addBlock(num);
            break;
        }

        case 2:
        {
            printf("How many blocks do you want to enter?: ");
            int num;
            char term;
            if (scanf("%d%c", &num, &term) != 2 || term != '\n')
            {
                printf("failure\n");
                exit(1);
            }
            for (int i = 0; i < num; i++)
            {
                r = rand() % (num * 10);
                printf("Entering: %d\n", r);
                addBlock(r);
            }
            break;
        }

        case 3:
        {
            printf("Which block do you want to alter?\n");
            scanf("%d", &n);
            printf("Enter value: ");
            scanf("%d", &r);
            alterNthBlock(n, r);
            break;
        }

        case 4:
        {
            printAllBlocks();
            break;
        }

        case 5:
        {
            verifyChain();
            break;
        }

        case 6:
        {
            hackChain();
            break;
        }

        default:
        {
            printf("Wrong choice!\n");
            exit(1);
        }
        }
        printf("Choice: ");
        scanf("%d", &c);
    }
    return 0;
}