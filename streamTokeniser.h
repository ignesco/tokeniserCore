/*
* Copyright (c) 1999-2006,2007, Craig S. Harrison
*
* All rights reserved.
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
*/
#ifndef STREAMTOKENISER_H
#define STREAMTOKENISER_H

#include "utilExceptions.h"

#include "tokenList.h"

template<class streamClass>
class genericTokeniser
{
	public:
		genericTokeniser()
		{
		}

		virtual ~genericTokeniser()
		{
		}

		virtual void tokenise(streamClass *) = 0;

};

template<class rClass,class streamClass>
class streamTokeniser : public genericTokeniser<streamClass>
{
	public:
		streamTokeniser(rClass *rc,int caseS):genericTokeniser<streamClass>(),reciever(rc),caseSensitive(caseS){}
		virtual ~streamTokeniser()
		{
			for(int i=0;i<tokList->getNumberOfItems();i++)
				delete tokList->getValueAtIndex(i);

			delete tokList;
		}

		virtual void initTokenList() = 0;
		virtual void tokenise(streamClass *);
		virtual void skipToNextToken();
		virtual int skipComments();

	protected:
		virtual void addToken(token *t);
		streamClass *scObj;
		char ach;
		int charsRead;

	private:
		virtual void internal_initTokenList();

		virtual void doReadNextChar(int skipComments=0);
		rClass *reciever;
		tokenList *tokList;
		int caseSensitive;
};

template<class rClass,class streamClass>
void streamTokeniser<rClass,streamClass>::addToken(token *t)
{
	tokList->add(t);
}

template<class rClass,class streamClass>
void streamTokeniser<rClass,streamClass>::internal_initTokenList()
{
	tokList = new tokenList;

	initTokenList();

	for(int i=0;i<tokList->getNumberOfItems();i++)
	{
		tokList->getValueAtIndex(i)->init();
	}
}

template<class rClass,class streamClass>
void streamTokeniser<rClass,streamClass>::tokenise(streamClass *sc)
{
	scObj = sc;
	internal_initTokenList();
	

	// = fread(&ach,1,1,f);
	skipToNextToken();
	int readNextChar;
	int doMoreChars = 1;
	//while(charsRead==1)
	int reachedEnd = 0;
	while(doMoreChars)
	{
	  //printf("%c",ach);
		readNextChar = 1;

		if(!caseSensitive)
			ach = toupper(ach);
		int matchCount=0;
		int notMatchedYetCount = 0;
		int lastMatchIndex = -1;
		for(int i=0;i<tokList->getNumberOfItems();i++)
		{
			token *t = tokList->getValueAtIndex(i);
			if(charsRead==1)
				t->injectChar(ach);
			else
			{
				t->reachedEndOfStream();
				doMoreChars = 0;
			}

			int tokState = t->getState();
			if(tokState==TOKEN_STATE_MATCH_FOUND)
			{
				matchCount++;
				lastMatchIndex = i;
			}
			if(tokState==TOKEN_STATE_MATCH_MORE_CHARS_NEEDED)
			{
				notMatchedYetCount++;
			}
			if(tokState==TOKEN_STATE_NO_MATCH_MORE_CHARS_NEEDED)
			{
				notMatchedYetCount++;
			}
		}

		//if(matchCount>0)
		if((matchCount>0) && (notMatchedYetCount==0))
		{
			//We have found a match

			token *t = tokList->getValueAtIndex(lastMatchIndex)->makeClone();
			reciever->newToken(t);

			readNextChar = 0;
			if(token::isWhitespace(ach))
			{
				skipToNextToken();

				if(charsRead==0)
					reachedEnd = 1;
			}

			for(int i=0;i<tokList->getNumberOfItems();i++)
			{
				tokList->getValueAtIndex(i)->init();
			}
		}

		if(!reachedEnd)
		{
			if((notMatchedYetCount==0) && (matchCount==0))
			{
			  char smallBuff[20];
			  sprintf(smallBuff,"%d",ach);
			  CSHString str("Invalid char in file stream:");
			  str.Cat(smallBuff);
			  str.Cat("'");
			  str.Cat(ach);
			  str.Cat("'");
			  throw generalFatalException(str.GetBuffer());
			}
			
			if(readNextChar)
				doReadNextChar();
		}
		else
		{
			doMoreChars = 0;
		}
	}

	//CSH NN
	//fclose(f);
}

template<class rClass,class streamClass>
int streamTokeniser<rClass,streamClass>::skipComments()
{
	size_t currLoc = scObj->ftell();
	char tempBuff[2];
	charsRead = scObj->fread(tempBuff,2);
	int foundComment = 0;
	if(charsRead==2)
	{
		if((*tempBuff=='-') && (*(tempBuff+1)=='-'))
		{
			//We have a comment
			foundComment = 1;

			charsRead = scObj->fread(&ach,1);
			while((charsRead>0) && (ach!='\r') && (ach!='\n') )
				charsRead = scObj->fread(&ach,1);
		}
	}

	if(!foundComment)
		scObj->fseek(currLoc,SEEK_SET);

	return foundComment;
}

template<class rClass,class streamClass>
void streamTokeniser<rClass,streamClass>::doReadNextChar(int skipComments)
{
	if(skipComments)
	{
		while(this->skipComments());
	}

	charsRead = scObj->fread(&ach,1);
	#ifdef DEBUG_READ
	if(charsRead==1)
	{
		printf("%c",ach);
	}
	#endif
}


template<class rClass,class streamClass>
void streamTokeniser<rClass,streamClass>::skipToNextToken()
{
	doReadNextChar(1);
	while((charsRead==1) && ((ach==' ') || (ach=='\t') || (ach=='\r') || (ach=='\n')))
	  {
		doReadNextChar(1);
	  }
}

#endif
