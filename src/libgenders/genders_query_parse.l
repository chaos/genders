%{
/*****************************************************************************\
 *  $Id: genders_query_parse.l,v 1.10 2010-03-24 21:26:05 chu11 Exp $
 *****************************************************************************
 *  Copyright (C) 2007-2019 Lawrence Livermore National Security, LLC.
 *  Copyright (C) 2001-2007 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Jim Garlick <garlick@llnl.gov> and Albert Chu <chu11@llnl.gov>.
 *  UCRL-CODE-2003-004.
 *
 *  This file is part of Genders, a cluster configuration database.
 *  For details, see <http://www.llnl.gov/linux/genders/>.
 *
 *  Genders is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  Genders is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with Genders.  If not, see <http://www.gnu.org/licenses/>.
\*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "genders_query.tab.h"

/* Regex notes:

  Special chars "-", "|", "&", by themselves must be followed by a
  alphanumeric character, otherwise we can't tell what is an
  attribute and what is a set operation.  For example, the query parser
  may get confused with an attribute "attr1&" in a query such as
  "attr1&&&attr2".
 */

%}

%%
[a-zA-Z0-9][a-zA-Z0-9_\.\=:%\\\/\+]*([\-\|&]?[a-zA-Z0-9_\.\=:%\\\/\+]+)* yylval.attr = strdup(yytext); return ATTRTOK;
\(                                                                       return LPARENTOK;
\)                                                                       return RPARENTOK;
\|\|                                                                     return UNIONTOK;
&&                                                                       return INTERSECTIONTOK;
--                                                                       return DIFFERENCETOK;
~                                                                        return COMPLEMENTTOK;
[ \t\n]+                                                                 ; /* ignore whitespace */
%%
