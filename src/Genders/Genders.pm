#############################################################################
#  $Id: Genders.pm,v 1.17 2004-10-07 18:22:31 achu Exp $
#############################################################################
#  Copyright (C) 2001-2003 The Regents of the University of California.
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
#  with Genders; if not, write to the Free Software Foundation, Inc.,
#  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
#############################################################################

package Genders;

use strict;
use Libgenders;

our $VERSION = "0.01";

require Exporter;
our @ISA = qw(Exporter);
our @EXPORT_OK = qw(&_errormsg $GENDERS_DEFAULT_FILE $debugkey $handlekey);
our %EXPORT_TAGS = ( 'all' => [ qw(&_errormsg 
                                   $GENDERS_DEFAULT_FILE 
                                   $debugkey 
                                   $handlekey) ] );

our $GENDERS_DEFAULT_FILE = Libgenders->GENDERS_DEFAULT_FILE;
our $debugkey = "_DEBUG";
our $handlekey = "_HANDLE";

sub _errormsg {
    my $self = shift;
    my $msg = shift;
    my $str;

    if ($self->{$debugkey}) {
        $str = $self->{$handlekey}->genders_errormsg();
        print STDERR "Error: $msg, $str\n";
    }
}

sub new {
    my $proto = shift;
    my $class = ref($proto) || $proto;
    my $filename = shift;
    my $self = {};
    my $handle;
    my $ret;
    
    $self->{$debugkey} = 0;
    
    $handle = Libgenders->genders_handle_create();
    if (!defined($handle)) {
        _errormsg($self, "genders_handle_create()");
        return undef;
    }

    $self->{$handlekey} = $handle;

    $ret = $self->{$handlekey}->genders_load_data($filename);
    if ($ret == -1) {
        _errormsg($self, "genders_load_data()");
        return undef;
    } 

    bless ($self, $class);
    return $self;
}

sub debug {
    my $self = shift;
    my $num = shift;

    if (ref($self)) {
        if (defined $num) {
            $self->{$debugkey} = $num;
        }
    }
}

sub getnodename {
    my $self = shift;
    my $node;

    if (ref($self)) {
        $node = $self->{$handlekey}->genders_getnodename();
        if (!defined($node)) {
            _errormsg($self, "genders_getnodename()");
            return "";
        }
        return $node;
    }
    else {
        return "";
    }
}

sub getnodes {
    my $self = shift;
    my $attr = shift;
    my $val = shift;
    my $nodes;

    if (ref($self)) {
        $nodes = $self->{$handlekey}->genders_getnodes($attr, $val);
        if (!defined($nodes)) {
            _errormsg($self, "genders_getnodes()");
            return ();
        }
        return @$nodes;
    }
    else {
        return ();
    }
}

sub getattr {
    my $self = shift;
    my $node = shift;
    my $attrsvals;
    my $attrs;

    if (ref($self)) {
        $attrsvals = $self->{$handlekey}->genders_getattr($node);
        if (!defined($attrsvals)) {
            _errormsg($self, "genders_getattr()");
            return ();
        }
        ($attrs) = @$attrsvals;
        return @$attrs;
    }
    else {
        return ();
    }
}

sub getattrval {
    my $self = shift;
    my $attr = shift;
    my $node = shift;
    my $val;

    if (ref($self)) {
        $val = $self->{$handlekey}->genders_getattrval($attr, $node);
        if (!defined($val)) {
            _errormsg($self, "genders_getattrval()");
            return "";
        }
        return $val;
    }
    else {
        return "";
    }
}

sub getattr_all {
    my $self = shift;
    my $attrs;

    if (ref($self)) {
        $attrs = $self->{$handlekey}->genders_getattr_all();
        if (!defined($attrs)) {
            _errormsg($self, "genders_getattr_all()");
            return ();
        }
        return @$attrs;
    }
    else {
        return ();
    }
}

sub testattr {
    my $self = shift;
    my $attr = shift;
    my $node = shift;
    my $retval;

    if (ref($self)) {
        $retval = $self->{$handlekey}->genders_testattr($attr, $node);
        if ($retval == -1) {
            _errormsg($self, "genders_testattr()");
            return 0;
        }
        return $retval; 
    }
    else {
        return 0;
    }
}

sub testattrval {
    my $self = shift;
    my $attr = shift;
    my $val = shift;
    my $node = shift;
    my $retval;

    if (ref($self)) {
        $retval = $self->{$handlekey}->genders_testattrval($attr, $val, $node);
        if ($retval == -1) {
            _errormsg($self, "genders_testattrval()");
            return 0;
        }
        return $retval; 
    }
    else {
        return 0;
    }
}

sub isnode {
    my $self = shift;
    my $node = shift;
    my $retval;

    if (ref($self)) {
        $retval = $self->{$handlekey}->genders_isnode($node);
        if ($retval == -1) {
            _errormsg($self, "genders_isnode()");
            return 0;
        }
        return $retval; 
    }
    else {
        return 0;
    }
}

sub isattr{
    my $self = shift;
    my $attr = shift;
    my $retval;

    if (ref($self)) {
        $retval = $self->{$handlekey}->genders_isattr($attr);
        if ($retval == -1) {
            _errormsg($self, "genders_isattr()");
            return 0;
        }
        return $retval;
    }
    else {
        return 0;
    }
}

sub isattrval {
    my $self = shift;
    my $attr = shift;
    my $val = shift;
    my $retval;

    if (ref($self)) {
        $retval = $self->{$handlekey}->genders_isattrval($attr, $val);
        if ($retval == -1) {
            _errormsg($self, "genders_isattrval()");
            return 0;
        }
        return $retval;
    }
    else {
        return 0;
    }
}

sub index_attrvals {
    my $self = shift;
    my $attr = shift;

    if (ref($self)) {
        $self->{$handlekey}->genders_index_attrvals($attr);
    }
}

sub query {
    my $self = shift;
    my $query = shift;
    my $nodes;

    if (ref($self)) {
        $nodes = $self->{$handlekey}->genders_query($query);
        if (!defined($nodes)) {
            _errormsg($self, "genders_query()");
            return ();
        }
        return @$nodes;
    }
    else {
        return ();
    }
}

1;

__END__


=head1 NAME

Genders - Perl library for querying a genders file

=head1 SYNOPSIS

 use Genders;

 $Genders::GENDERS_DEFAULT_FILE;

 $obj = Genders->new([$filename])

 $obj->debug($num)

 $obj->getnodename()
 $obj->getnodes([$attr, [$val]])
 $obj->getattr([$node])
 $obj->getattr_all()
 $obj->getattrval($attr, [$node])

 $obj->testattr($attr, [$node])
 $obj->testattrval($attr, $val, [$node])

 $obj->isnode([$node])
 $obj->isattr($attr)
 $obj->isattrval($attr, $val)

 $obj->index_attrvals($attr)

 $obj->query($query)

=head1 DESCRIPTION

This package provides a perl interface for querying a genders file.

=over 4

=item B<Genders-E<gt>new([$filename])>

Creates a Genders object and load genders data from the specified
file.  If the genders file is not specified, the default genders file
will be used.  Returns undef if file cannot be read.

=item B<$obj-E<gt>debug($num)>

Set the debug level in the genders object.  By default, the debug
level is 0 and all debugging is turned off.  To turn it on, set the
level to 1.

=item B<$obj-E<gt>getnodename()>

Returns the name of the current node.

=item B<$obj-E<gt>getnodes([$attr, [$val]])>

Returns a list of nodes with the specified attribute and value.  If a
value is not specified only the attribute is considered.  If the
attribute is not specified, all nodes listed in the genders file are
returned.

=item B<$obj-E<gt>getattr([$node])>

Returns a list of attributes for the specified node.  If the node
is not specified, the local node's attributes returned.

=item B<$obj-E<gt>getattr_all()>

Returns a list of all attributes listed in the genders file.

=item B<$obj-E<gt>getattrval($attr, [$node])>

Returns the value of the specified attribute for the specified node.
If the attribute does not exist or the attribute has no value, an
empty string is returned.  If the node is not specified, the local
node's attribute value is returned.

=item B<$obj-E<gt>testattr($attr, [$node])>

Returns 1 if the specified node has the specified attribute, 0 if it
does not.  If the node is not specified, the local node is checked.

=item B<$obj-E<gt>testattrval($attr, $val, [$node])>

Returns 1 if the specified node has the specified attribute and value,
0 if it does not.  If the node is not specified, the local node is
checked.

=item B<$obj-E<gt>isnode([$node])>

Returns 1 if the specified node is listed in the genders file, 0 if it
is not.  If the node is not specified, the local node is checked.

=item B<$obj-E<gt>isattr($attr)>

Returns 1 if the specified attribute is listed in the genders file, 0
if it is not.

=item B<$obj-E<gt>isattrval($attr, $val)>

Returns 1 if the specified attribute is equal to the specified value
for some node in the genders file, 0 if it is not.

=item B<$obj-E<gt>index_attrvals($attr)>

Internally indexes genders attribute values for faster search times.
Subsequent calls with a different attribute will overwrite earlier
indexes.

=item B<$obj-E<gt>query($query)>

Query the genders database based on the union, intersection, or set
difference between genders attributes and values.  Use plus ('|') for
union, comma ('&') for intersection and minus ('-') for set
difference.

=back 

=head1 AUTHOR

Albert Chu E<lt>chu11@llnl.govE<gt>

=head1 SEE ALSO

L<Libgenders>.

L<libgenders>.
