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
#include "uuidToken.h"

const int SPECIAL_TOKEN_TYPE_UUID	=-1;

void uuidToken::init()
{
	currState = TOKEN_STATE_NO_MATCH_MORE_CHARS_NEEDED;
	number = "";
	uuidFound = 0;
}

void uuidToken::injectChar(char ch)
{
	ch = toupper(ch);
	if(currState==TOKEN_STATE_MATCH_FOUND)
		currState = TOKEN_STATE_MATCH_ERROR;
	else
	{
		CSHString valid("UID(-0123456789ABCDEF");
		CSHString validAfterUUID("(-0123456789ABCDEF");
		if(charInList(ch,valid))
		{
			if(ch=='U')
			{
				if(!((number.length()==0) || (number.length()==1)))
				{
					//we only allow an u at the first and second position
					currState=TOKEN_STATE_MATCH_ERROR;
				}
			}
			else if(ch=='I')
			{
				if(!(number.length()==2))
				{
					//we only allow an i at this position
					currState=TOKEN_STATE_MATCH_ERROR;
				}
			}
			else if(ch=='D')
			{
				if(!(number.length()==3))
				{
					//we only allow an D at this third position
					if(!uuidFound)
						currState=TOKEN_STATE_MATCH_ERROR;
				}
			}
			else if(ch=='(')
			{
				if(!(number.length()==4))
				{
					//we only allow an ( at this third position
					currState=TOKEN_STATE_MATCH_ERROR;
				}
				else
					uuidFound = 1;
			}
			else if(ch=='-')
				if(number.length()<4)
					currState=TOKEN_STATE_MATCH_ERROR;

			if(uuidFound)
			{
				if(!charInList(ch,validAfterUUID))
					currState=TOKEN_STATE_MATCH_ERROR;
			}

			number.Cat(ch);
		}
		else
		{
			if(!uuidFound)
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

char *uuidToken::getTermCharList()
{
	return ")";
}

int uuidToken::getState()
{
	return currState;
}

void uuidToken::dumpToken()
{
	printf("UUID:%s",number.GetBuffer());
}
