;#############################################################################
# $Id: Genders.pm,v 1.1 2003-05-10 00:06:13 achu Exp $
# $Source: /g/g0/achu/temp/genders-cvsbackup-full/genders/src/Genders/Genders.pm,v $
#############################################################################

package Genders;

use strict;
use Libgenders;

our $VERSION = "2.0";

require Exporter;
our @ISA = qw(Exporter);
our @EXPORT = qw(_errormsg);
our @EXPORT_OK = qw(_errormsg);
our %EXPORT_TAGS = ( 'all' => [ qw(_errormsg) ] );

sub _errormsg {
    my $self = shift;
    my $msg = shift;
    my $str;

    if ($self->{"_DEBUG"}) {
        $str = $self->{"_HANDLE"}->genders_errormsg();
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
    
    $self->{"_DEBUG"} = 0;
    
    $handle = Libgenders->genders_handle_create();
    if (!defined($handle)) {
        _errormsg($self, "genders_handle_create()");
        return undef;
    }

    $self->{"_HANDLE"} = $handle;

    $ret = $self->{"_HANDLE"}->genders_load_data($filename);
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
            $self->{"_DEBUG"} = $num;
        }
    }
}

sub getnodename {
    my $self = shift;
    my $node;

    if (ref($self)) {
        $node = $self->{"_HANDLE"}->genders_getnodename();
        if (!defined($node)) {
            _errormsg($self, "genders_getnodename()");
            return "";
        }
        return $node;
    }
    else {
        return undef;
    }
}

sub getnodes {
    my $self = shift;
    my $attr = shift;
    my $val = shift;
    my $nodes;

    if (ref($self)) {
        $nodes = $self->{"_HANDLE"}->genders_getnodes($attr, $val);
        if (!defined($nodes)) {
            _errormsg($self, "genders_getnodes()");
            return ();
        }
        
        return @$nodes;
    }
    else {
        return undef;
    }
}

sub getattr {
    my $self = shift;
    my $node = shift;
    my $attrsvals;
    my $attrs;

    if (ref($self)) {
        $attrsvals = $self->{"_HANDLE"}->genders_getattr($node);
        if (!defined($attrsvals)) {
            _errormsg($self, "genders_getattr()");
            return ();
        }
        
        ($attrs) = @$attrsvals;

        return @$attrs;
    }
    else {
        return undef;
    }
}

sub getattrval {
    my $self = shift;
    my $attr = shift;
    my $node = shift;
    my $val;

    if (ref($self)) {
        $val = $self->{"_HANDLE"}->genders_getattrval($attr, $node);
        if (!defined($val)) {
            _errormsg($self, "genders_getattrval()");
            return "";
        }
        
        return $val;
    }
    else {
        return undef;
    }
}

sub getattr_all {
    my $self = shift;
    my $attrs;

    if (ref($self)) {
        $attrs = $self->{"_HANDLE"}->genders_getattr_all();
        if (!defined($attrs)) {
            _errormsg($self, "genders_getattr_all()");
            return ();
        }
       
        return @$attrs;
    }
    else {
        return undef;
    }
}

sub testattr {
    my $self = shift;
    my $attr = shift;
    my $node = shift;
    my $retval;

    if (ref($self)) {
        $retval = $self->{"_HANDLE"}->genders_testattr($attr, $node);
        if ($retval == -1) {
            _errormsg($self, "genders_testattr()");
            return 0;
        }
        return $retval; 
    }
    else {
        return undef;
    }
}

sub testattrval {
    my $self = shift;
    my $attr = shift;
    my $val = shift;
    my $node = shift;
    my $retval;

    if (ref($self)) {
        $retval = $self->{"_HANDLE"}->genders_testattrval($attr, $val, $node);
        if ($retval == -1) {
            _errormsg($self, "genders_testattrval()");
            return 0;
        }
        return $retval; 
    }
    else {
        return undef;
    }
}

sub isnode {
    my $self = shift;
    my $node = shift;
    my $retval;

    if (ref($self)) {
        $retval = $self->{"_HANDLE"}->genders_isnode($node);
        if ($retval == -1) {
            _errormsg($self, "genders_isnode()");
            return 0;
        }
        return $retval; 
    }
    else {
        return undef;
    }
}

sub isattr{
    my $self = shift;
    my $attr = shift;
    my $retval;

    if (ref($self)) {
        $retval = $self->{"_HANDLE"}->genders_isattr($attr);
        if ($retval == -1) {
            _errormsg($self, "genders_isattr()");
            return 0;
        }
    }
    else {
        return undef;
    }
}

sub isattrval {
    my $self = shift;
    my $attr = shift;
    my $val = shift;
    my $retval;

    if (ref($self)) {
        $retval = $self->{"_HANDLE"}->genders_isattrval($attr, $val);
        if ($retval == -1) {
            _errormsg($self, "genders_isattrval()");
            return 0;
        }
    }
    else {
        return undef;
    }
}

1;

__END__


=head1 NAME

Genders - Perl library for querying genders file

=head1 SYNOPSIS

 use Genders;

 $obj = Genders->new()

 $obj->debug($num)

 $obj->genders_getnodes([$attr, [$val]])
 $obj->genders_getattr([$node])
 $obj->genders_getattr_all()
 $obj->genders_getattrval($attr, [$node])

 $obj->genders_testattr($attr, [$node])
 $obj->genders_testattrval($attr, $val, [$node])

 $obj->genders_isnode([$node])
 $obj->genders_isattr($attr)
 $obj->genders_isattrval($attr, $val)

=head1 DESCRIPTION

This package provides a perl interface for querying a genders file.

=over 4

=item B<Genders->new()>

Creates a genders object.

=item B<$obj-E<gt>debug($num)>

Set the debug level in the genders object.  By default, the debug
level is 0 and all debugging is turned off.  To turn it on, set the
level to 1.

=item B<$obj-E<gt>genders_getnodes([$attr, [$val]])>

Returns a list of nodes with the specified attribute and value.  If a
value is not specified only the attribute is considered.  If the
attribute is not specified, all nodes listed in the genders file are
returned.

=item B<$obj-E<gt>genders_getattr([$node])>

Returns a list of attributes for the specified node.  If the node
is not specified, the local node's attributes returned.

=item B<$obj-E<gt>genders_getattr_all()>

Returns a list of all attributes listed in the genders file.

=item B<$obj-E<gt>genders_getattrval($attr, [$node])>

Returns the value of the specified attribute for the specified node.
If the node is not specified, the local node's attribute value is
returned.

=item B<$obj-E<gt>genders_testattr($attr, [$node])>

Returns 1 if the specified node has the specified attribute, 0 if it
does not.  If the node is not specified, the local node is checked.

=item B<$obj-E<gt>genders_testattrval($attr, $val, [$node])>

Returns 1 if the specified node has the specified attribute and value,
0 if it does not.  If the node is not specified, the local node is
checked.

=item B<$obj-E<gt>genders_isnode([$node])>

Returns 1 if the specified node is listed in the genders file, 0 if it
is not.  If the node is not specified, the local node is checked.

=item B<$obj-E<gt>genders_isattr($attr)>

Returns 1 if the specified attribute is listed in the genders file, 0
if it is not.

=item B<$obj-E<gt>genders_isattrval($attr, $val)>

Returns 1 if the specified attribute is equal to the specified value
for some node in the genders file, 0 if it is not.

=back 

=head1 AUTHOR

Albert Chu E<lt>chu11@llnl.govE<gt>

=head1 SEE ALSO

L<Libgenders>.

L<libgenders>.
