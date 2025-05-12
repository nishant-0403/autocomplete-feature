#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define PROMPT_COUNT 10

typedef struct TrieNode { // Structure of a Trie node
	bool isValidWord;
	struct TrieNode *children[26];
}TrieNode;

typedef struct Trie { // Structure for the Trie data structure
	TrieNode *root;
}Trie;

typedef struct SQNode { // Structure of a Staq node
	char character;
	struct SQNode *next;
	struct SQNode *prev;
}SQNode;

typedef struct Stack {
	TrieNode *arr[8];
	int top;
	int size;
}Stack;

typedef struct Staq { // Structure to facilitate deletion from both ends (front and rear), and insertion from the rear end
	SQNode *front;
	SQNode *rear;
}Staq;

typedef struct Prompts { // Structure to store the generated prompts
	char *promptArr[PROMPT_COUNT];
	int indexOfLastPrompt;
	int lengthArray[PROMPT_COUNT];
}Prompts;

void initStack(Stack *st);

void push(Stack *st, TrieNode *t);

TrieNode* pop(Stack *st);

void printPrompts();

void printSelectedPrompt(int digit);

void freeStaq();

int isEmpty();

void initStaq();

void printStaq();

void freePrompts();

void storePrompts(char *arr, int len);

void initPrompts();

void initTrie(Trie *t);

void pushStaq(char ch);

void popStaq();

char deStaq();

int findLength();

void givePrompt(char *arr, int len);

void helperFunction();

void createNode(TrieNode **current, char ch);

void readyTrie(Trie *t, FILE *fp);

void freeSubTrie(TrieNode *t);

void traverse(TrieNode *t, int indexParent);

void destroy(Trie *t);
