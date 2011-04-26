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
#ifndef SIMPLELOADERTOKEN_H
#define SIMPLELOADERTOKEN_H

#include "cshTypes_String.h"

extern const int TOKEN_TYPE_UNDEF;
extern const int TOKEN_TYPE_ID;
extern const int TOKEN_TYPE_WORD;
extern const int TOKEN_TYPE_SYM;
extern const int TOKEN_TYPE_NUM;
extern const int TOKEN_TYPE_DQ_STRING;
extern const int TOKEN_TYPE_SQ_STRING;
extern const int TOKEN_TYPE_DOTTED_NUM;

class token
{
	public:
		token(int tt,char *str):tokType(tt),strRep(str)
		{
		}

		virtual ~token()
		{
		}

		virtual void dumpToken()
		{
		}

		virtual int getTokType()
		{
			return tokType;
		}

		virtual CSHString *getRep()
		{
			return &strRep;
		}

		virtual int like(char *s)
		{
			return strRep.equals(s);
		}

	private:
		int tokType;
		CSHString strRep;
};

#endif
