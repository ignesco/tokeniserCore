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
#define _ASSERTE
#include "CSHMemDebug.h"
#include "standardTokens.h"

#include "cshTypes_String.h"

const int TOKEN_TYPE_UNDEF		=0x00;
const int TOKEN_TYPE_ID			=0x01;
const int TOKEN_TYPE_WORD		=0x02;
const int TOKEN_TYPE_SYM		=0x03;
const int TOKEN_TYPE_NUM		=0x04;
const int TOKEN_TYPE_DQ_STRING	=0x05;
const int TOKEN_TYPE_SQ_STRING	=0x06;
const int TOKEN_TYPE_DOTTED_NUM	=0x07;


char *identifierToken::getTermCharList()
{
	return " \r\t\n()\\/;-',[=";
}

token *identifierToken::makeClone()
{
	token *retVal = new identifierToken(identifier.GetBuffer());
	retVal->setTokType(TOKEN_TYPE_ID);
	return retVal;
}

void identifierToken::init()
{
	currState = TOKEN_STATE_MATCH_MORE_CHARS_NEEDED;
	identifier = "";
	firstChar = 1;
}

void identifierToken::injectChar(char ch)
{
	if(currState!=TOKEN_STATE_MATCH_ERROR)
		if(currState==TOKEN_STATE_MATCH_FOUND)
			currState = TOKEN_STATE_MATCH_ERROR;
		else
		{
			int validChar = 0;

			CSHString validChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789";
			for(int i=0;i<validChars.length();i++)
			{
				if(ch==validChars[i])
				{
					validChar = 1;
					break;
				}
			}

			if(firstChar)
			{
				if((ch>='0') && (ch<='9'))
					validChar = 0;

			}

			if(validChar)
			{
				identifier.Cat(ch);
				firstChar = 0;
			}
			else
			{
				if(firstChar && !validChar)
					currState = TOKEN_STATE_MATCH_ERROR;
				else
				{
					CSHString termString = getTermCharList();

					if(charInList(ch,termString))
						currState = TOKEN_STATE_MATCH_FOUND;
					else
						currState = TOKEN_STATE_MATCH_ERROR;
				}
			}
		}
}


int identifierToken::getState()
{
	return currState;
}

void identifierToken::dumpToken()
{
	printf("ID:%s",identifier.GetBuffer());
}

void numberToken::init()
{
	currState = TOKEN_STATE_NO_MATCH_MORE_CHARS_NEEDED;
	number = "";
	firstNumFound = 0;
	isHex = 0;
}

void numberToken::injectChar(char ch)
{
	ch = toupper(ch);
	if(currState==TOKEN_STATE_MATCH_FOUND)
		currState = TOKEN_STATE_MATCH_ERROR;
	else
	{
		CSHString valid("0123456789ABCDEFX");
		CSHString decimalvalid("0123456789");
		if(charInList(ch,valid))
		{
			if(ch=='X')
			{
				if(!(number.length()==1))
				{
					//we only allow an x at the second position
					currState=TOKEN_STATE_MATCH_ERROR;
				}
				else
				{
					if(number.GetBuffer()[0]=='0')
						isHex = 1;
					else
						currState=TOKEN_STATE_MATCH_ERROR;
				}
			}

			if(!isHex)
			{
				if(!charInList(ch,decimalvalid))
					currState=TOKEN_STATE_MATCH_ERROR;
			}

			number.Cat(ch);
			firstNumFound = 1;
		}
		else
		{
			if(!firstNumFound)
				currState = TOKEN_STATE_MATCH_ERROR;
			else
			{
				CSHString  termString = getTermCharList();

				if(charInList(ch,termString) && (currState!=TOKEN_STATE_MATCH_ERROR))

					currState = TOKEN_STATE_MATCH_FOUND;
				else
					currState = TOKEN_STATE_MATCH_ERROR;
			}
		}
	}
}

char *numberToken::getTermCharList()
{
	return " \t\r\n'()+-/*,:;]}";
}

int numberToken::getState()
{
	return currState;
}

void numberToken::dumpToken()
{
	printf("NUM:%s",number.GetBuffer());
}

void dottedNumberToken::init()
{
	currState = TOKEN_STATE_NO_MATCH_MORE_CHARS_NEEDED;
	number = "";
	firstNumFound = 0;
	isHex = 0;
}

void dottedNumberToken::injectChar(char ch)
{
	ch = toupper(ch);
	if(currState==TOKEN_STATE_MATCH_FOUND)
		currState = TOKEN_STATE_MATCH_ERROR;
	else
	{
		CSHString valid("0123456789ABCDEFX.");
		CSHString decimalvalid("0123456789.");
		if(charInList(ch,valid))
		{
			if(ch=='X')
			{
				if(!(number.length()==1))
				{
					//we only allow an x at the second position
					currState=TOKEN_STATE_MATCH_ERROR;
				}
				else
				{
					if(number.GetBuffer()[0]=='0')
						isHex = 1;
					else
						currState=TOKEN_STATE_MATCH_ERROR;
				}
			}

			if(!isHex)
			{
				if(!charInList(ch,decimalvalid))
					currState=TOKEN_STATE_MATCH_ERROR;
			}

			number.Cat(ch);
			firstNumFound = 1;
		}
		else
		{
			if(!firstNumFound)
				currState = TOKEN_STATE_MATCH_ERROR;
			else
			{
				CSHString  termString = getTermCharList();

				if(charInList(ch,termString) && (currState!=TOKEN_STATE_MATCH_ERROR))

					currState = TOKEN_STATE_MATCH_FOUND;
				else
					currState = TOKEN_STATE_MATCH_ERROR;
			}
		}
	}
}

char *dottedNumberToken::getTermCharList()
{
	return " \t\r\n'()+-/*,:;]";
}

int dottedNumberToken::getState()
{
	return currState;
}

void dottedNumberToken::dumpToken()
{
	printf("DOTTEDNUM:%s",number.GetBuffer());
}

void simpleToken::dumpToken()
{
	printf("SIMP:%s",strMatch.GetBuffer());
}

void simpleToken::init()
{
	strMatchPtr = -1;
	currState = TOKEN_STATE_MATCH_MORE_CHARS_NEEDED;
	nextCharShouldBeAWhitespace = 0;
}

void simpleToken::reachedEndOfStream()
{
	if(currState!=TOKEN_STATE_MATCH_ERROR)
		if((strMatchPtr+1)==strMatch.length())
			currState = TOKEN_STATE_MATCH_FOUND;
		else
			currState = TOKEN_STATE_MATCH_ERROR;
}

void simpleToken::injectChar(char ch)
{
	if(currState==TOKEN_STATE_MATCH_FOUND)
		currState = TOKEN_STATE_MATCH_ERROR;
	else
	{
		if(nextCharShouldBeAWhitespace && (currState!=TOKEN_STATE_MATCH_ERROR))
		{
			CSHString termString = getTermCharList();

			currState = TOKEN_STATE_MATCH_ERROR;

			if(charInList(ch,termString))
					currState = TOKEN_STATE_MATCH_FOUND;
		}
		else
		{
			if(strMatchPtr<strMatch.length())
			{
				//We are still in the middle of this token
				strMatchPtr++;
				if(strMatch[strMatchPtr]!=ch)
				{
					currState = TOKEN_STATE_MATCH_ERROR;
				}
			}

			if((strMatchPtr+1)==strMatch.length())
			{
				//We are not sure if we have found this prompt yet as we don't know if
				//the next char is a whitespace yet...
				nextCharShouldBeAWhitespace = 1;
			}
		}
	}
}

int simpleToken::getState()
{
	return currState;
}
