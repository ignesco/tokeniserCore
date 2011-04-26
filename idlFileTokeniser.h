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
#ifndef IDLFILETOKENISER_H
#define IDLFILETOKENISER_H

#include "uuidToken.h"

template<class rClass,class StreamClass>
class idlFileTokeniser: public streamTokeniser<rClass,StreamClass>
{
	public:
		idlFileTokeniser(rClass *psr):streamTokeniser<rClass,StreamClass>(psr,1)
		{
		}

		virtual ~idlFileTokeniser()
		{
		}

		virtual void initTokenList();
};

template<class rClass,class StreamClass>
void idlFileTokeniser<rClass,StreamClass>::initTokenList()
{
	addToken(new identifierToken);

	addToken(new uuidToken);
	
	addToken(new dottedNumberToken);
	addToken(new numberToken);

	addToken(new doubleQuotedStringToken);

	addToken(new simpleToken("import"));
	addToken(new simpleToken("importlib"));
	addToken(new simpleToken("object"));
	addToken(new simpleToken("dual"));
	addToken(new simpleToken("helpstring"));
	addToken(new simpleToken("pointer_default"));
	addToken(new simpleToken("unique"));
	addToken(new simpleToken("version"));
	addToken(new simpleToken("oleautomation"));
	addToken(new simpleToken("interface"));
	addToken(new simpleToken("id"));
	addToken(new simpleToken("out"));
	addToken(new simpleToken("retval"));
	addToken(new simpleToken("default"));
	addToken(new simpleToken("in"));
	addToken(new simpleToken("cpp_quote"));
	addToken(new simpleToken("typedef"));
	addToken(new simpleToken("enum"));
	addToken(new simpleToken("propput"));
	addToken(new simpleToken("propget"));
	addToken(new simpleToken("library"));
	addToken(new simpleToken("coclass"));

	addToken(new simpleToken("HRESULT"));
	addToken(new simpleToken("BSTR"));
	addToken(new simpleToken("VARIANT"));
	addToken(new simpleToken("VARIANT_BOOL"));
	addToken(new simpleToken("long"));
	addToken(new simpleToken("DATE"));
	addToken(new simpleToken("void"));
	
	addToken(new SYMBOLToken(";"));
	addToken(new SYMBOLToken(":"));
	addToken(new SYMBOLToken(","));
	addToken(new SYMBOLToken("*"));
	addToken(new SYMBOLToken("."));
	addToken(new SYMBOLToken("="));
	addToken(new SYMBOLToken("-"));

	addToken(new SYMBOLToken("["));
	addToken(new SYMBOLToken("]"));
	addToken(new SYMBOLToken("("));
	addToken(new SYMBOLToken(")"));
	addToken(new SYMBOLToken("{"));
	addToken(new SYMBOLToken("}"));
}

#endif
