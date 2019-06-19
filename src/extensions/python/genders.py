"""
Genders database parsing and querying
"""
#############################################################################
#  Copyright (C) 2007-2019 Lawrence Livermore National Security, LLC.
#  Copyright (C) 2001-2007 The Regents of the University of California.
#  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
#  Written by Jim Garlick <garlick@llnl.gov> and Albert Chu <chu11@llnl.gov>.
#  UCRL-CODE-2003-004.
#
#  This file is part of Genders, a cluster configuration database.
#  For details, see <http://www.llnl.gov/linux/genders/>.
#
#  Genders is free software; you can redistribute it and/or modify it under
#  the terms of the GNU General Public License as published by the Free
#  Software Foundation; either version 2 of the License, or (at your option)
#  any later version.
#
#  Genders is distributed in the hope that it will be useful, but WITHOUT ANY
#  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
#  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
#  details.
#
#  You should have received a copy of the GNU General Public License along
#  with Genders.  If not, see <http://www.gnu.org/licenses/>.
#############################################################################

from __future__ import print_function
import sys
import libgenders

class Genders_Err(Exception):
    """
    Genders Error Exception Base Class
    """
    def __repr__(self):
        return "Genders Error"

class Genders_Err_Open(Genders_Err):
    """
    Genders Open Error Exception 
    """
    def __repr__(self):
        return "error opening genders file"

class Genders_Err_Read(Genders_Err):
    """
    Genders Read Error Exception 
    """
    def __repr__(self):
        return "error reading genders file"

class Genders_Err_Parse(Genders_Err):
    """
    Genders Parse Error Exception 
    """
    def __repr__(self):
        return "genders file parse error"

class Genders_Err_NotFound(Genders_Err):
    """
    Genders NotFound Error Exception 
    """
    def __repr__(self):
        return "node or attribute not found"

class Genders_Err_Syntax(Genders_Err):
    """
    Genders Syntax Error Exception 
    """
    def __repr__(self):
        return "query syntax error"

class Genders_Err_Internal(Genders_Err):
    """
    Genders Internal Error Exception 
    """
    def __repr__(self):
        return "unknown internal error"

class Genders:
    """
    Genders database parsing and querying 
    """
    __lgh = None;

    def __find_exception(self):
        # When SystemError occurs, assume it's a genders error, map to
        # appropriate genders issue.
        if self.__lgh.errnum() == self.__lgh.genders_err_open:
            return Genders_Err_Open()
        elif self.__lgh.errnum() == self.__lgh.genders_err_read:
            return Genders_Err_Read()
        elif self.__lgh.errnum() == self.__lgh.genders_err_parse:
            return Genders_Err_Parse()
        elif self.__lgh.errnum() == self.__lgh.genders_err_notfound:
            return Genders_Err_NotFound()
        elif self.__lgh.errnum() == self.__lgh.genders_err_syntax:
            return Genders_Err_Syntax()
        elif self.__lgh.errnum() == self.__lgh.genders_err_internal:
            return Genders_Err_Internal()
        else:
            return Genders_Err()
    def __init__(self, filename=None):
        """
        Creates a Genders object and load genders data from the
        specified file.  If the genders file is not specified, the
        default genders file will be used.       
        """
        self.__lgh = libgenders.Libgenders()
        try:
            self.__lgh.load_data(filename)
        except SystemError:
            raise Genders.__find_exception(self)
    def getnodename(self):
        """
        Returns the name of the current node.
        """
        try:
            return self.__lgh.getnodename()
        except SystemError:
            raise Genders.__find_exception(self)
    def getnodes(self, attr=None, val=None):
        """
        Returns a list of nodes with the specified attribute and
        value.  If a value is not specified only the attribute is
        considered.  If the attribute is not specified, all nodes
        listed in the genders file are returned.       
        """
        try:
            str = self.__lgh.getnodes(attr, val)
        except SystemError:
            raise Genders.__find_exception(self)

        if str:
            rv = str.split(',')
        else:
            rv = [];
        return rv
    def getattr(self, node=None):
        """
        Returns a list of attributes for the specified node.  If the
        node is not specified, the local node's attributes returned.
        """
        try:
            str = self.__lgh.getattr(node)
        except SystemError:
            raise Genders.__find_exception(self)
           
        if str:
            rv = str.split(',')
        else:
            rv = []
        return rv
    def getattr_all(self):
        """
        Returns a list of all attributes listed in the genders file.
        """
        try:
            str = self.__lgh.getattr_all()
        except SystemError:
            raise Genders.__find_exception(self)
            
        if str:
            rv = str.split(',')
        else:
            rv = []
        return rv
    def getattrval(self, attr, node=None):
        """
        Returns the value of the specified attribute for the specified
        node.  If the attribute does not exist or the attribute has no
        value, an empty string is returned.  If the node is not
        specified, the local node's attribute value is returned.
        """
        try:
            return self.__lgh.getattrval(attr, node)
        except SystemError:
            raise Genders.__find_exception(self)
    def testattr(self, attr, node=None):
        """
        Returns 1 if the specified node has the specified attribute, 0
        if it does not.  If the node is not specified, the local node
        is checked.       
        """
        try:
            return self.__lgh.testattr(attr, node)
        except SystemError:
            raise Genders.__find_exception(self)
    def testattrval(self, attr, val, node=None):
        """
        Returns 1 if the specified node has the specified attribute
        and value, 0 if it does not.  If the node is not specified,
        the local node is checked.       
        """
        try:
            return self.__lgh.testattrval(attr, val, node)
        except SystemError:
            raise Genders.__find_exception(self)
    def isnode(self, node=None):
        """
        Returns 1 if the specified node is listed in the genders file,
        0 if it is not.  If the node is not specified, the local node
        is checked.        
        """
        try:
            return self.__lgh.isnode(node)
        except SystemError:
            raise Genders.__find_exception(self)
    def isattr(self, attr):
        """
        Returns 1 if the specified attribute is listed in the genders
        file, 0 if it is not.        
        """
        try:
            return self.__lgh.isattr(attr)
        except SystemError:
            raise Genders.__find_exception(self)
    def isattrval(self, attr, val):
        """
        Returns 1 if the specified attribute is equal to the specified
        value for some node in the genders file, 0 if it is not.       
        """
        try:
            return self.__lgh.isattrval(attr, val)
        except SystemError:
            raise Genders.__find_exception(self)
    def query(self, query=None):
        """
        Returns a list of nodes specified by a genders query.  A
        genders query is based on the union, intersection, set
        difference, or complement between genders attributes and
        values.  Union is represented by two pipe symbols ('||'),
        intersection by two ampersand symbols ('&&'), difference by
        two minus symbols ('--'), and complement by a tilde ('~')
        Operations are performed from left to right.  Parentheses may
        be used to change the order of operations.  For example, the
        following query would retrieve all nodes other than management
        or login nodes: "all-(mgmt+login)".  If the query is not
        specified, all nodes listed in the genders file are returned.  
        """
        try:
            str = self.__lgh.query(query)
        except SystemError:
            raise Genders.__find_exception(self)

        if str:
            rv = str.split(',')
        else:
            rv = [];
        return rv
    def testquery(self, query, node=None):
        """
        Returns 1 if the specified node meets the conditions of the
        specified query, 0 if it does not.  If the node is not
        specified, the local node is checked.           
        """
        try:
            return self.__lgh.testquery(query, node)
        except SystemError:
            raise Genders.__find_exception(self)

if __name__ == '__main__':

    gh = Genders()

    print("getnodename:", gh.getnodename())

    print("getnodes:", gh.getnodes())
    print("getnodes:", gh.getnodes("foofdfd"))
    print("getnodes:", gh.getnodes("mgmt"))
    print("getattr:", gh.getattr())

    try:
        print("getattr:", gh.getattr("fdafdsfdsa"))
    except Genders_Err_NotFound:
        print("got genders exception", sys.exc_info()[0], sys.exc_info()[1])
    except:
        print("unexpected exception:", sys.exc_info()[0], sys.exc_info()[1], sys.exc_info[2])

    print("getattr_all:", gh.getattr_all())
    print("isnode <blank>:", gh.isnode())
    print("isnode foo:", gh.isnode("foo"))
    print("isattr foo:", gh.isattr("foo"))
    print("isattr mgmt:", gh.isattr("mgmt"))
    print("isattrval cpu=14:", gh.isattrval("cpu", "14"))
    print("isattrval cpu=14:", gh.isattrval("cpu", "16"))
    print("query:", gh.query("mgmt"))
    print("query:", gh.query("mgmt||login"))
    print("query:", gh.query())
    print("query:", gh.query("bdjfkdsalfdsafds"))

    try:
        print("query:", gh.query("&&||!!!~~"))
    except Genders_Err_Syntax:
        print("got genders exception", sys.exc_info()[0], sys.exc_info()[1])
    except:
        print("unexpected exception:", sys.exc_info()[0], sys.exc_info()[1], sys.exc_info[2])
