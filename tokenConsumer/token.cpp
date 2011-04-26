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
#include "token.h"

const int TOKEN_TYPE_UNDEF		=0x00;
const int TOKEN_TYPE_ID			=0x01;
const int TOKEN_TYPE_WORD		=0x02;
const int TOKEN_TYPE_SYM		=0x03;
const int TOKEN_TYPE_NUM		=0x04;
const int TOKEN_TYPE_DQ_STRING	=0x05;
const int TOKEN_TYPE_SQ_STRING	=0x06;
const int TOKEN_TYPE_DOTTED_NUM	=0x07;
