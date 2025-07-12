#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "autocomplete.h"

Prompts prompts;

Staq sq;

void initStack(Stack *st) {
	st->top = -1;
	st->size = 8;
}

void push(Stack *st, TrieNode *t) {	
	if(st->top < st->size) {
		st->top++;
		st->arr[st->top] = t;
	}
}

TrieNode* pop(Stack *st) {
	if(st->top == -1)
		return NULL;
	TrieNode *t = st->arr[st->top];
	st->top--;
	return t;
}

void printPrompts() {

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
	
	if(index < 0 || index >= PROMPT_COUNT) {
		printf("Invalid input given!!\n");
		return;
	}
	
	if(!prompts.promptArr[index])
		printf("Invalid input given!!");
	
	for(i = 0; i != prompts.lengthArray[index]; i++) {
		ch = (prompts.promptArr[index])[i];
		printf("%c", ch);
	}
	printf("\n");
}

int isEmpty() {
	if(sq.front == NULL) {
		return 1;
	}
	else {
		return 0;
	}
}

void initPrompts() {
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

void initTrie(Trie *t) {
	t->root = NULL;
}

void initStaq() {
	sq.front = sq.rear = NULL;
}

void freeStaq() {
	while(!isEmpty()) {
		popStaq();
	}
}

void pushStaq(char ch) {
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

void popStaq() {
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

char deStaq() {
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

int findLength() {
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

void helperFunction() { // To facilitate printing and recording prompts, and adjusting the Staq
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


void printStaq() {
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
