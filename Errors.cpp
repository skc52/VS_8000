#include "pch.h"

#include "Errors.h"


vector<string> Errors::m_ErrorMsgs;
int Errors::errorCount;

/**/
/*
Errors::InitErrorReporting()
NAME
	Errors::InitErrorReporting - empties the vector storing error messages and resets errocount to 0
SYNOPSIS
	void Errors::InitErrorReporting();
DESCRIPTION
	This function will be called before starting to record errors.
	It will set the vector for storing error messages m_ErrorMsgs to empty.
	It will set the errorCount to 0.
Returns
	void

*/
/**/

void Errors::InitErrorReporting() {
	m_ErrorMsgs = {};
	errorCount = 0;
}
/*Errors::InitErrorReporting();*/


/**/
/*
Errors::RecordError()
NAME
	Errors::RecordError - Records the error message.
SYNOPSIS
	Errors::RecordError(string a_emsg);
	a_emsg -> The string that contains the error message.
DESCRIPTION
	Stores the error message a_emsg to the end of the vector
	consisting of the list of error messages.
Returns
	void

*/
/**/

void Errors::RecordError(string a_emsg) {
	m_ErrorMsgs.push_back(a_emsg);
	errorCount++;
}

/*Errors::RecordError(string a_emsg);*/




/**/
/*
Errors::DisplayErrors()
NAME
	Errors::DisplayErrors - Prints the lists of errors.
SYNOPSIS
	void Errors::DisplayErrors();
DESCRIPTION
	Prints out all the error messages stored in the vector that consists of errors.
Returns
	void
*/
/**/

void Errors::DisplayErrors() {
	for (string i : m_ErrorMsgs)
		cout << i << endl;
}

/*void Errors::DisplayErrors();*/
