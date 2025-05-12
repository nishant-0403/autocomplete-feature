#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "autocomplete.h"

int main() {
	int numOfWords = 0;
	int i = 0, j = 0, k = 0;
	int indexOfInput;
	char inputArray[12];
	char identifier, tempChar;
	Stack st;
	TrieNode *current;
	Trie trie;
	initStack(&st);
	initStaq();
	initTrie(&trie);
	initPrompts();
	FILE *fp = fopen("/home/nishant/coepAcademics/sem3_sy/dsa/dsa_project/final/smallDictionary.txt", "r");
	readyTrie(&trie, fp);
	fclose(fp);
	current = trie.root;
	printf("Hello!\n");
	printf("Start entering input!\n");
	printf("To select a prompt, enter the digit displayed before it\n");
	
	
	while(numOfWords < 10) {
		inputArray[j] = tolower(getchar());
		if(inputArray[j] == '\n') {
			continue;
		}
		
		int asciiOfInput = (int)inputArray[j];
		
		if((asciiOfInput >= 48) && (asciiOfInput <= 57)) { // If user wants to select a prompt
			printSelectedPrompt(asciiOfInput);
			destroy(&trie);
			initTrie(&trie);
			fp = fopen("/home/nishant/coepAcademics/sem3_sy/dsa/dsa_project/final/smallDictionary.txt", "r");
			readyTrie(&trie, fp);
			fclose(fp);
			current = trie.root;
			freePrompts();
			freeStaq();
			initStaq();
			initPrompts();
			j = 0;
			numOfWords++;
			
			if(numOfWords >= 10)
				break;
			
			printf("Start entering new word\n");
			continue;	
		}
		
		if (inputArray[j] == '\t' || inputArray[j] == ' ' || inputArray[j] == '.' || inputArray[j] == ',' || inputArray[j] == ';') {  // If user enters delimiter
		
			if(numOfWords >= 10)
				break;
			printf("Start entering new word\n");
			destroy(&trie);
			initTrie(&trie);
			fp = fopen("/home/nishant/coepAcademics/sem3_sy/dsa/dsa_project/final/smallDictionary.txt", "r");
			readyTrie(&trie, fp);
			fclose(fp);
			current = trie.root;
			freePrompts();
			freeStaq();
			initStaq();
			initPrompts();
			j = 0;
			numOfWords++;
			continue;	
		}
		
		indexOfInput = (int)inputArray[j] - 97;

		if(!current || !current->children[indexOfInput]) {
			push(&st, current);
			if(current) {
				current = current->children[indexOfInput];
			}
			
			printf("Word not in dictionary, enter 'E' to keep typing, and 'B' for backspace\n");
			scanf("%c", &tempChar);
			scanf("%c", &identifier); 			
			
			if(identifier == 'E') {
			
				for(k = 0; k <= j; k++) {
					printf("%c", inputArray[k]);
				}
				
				j++;
				continue;
			}
			
			while(identifier == 'B') {
				printPrompts();
				
				for(k = 0; k < j; k++) {
					printf("%c", inputArray[k]);
				}
				
				j--;
				current = pop(&st);
				scanf("%c", &tempChar);
				scanf("%c", &identifier);
			}
			
			if(identifier == 'E') {
			
				for(k = 0; k <= j; k++) {
					printf("%c", inputArray[k]);
				}
				
				j++;
				continue;
			}
			else {
				printSelectedPrompt((int)identifier);
				destroy(&trie);
				initTrie(&trie);
				fp = fopen("/home/nishant/coepAcademics/sem3_sy/dsa/dsa_project/final/smallDictionary.txt", "r");
				readyTrie(&trie, fp);
				fclose(fp);
				current = trie.root;
				freePrompts();
				freeStaq();
				initStaq();
				initPrompts();
				j = 0;
				numOfWords++;
				
				if(numOfWords >= 10)
					break;
					
				printf("Start entering new word\n");
				continue;	
			}
		}	
		freePrompts();
		initPrompts();	
		i = 0;
		
		while(i < 26) { // To free useless sub trie
			if(i == indexOfInput) {
				i++;
				continue;
			}
			
			else {
				freeSubTrie(current->children[i]);
				current->children[i] = NULL;
			}
			i++;
		}

		current = current->children[indexOfInput];
		traverse(current, indexOfInput);
		pushStaq(inputArray[j]);
		
		for(k = 0; k <= j; k++) {
			printf("%c", inputArray[k]);
		}
		
		j++;
	}
	
	printf("10-word limit exceeded!\n");
	freePrompts();
	freeStaq();
	destroy(&trie);
	return 0;
}
