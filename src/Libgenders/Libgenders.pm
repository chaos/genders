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

=head1 DESCRIPTION

This package provides a perl interface to the genders C API (see
libgenders(3)).  The perl interface is nearly identical to the genders
C API, however some changes were necessary due to the inherent
differences between C and perl.  Some of the functions from the C API
cannot be accessed via this perl interface, some new functions were
created, the behavior of some functions was modified, and the parame-
ters passed into the some functions was changed.

Stub documentation for Libgenders, created by h2xs. It looks like the
author of the extension was negligent enough to leave the stub
unedited.

Blah blah blah.

=head1 AUTHOR

Albert Chu E<lt>chu11@llnl.govE<gt>

=head1 SEE ALSO

libgenders(3)

=cut
