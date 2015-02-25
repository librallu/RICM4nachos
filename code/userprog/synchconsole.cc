#ifdef CHANGED
#include "copyright.h"
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
	console = new Console(readFile, writeFile, ReadAvail, WriteDone, 0);
}

SynchConsole::~SynchConsole() {
	delete console;
	delete writeDone;
	delete readAvail;
}

void SynchConsole::SynchPutChar(const char ch) {
	console->PutChar(ch);
	writeDone->P();
}

int SynchConsole::SynchGetChar() {
	readAvail->P();
	return (int) (console->Feof() ? EOF : console->GetChar());
	//return console->GetChar();
}

void SynchConsole::SynchPutString(const char s[]) {
	int i = 0;
	while ( s[i] != '\0' ){
		SynchPutChar(s[i]);
		i++;
	}

}


void SynchConsole::SynchGetString(char*s, int n){
	int i; 
	char c;
	for ( i = 0 ; i < n-1 ; i++ ){
		c = SynchGetChar();
		if ( c == EOF )
			break;
		s[i] = c;
		if ( c == '\n' ) {
			i++;
			break;
		}	
	}
	s[i] = '\0';
}


void SynchConsole::SynchPutInt(int n){
	char buff[MAX_STRING_SIZE];
	snprintf(buff,MAX_STRING_SIZE, "%d",n);
	SynchPutString(buff);
}

void SynchConsole::SynchGetInt(int *n){
	char buff[MAX_STRING_SIZE];
	SynchGetString(buff,MAX_STRING_SIZE);
	*n=0;
	ASSERT(sscanf(buff,"%d",n) != EOF );
}


#endif // CHANGED

