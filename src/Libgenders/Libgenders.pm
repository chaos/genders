package Libgenders;

use 5.006;
use strict;
use warnings;
use Carp;

require Exporter;
require DynaLoader;

our @ISA = qw(Exporter DynaLoader);

our $VERSION = '0.01';

bootstrap Libgenders $VERSION;

1;

__END__

# Below is stub documentation for your module. You better edit it!
# Ok, ok, I will!
 
=head1 NAME

Libgenders - Perl extension for libgenders

=head1 SYNOPSIS

 use Libgenders;

 Libgenders::GENDERS_ERR_SUCCESS
 Libgenders::GENDERS_ERR_NULLHANDLE
 Libgenders::GENDERS_ERR_OPEN
 Libgenders::GENDERS_ERR_READ
 Libgenders::GENDERS_ERR_PARSE
 Libgenders::GENDERS_ERR_NOTLOADED
 Libgenders::GENDERS_ERR_ISLOADED
 Libgenders::GENDERS_ERR_OVERFLOW
 Libgenders::GENDERS_ERR_PARAMETERS
 Libgenders::GENDERS_ERR_NULLPTR
 Libgenders::GENDERS_ERR_NOTFOUND
 Libgenders::GENDERS_ERR_OUTMEM
 Libgenders::GENDERS_ERR_MAGIC
 Libgenders::GENDERS_ERR_INTERNAL
 Libgenders::GENDERS_ERR_ERRNUMRANGE
 Libgenders::GENDERS_DEFAULT_FILE

 Libgenders::genders_handle_create
 Libgenders::genders_handle_destroy($handle)
 Libgenders::genders_load_data($handle, [$filename])

 Libgenders::genders_errnum($handle)
 Libgenders::genders_strerror($errnum)
 Libgenders::genders_errormsg($handle)
 Libgenders::genders_perror($handle, $msg)

 Libgenders::genders_handle_dump($handle)
 Libgenders::genders_getnumnodes($handle)
 Libgenders::genders_getnumattrs($handle)
 Libgenders::genders_getnodename($handle)

 Libgenders::genders_getnodes($handle, [$attr, [$val]])
 Libgenders::genders_getattr($handle, [$node])
 Libgenders::genders_getattr_all($handle)
 Libgenders::genders_getattrval($handle, $attr, [$node])
 Libgenders::genders_testattr($handle, $attr, [$node])
 Libgenders::genders_testattrval($handle, $attr, $val, [$node])

 Libgenders::genders_isnode($handle, [$node])
 Libgenders::genders_isattr($handle, $attr)
 Libgenders::genders_isattrval($handle, $attr, $val)

 Libgenders::genders_parse($handle, [$filename]);

=head1 DESCRIPTION

This package provides a perl interface to the genders C API (see
libgenders(3)).  The perl interface is nearly identical to the genders
C API, with some necessary changes due to the inherent differences
between C and perl.  Some of the functions from the C API cannot be
accessed via this perl interface, some new functions were created, the
behavior of some functions was modified, and the parameters passed
into the some functions was changed.

=head2 Initialization/Destruction

=over 4

=item B<Libgenders::genders_handle_create>

Returns a genders handle to be used throughout the genders API.
Returns a genders handle on success, undef on error.

=item B<Libgenders::genders_handle_destroy($handle)>

Destroys a genders handle. Returns 0 on success, -1 on error.

=item B<Libgenders::genders_load_data($handle, [$filename])>

Opens, reads, and parses the genders file specified by $filename.  If
$filename is not specified, the default genders file is parsed.
Returns 0 on success, -1 on error.

=back

=head2 Error Messages

Similarly to the C API, an error code is stored in the genders handle
after an error has occurred.  The following can be used to retrieve
the error code and output information about the error.  

=over 4

=item B<Libgenders::genders_errnum($handle)>

Returns the error code most recently set in the handle.  

=item B<Libgenders::genders_strerror($errnum)>

Returns a string describing the error code $errnum.  

=item B<Libgenders::genders_errormsg($handle)>

Returns a string describing the most recent error set in the handle.

=item B<Libgenders::genders_perror($handle, [$msg])>

Outputs $msg and a string describing the most recent error to standard
error.  If $msg is not specified, only a description of the most
recent error will be output to standard error.

=back

=head2 Utility Functions

=over 4

=item B<Libgenders::genders_handle_dump($handle)>

Dump the contents stored in the genders handle.  This is used
primarily for debugging purposes.  Returns 0 on success, -1 on error.

=item B<Libgenders::genders_getnumnodes($handle)>

Returns the number of nodes listed in the genders file.  Returns -1 on
error.

=item B<Libgenders::genders_getnumattrs($handle)>

Returns the number of attributes listed in the genders file.  Returns
-1 on error.

=item B<Libgenders::genders_getnodename($handle)>

Returns the hostname of the current node.  Returns undef on error.

=back

=head2 Parsing Functions

=item B<Libgenders::genders_getnodes($handle, [$attr, [$val]])>

Returns a reference to a list of nodes that have the specified
attribute and value.  If $val is not specified, only $attr is
considered.  If both $attr and $val are not specified, all nodes
listed in the genders file are returned.  Returns undef on error.

=item B<Libgenders::genders_getattr($handle, [$node])>

Returns a reference to an array that holds references to two lists.
The first list is a reference to attributes for the specified node.
The second list is a reference to values for the specified node.  If
$node is not specified, the local node is used.  Returns undef on
error.

=item B<Libgenders::genders_getattr_all($handle)>

Returns a reference to a list of all the attributes listed in the
genders file.  Returns undef on error.

=item B<Libgenders::genders_getattrval($handle, $attr, [$node])>

Returns the value of an attribute listed in a node.  Returns the empty
string if the attribute has no value.  If $node is not specified,
local node is used.  Returns undef on error.

=item B<Libgenders::genders_testattr($handle, $attr, [$node])>

Tests if a node has a specified attribute.  If $node is not specified,
local node is used.  Returns 1 if the node contains the attribute, 0
if not, -1 on error.

=item B<Libgenders::genders_testattrval($handle, $attr, $val, [$node])>

Tests if a node has a specified attribute=value pair.  If $node is not
specified, local node is used.  Returns 1 if the node contains the
attribute=value pair, 0 if not, -1 on error.

=item B<Libgenders::genders_isnode($handle, [$node])>

Tests if a node is listed in the genders file.  If $node is not
specified, local node is used.  Returns 1 if the node is listed, 0 if
it is not, -1 on error.

=item B<Libgenders::genders_isattr($handle, $attr)>

Tests if the attribute $attr is listed in the genders file.  Returns 1
if the attribute is listed, 0 if it is not, -1 on error.

=item B<Libgenders::genders_isattrval($handle, $attr, $val)>

Tests if the attribute=value pair is listed in the genders file.
Returns 1 if the pair is listed, 0 if it is not, -1 on error.

=item B<Libgenders::genders_parse($handle, [$filename])>

Parse a genders file and output parse errors to standard error.  If
$filename is not specified, the default genders file is parsed.
Returns the number of errors (0 if no parse errors were found) on
success, -1 on error.

=back

=head2 Error Codes/Constants

The same error codes and constants listed in /usr/include/genders.h
can be accessed through the following: 

 Libgenders::GENDERS_ERR_SUCCESS
 Libgenders::GENDERS_ERR_NULLHANDLE
 Libgenders::GENDERS_ERR_OPEN
 Libgenders::GENDERS_ERR_READ
 Libgenders::GENDERS_ERR_PARSE
 Libgenders::GENDERS_ERR_NOTLOADED
 Libgenders::GENDERS_ERR_ISLOADED
 Libgenders::GENDERS_ERR_OVERFLOW
 Libgenders::GENDERS_ERR_PARAMETERS
 Libgenders::GENDERS_ERR_NULLPTR
 Libgenders::GENDERS_ERR_NOTFOUND
 Libgenders::GENDERS_ERR_OUTMEM
 Libgenders::GENDERS_ERR_MAGIC
 Libgenders::GENDERS_ERR_INTERNAL
 Libgenders::GENDERS_ERR_ERRNUMRANGE
 Libgenders::GENDERS_DEFAULT_FILE

=head1 AUTHOR

Albert Chu E<lt>chu11@llnl.govE<gt>

=head1 SEE ALSO

L<libgenders>

=cut
