#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "autocomplete.h"

Prompts prompts;

Staq sq;

void initStack(Stack *st) { //to initialise the stack
	st->top = -1;
	st->size = 8;
}

void push(Stack *st, TrieNode *t) { //to push a TrieNode* pointer onto the stack	
	if(st->top < st->size) {
		st->top++;
		st->arr[st->top] = t;
	}
}

TrieNode* pop(Stack *st) { //to pop the top of stack
	if(st->top == -1)
		return NULL;
	TrieNode *t = st->arr[st->top];
	st->top--;
	return t;
}

void printPrompts() { //to print the prompts stored in the global object of type Prompts

	for(int i = 0; i <= prompts.indexOfLastPrompt; i++) {
		printf("%d - ", i);
		
		for(int j = 0; j < prompts.lengthArray[i]; j++) {
			printf("%c", prompts.promptArr[i][j]);
		}
		
		printf("\n");
	}
}

void printSelectedPrompt(int digit) { // To complete word partially entered by user
	int i = 0;
	char ch;
	int index = digit - 48;
	
	for(i = 0; i != prompts.lengthArray[index]; i++) {
		ch = (prompts.promptArr[index])[i];
		printf("%c", ch);
	}
	printf("\n");
}

int isEmpty() { //to check if staq is empty
	if(sq.front == NULL) {
		return 1;
	}
	else {
		return 0;
	}
}

void initPrompts() { //to initialise the object of type Prompts
	prompts.indexOfLastPrompt = -1;
	prompts.promptArr[0] = NULL;
}

void freePrompts() { // To free the global structure 'prompts' after every character
	prompts.indexOfLastPrompt = -1;
	
	for(int i = 0; i < PROMPT_COUNT; i++) {
		free(prompts.promptArr[i]);
		prompts.promptArr[i] = NULL;
		prompts.lengthArray[i] = 0;
	}
}

void initTrie(Trie *t) { //to initialise the object of type Trie
	t->root = NULL;
}

void initStaq() { //to initialise staq
	sq.front = sq.rear = NULL;
}

void freeStaq() { //to free staq
	while(!isEmpty()) {
		popStaq();
	}
}

void pushStaq(char ch) { //to push an SQNode on the staq (rear end)
	SQNode *t;
	t = (SQNode *)malloc(sizeof(SQNode));
	t->character = ch;
	t->next = NULL;
	
	if((sq.front == NULL) && (sq.rear == NULL)) {
		sq.front = sq.rear = t;
		t->prev = NULL;
	}
	
	else {
		sq.rear->next = t;
		t->prev = sq.rear;
		sq.rear = t;
	}
} 

void popStaq() { //to pop the top of staq structure (rear end)
	if(isEmpty()) {
		return;
	}
	
	if(!sq.rear->prev) {
		free(sq.rear);
		sq.front = sq.rear = NULL;
		return;
	}
	
	sq.rear = sq.rear->prev;
	free(sq.rear->next);
	sq.rear->next = NULL;	
}

char deStaq() { //to dequeue the front of staq structure (front end)
	if(isEmpty()) {
		return '\0';
	}
	char x = sq.front->character;
	
	if(!sq.front->next) {
		free(sq.front);
		sq.front = sq.rear = NULL;
		return x;
	}
	
	sq.front = sq.front->next;
	free(sq.front->prev);
	sq.front->prev = NULL;
	return x;
}

int findLength() { //to find length of the resultant string stored in the staq
	int count = 1;
	SQNode *p = sq.front;
	
	while(p->next) {
		count++;
		p = p->next;
	}
	
	return count;
}

void storePrompts(char *arr, int len) { // To store the generated prompts
	prompts.indexOfLastPrompt++;
	prompts.promptArr[prompts.indexOfLastPrompt] = (char *)malloc(sizeof(char) * (len));
	prompts.lengthArray[prompts.indexOfLastPrompt] = len;
	int i = 0;
	
	while(i < len) {
		prompts.promptArr[prompts.indexOfLastPrompt][i] = arr[i];
		i++;
	}
}

void givePrompt(char *arr, int len) { // To print the generated prompts
	storePrompts(arr, len);
	printf("%d - ", prompts.indexOfLastPrompt);
	int i = 0;
	
	while(i < len) {
		printf("%c", arr[i]);
		i++;
	}
	
	printf("\n");
}

void helperFunction() { //gets called in the recursive traverse(), and calls givePrompt() to give 1 prompt for each call
	int i = 0;
	int len = findLength();
	char tempArr[len];
	
	while(i < len) {
		tempArr[i] = deStaq();
		i++;
	}
	
	givePrompt(tempArr, len);
	i = 0;
	
	while(i < (len - 1)) {
		pushStaq(tempArr[i]);
		i++;
	}
}

void createNode(TrieNode **current, char ch) { // To create one node of the Trie structure
	int indexOfInput = ((int)ch - 97);
	int i = 0;
	
	if((*current)->children[indexOfInput] != NULL) {
		(*current) = (*current)->children[indexOfInput];
		return;
	}
	
	(*current)->children[indexOfInput] = (TrieNode *)malloc(sizeof(TrieNode));
	(*current) = (*current)->children[indexOfInput];
	
	while(i < 26) {
		(*current)->children[i] = NULL;
		i++;
	}
	(*current)->isValidWord = false;
}

void readyTrie(Trie *t, FILE *fp) { // To create entire Trie structure before user starts typing
	int i = 0;
	char ch;
	t->root = (TrieNode *)malloc(sizeof(TrieNode));
	TrieNode *current = t->root;
	
	while(i < 26) {
		current->children[i] = NULL;
		i++;
	}
	ch = fgetc(fp);
	
	while(ch != EOF) {
		if(ch == '\n') {
			current->isValidWord = true;
			current = t->root;
			ch = fgetc(fp);
			continue;
		}
		
		createNode(&current, ch);
		ch = fgetc(fp);
	}
}

void freeSubTrie(TrieNode *t) { // To free the useless sub Trie, according to user input
	if(!t) {
		return;
	}
	
	for(int i = 0; i < 26; i++) {
		freeSubTrie(t->children[i]);
	}
	
	free(t);
	t = NULL;
}

void destroy(Trie *t) { // To destroy the entire Trie structure
	freeSubTrie(t->root);
}


void printStaq() { //to print the current contents of staq
	SQNode *p = sq.front;
	
	while(p) {
		printf("%c", p->character);
		p = p->next;
	}
}


void traverse(TrieNode *t, int indexParent) { // Traversing to give prompts
	if(!t) {
		return;
	}
	
	char ch = (indexParent + 'a');
	pushStaq(ch);
	for(int i = 0; i < 26; i++) {
		traverse(t->children[i], i);
	}
	
	if(t->isValidWord == true) {
		if(prompts.indexOfLastPrompt >= (PROMPT_COUNT - 1)) {
			popStaq();
			return;
		}
		helperFunction();
	}
	
	else {
		popStaq();
	}
}
