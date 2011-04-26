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
#ifndef STANDARDTOKENS_H
#define STANDARDTOKENS_H

const int TOKEN_STATE_MATCH_FOUND					=0x01;
const int TOKEN_STATE_MATCH_ERROR					=0x02;
const int TOKEN_STATE_MATCH_MORE_CHARS_NEEDED		=0x03;
const int TOKEN_STATE_NO_MATCH_MORE_CHARS_NEEDED	=0x04;

extern const int TOKEN_TYPE_UNDEF;
extern const int TOKEN_TYPE_ID;
extern const int TOKEN_TYPE_WORD;
extern const int TOKEN_TYPE_SYM;
extern const int TOKEN_TYPE_NUM;
extern const int TOKEN_TYPE_DQ_STRING;
extern const int TOKEN_TYPE_SQ_STRING;
extern const int TOKEN_TYPE_DOTTED_NUM;

#include "cshTypes_String.h"
#include "CSHMemDebug.h"
#include <string.h>
#include <ctype.h>

class token
{
	public:
		DEBUG_THIS_CLASS;
		token():tokType(TOKEN_TYPE_UNDEF){};
		virtual ~token()
		{
		}

		virtual char *getTermCharList() = 0;

		virtual int charInList(char ch,CSHString &str)
		{
			int retVal = 0;
			for(int i=0;i<str.length();i++)
			{
				if(ch==str[i])
				{
					retVal = 1;
					break;
				}
			}

			return retVal;
		}

		virtual int like(char *lstr)
		{
			if(getRep()->equal(lstr))
				return 1;
			else
				return 0;
		}

		virtual void init() = 0;
		virtual void injectChar(char ch) = 0;
		virtual void reachedEndOfStream() = 0;
		virtual int getState() = 0;
		virtual void dumpToken() = 0;
		virtual token *makeClone() = 0;
		virtual CSHString *getRep() = 0;

		static int isWhitespace(char ch)
		{
			if((ch==' ') || (ch=='\t') || (ch=='\r') || (ch=='\n'))
				return 1;
			else
				return 0;
		}

		virtual int getTokType()
		{
			return tokType;
		}
		virtual void setTokType(int type)
		{
			tokType = type;
		}

	private:
		int tokType;
};

#define QUOTEDSTRINGVALIDCHARS "_ %%abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789)(:,{;->}!\\/.*<>?+#=$"

class doubleQuotedStringToken : public token
{
	public:
		doubleQuotedStringToken(char *str=""):token(),string(str)
		{
		}

		virtual ~doubleQuotedStringToken()
		{
		}

		virtual char *getTermCharList()
		{
			return " \t\r\n)],;";
		}

		virtual void injectChar(char ch)
		{
			if(currState==TOKEN_STATE_MATCH_MORE_CHARS_NEEDED)
			{
				//We just need a valid terminiating char
				CSHString str=getTermCharList();
				if(charInList(ch,str))
				{
					currState = TOKEN_STATE_MATCH_FOUND;
				}
				else
					currState = TOKEN_STATE_MATCH_ERROR;
			}
			else if(currState==TOKEN_STATE_MATCH_FOUND)
			{
				currState = TOKEN_STATE_MATCH_ERROR;
			}
			else if(currState==TOKEN_STATE_MATCH_ERROR)
			{
			}
			else
			{
				if(firstChar)
				{
					if(ch!='\"')
						currState = TOKEN_STATE_MATCH_ERROR;
					else
						string.Cat(ch);
					firstChar = 0;
				}
				else
				{
					CSHString validChars = QUOTEDSTRINGVALIDCHARS;
					if(charInList(ch,validChars))
					{
						string.Cat(ch);
						firstChar = 0;
					}
					else
					{
						string.Cat(ch);
						if(ch=='\"')
							currState = TOKEN_STATE_MATCH_MORE_CHARS_NEEDED;
						else
							currState = TOKEN_STATE_MATCH_ERROR;
					}
				}
			}
		}

		virtual void reachedEndOfStream()
		{
			if(currState == TOKEN_STATE_MATCH_MORE_CHARS_NEEDED)
			{
				currState = TOKEN_STATE_MATCH_FOUND;
			}
			else if(!currState != TOKEN_STATE_MATCH_FOUND)
				currState = TOKEN_STATE_MATCH_ERROR;
		}

		virtual void init()
		{
			currState = TOKEN_STATE_NO_MATCH_MORE_CHARS_NEEDED;
			string = "";
			firstChar = 1;
		}

		virtual int getState()
		{
			return currState;
		}

		virtual void dumpToken()
		{
			printf("DQS:%s",string.GetBuffer());
		}

		virtual token *makeClone()
		{
			token *retVal = new doubleQuotedStringToken(string.GetBuffer());
			retVal->setTokType(TOKEN_TYPE_DQ_STRING);
			return retVal;
		}

		virtual CSHString *getRep()
		{
			return &string;
		}

	private:
		CSHString string;
		int currState;
		int firstChar;
};

class singleQuotedStringToken : public token
{
	public:
		singleQuotedStringToken(char *str=""):token(),string(str)
		{
		}

		virtual ~singleQuotedStringToken()
		{
		}

		virtual char *getTermCharList()
		{
			return " \t\r\n)],;";
		}

		virtual void injectChar(char ch)
		{
			if(currState==TOKEN_STATE_MATCH_MORE_CHARS_NEEDED)
			{
				//We just need a valid terminiating char
				CSHString str=getTermCharList();
				if(charInList(ch,str))
				{
					currState = TOKEN_STATE_MATCH_FOUND;
				}
				else
					currState = TOKEN_STATE_MATCH_ERROR;
			}
			else if(currState==TOKEN_STATE_MATCH_FOUND)
			{
				currState = TOKEN_STATE_MATCH_ERROR;
			}
			else
			{
				if(firstChar)
				{
					if(ch!='\'')
						currState = TOKEN_STATE_MATCH_ERROR;
					else
						string.Cat(ch);
					firstChar = 0;
				}
				else
				{
					CSHString validChars = QUOTEDSTRINGVALIDCHARS;
					if(charInList(ch,validChars))
					{
						string.Cat(ch);
						firstChar = 0;
					}
					else
					{
						string.Cat(ch);
						if(ch=='\'')
							currState = TOKEN_STATE_MATCH_MORE_CHARS_NEEDED;
						else
							currState = TOKEN_STATE_MATCH_ERROR;
					}
				}
			}
		}

		virtual void reachedEndOfStream()
		{
			if(currState == TOKEN_STATE_MATCH_MORE_CHARS_NEEDED)
			{
				currState = TOKEN_STATE_MATCH_FOUND;
			}
			else if(!currState != TOKEN_STATE_MATCH_FOUND)
				currState = TOKEN_STATE_MATCH_ERROR;
		}

		virtual void init()
		{
			currState = TOKEN_STATE_NO_MATCH_MORE_CHARS_NEEDED;
			string = "";
			firstChar = 1;
		}

		virtual int getState()
		{
			return currState;
		}

		virtual void dumpToken()
		{
			printf("SQS:%s",string.GetBuffer());
		}

		virtual token *makeClone()
		{
			token *retVal = new singleQuotedStringToken(string.GetBuffer());
			retVal->setTokType(TOKEN_TYPE_SQ_STRING);
			return retVal;
		}

		virtual CSHString *getRep()
		{
			return &string;
		}

	private:
		CSHString string;
		int currState;
		int firstChar;
};


class identifierToken : public token
{
	public:
		identifierToken(char *id=""):identifier(id){};
		virtual ~identifierToken()
		{
		}
		virtual void init();

		virtual char *getTermCharList();

		virtual void injectChar(char ch);
		virtual int getState();
		virtual void dumpToken();

		virtual token *makeClone();
		virtual CSHString *getRep()
		{
			return &identifier;
		}

		virtual void reachedEndOfStream()
		{
			if(currState == TOKEN_STATE_MATCH_MORE_CHARS_NEEDED)
			{
				currState = TOKEN_STATE_MATCH_FOUND;
			}
			else if(!currState != TOKEN_STATE_MATCH_FOUND)
				currState = TOKEN_STATE_MATCH_ERROR;
		}


	private:
		CSHString identifier;
		int currState;
		int firstChar;
};



class simpleToken : public token
{
	public:
		simpleToken(char *strToMatch):strMatch(strToMatch){};
		virtual ~simpleToken(){};
		virtual void init();

		virtual void injectChar(char ch);
		virtual void reachedEndOfStream();

		virtual int getState();
		virtual void dumpToken();

		virtual char *getTermCharList()
		{
			return "; \n\r\t,[]:().*";
		}

		virtual token *makeClone()
		{
			token *retVal = new simpleToken(strMatch.GetBuffer());
			retVal->setTokType(TOKEN_TYPE_WORD);
			return retVal;
		}

		virtual CSHString *getRep()
		{
			return &strMatch;
		}

	private:
		CSHString strMatch;
		int strMatchPtr;

		int currState;
		int nextCharShouldBeAWhitespace;
		int caseSensitive;
};


class numberToken : public token
{
	public:
		numberToken(char *num=""):number(num),isHex(0){};
		virtual ~numberToken()
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
			token *retVal = new numberToken(number);
			retVal->setTokType(TOKEN_TYPE_NUM);
			return retVal;
		}

	private:
		CSHString number;
		int currState;
		int firstNumFound;
		int isHex;
};


class dottedNumberToken : public token
{
	public:
		dottedNumberToken(char *num=""):number(num),isHex(0){};
		virtual ~dottedNumberToken()
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
			token *retVal = new dottedNumberToken(number);
			retVal->setTokType(TOKEN_TYPE_DOTTED_NUM);
			return retVal;
		}

	private:
		CSHString number;
		int currState;
		int firstNumFound;
		int isHex;
};


class SYMBOLToken : public simpleToken
{
	public:
		SYMBOLToken(char *word):simpleToken(word),theWord(word){};
		virtual ~SYMBOLToken()
		{
		}

		virtual void injectChar(char ch)
		{
			simpleToken::injectChar(ch);
		}

		virtual char *getTermCharList()
		{
			return ".+-*/(),;[] \"\t\r\n'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
		}

		virtual CSHString *getRep()
		{
			return &theWord;
		}
		virtual void dumpToken()
		{
			printf("SYM:%s",theWord.GetBuffer());
		}

		virtual token *makeClone()
		{
			token *retVal = new SYMBOLToken(theWord.GetBuffer());
			retVal->setTokType(TOKEN_TYPE_SYM);
			return retVal;
		}

	private:
		CSHString theWord;

};


#endif
