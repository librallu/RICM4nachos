#ifdef CHANGED
#include "copyright.h"
#include <stdlib.h>
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;
static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }

SynchConsole::SynchConsole(char *readFile, char *writeFile) {
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);
	console = new Console(readFile,writeFile,ReadAvail,WriteDone,0);
}

SynchConsole::~SynchConsole() {
	delete console;
	delete writeDone;
	delete readAvail;
}
void SynchConsole::SynchPutChar(const char ch){
	console->PutChar(ch);
	writeDone->P();
}

char SynchConsole::SynchGetChar() {
	char ch;
	readAvail->P();
	ch = console->GetChar();
	return ch;
}

void SynchConsole::SynchPutString(const char s[]){
	int i = 0;
	while (s[i]!='\0'){
		SynchPutChar(s[i++]);
	}
}

void SynchConsole::SynchGetString(char *s, int n){
	s = (char*) malloc (n);
	for (int i=0;i<n;i++){
		s[i]=SynchGetChar();
	}
}

#endif // CHANGED
