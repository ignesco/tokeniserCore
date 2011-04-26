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
#ifndef UUIDTOKEN_H
#define UUIDTOKEN_H

#ifndef _ASSERTE
#define _ASSERTE
#endif

//#include "CSHMemDebug.h"
#include "standardTokens.h"

extern const int SPECIAL_TOKEN_TYPE_UUID;

class uuidToken : public token
{
	public:
		uuidToken(char *num=""):number(num){};
		virtual ~uuidToken()
		{
		}

		virtual void init();
		virtual void reachedEndOfStream()
		{
			if(currState == TOKEN_STATE_MATCH_MORE_CHARS_NEEDED)
			{
				currState = TOKEN_STATE_MATCH_FOUND;
			}
			else if(!currState != TOKEN_STATE_MATCH_FOUND)
				currState = TOKEN_STATE_MATCH_ERROR;
		}

		virtual char *getTermCharList();

		virtual void injectChar(char ch);
		virtual int getState();
		virtual void dumpToken();

		virtual CSHString *getRep()
		{
			return &number;
		}

		virtual token *makeClone()
		{
			token *retVal = new uuidToken(number);
			retVal->setTokType(SPECIAL_TOKEN_TYPE_UUID);
			return retVal;
		}

	private:
		CSHString number;
		int currState;
		int uuidFound;
};


#endif
