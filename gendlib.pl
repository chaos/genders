#############################################################################
#  $Id: gendlib.pl,v 1.7 2003-03-31 22:28:47 achu Exp $
#############################################################################
#  Copyright (C) 2001-2002 The Regents of the University of California.
#  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
#  Written by Jim Garlick <garlick@llnl.gov>.
#  UCRL-CODE-2003-004.
#  
#  This file is part of Genders, a cluster configuration database and
#  rdist preprocessor.
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
use Sys::Hostname;
use vars qw($included $debug $havePSSP $path_genders $path_clusters);
use vars qw($path_hostname %attrs %nodes %xnodes @clusters $init_called);
use vars qw($init_hname_called $init_clusters_called $altAttr $hname);

if (!$included) {
$included = 1;

##
## Package globals
##

$debug = 		0;

# See note in to_altnames().
$havePSSP = 		(-d "/usr/lpp/ssp");
if ($havePSSP) {
	require "/admin/lib/sdrlib.pl";
} else {
	$altAttr =	"altname";
}

$path_genders = 	"/etc/genders";
$path_clusters = 	"/etc/clusters";
$path_hostname =	"/bin/hostname";

%attrs = 		();	# attributes by node
%nodes = 		();	# nodes by attribute
%xnodes = 		();	# nodes by attribute with =val

@clusters =		();	# first item is my cluster or null,
				# remaining items are other clusters

$hname =		""; 	# short hostname

$init_called = 0;		# needed for self-initialization
$init_hname_called = 0;	
$init_clusters_called = 0;

##
## Subroutines
##

# initialize package
#	$path (IN)	[optional] path to genders file
#	$rv (RETURN)	0 on failure opening genders, 1 on success
sub init
{
	my (@alist, $attr, $node, $blob, @gendfiles, $gendfile);
	if (@_) {
		@gendfiles = @_;
	} else {	
		@gendfiles = ($path_genders);
	}

	$init_called = 1;
	$debug && print("init called\n");

	# zero hashes
	%attrs = ();
	%nodes = ();
	%xnodes = ();

	foreach $gendfile (@gendfiles) {
	
		# populate hashes
		if (!open(GEND, $gendfile)) {
			printf STDERR ("Error: could not open %s\n", $gendfile);
			next;
		}
		while (<GEND>) {
			chomp;			# delete trailing \n, if any
			s/\#.*$//;		# strip comments
			next if (/^\s*$/);	# skip blank lines
			($node, $blob) = split;	# blank separates node and alist
						# commas separate attrs in list
			@alist = split /,/, $blob; 

						# add to %attrs
			@{$attrs{$node}} = @alist;

			foreach $attr (@alist) {
				push @{$xnodes{$attr}}, $node;
				$attr =~ s/=.*//; # drop any =value on attribute
				push @{$nodes{$attr}}, $node;
			}
		}
		close(GEND);
	}

	# initialize hostname
	if (!$init_hname_called) {
		init_hname();
	}

	return(1);
}

# cache the local hostname
sub init_hname
{
	$init_hname_called = 1;
	$debug && print("init_hname called\n");

	# get 'my' hostname
	# oddly enough, running `hostname` seems to be faster than
	# calling hostname() from Sys::hostname.
	$hname = `$path_hostname -s`;  chomp($hname);
}


# determine if host has attribute
#	$attr (IN)	attribute
#	$node (IN)	[optional] hostname
#	$found (RETURN)	0 if not found, 1 if found
sub hasattr
{
	if (!$init_called) {
		init();
	}

	my $attr = shift(@_);
	my $node = (@_) ? shift(@_) : $hname;

	my ($xattr);

	if (defined($attrs{$node})) {
		foreach $xattr (@{$attrs{$node}}) {
			if ($xattr eq $attr || $xattr =~ /^$attr=.*/) {
				return $xattr;
			}
		}
	}
	return 0;
}

# return value of attribute held by host
#	$attr (IN)	attribute
#	$node (IN)	[optional] hostname
#	$value (RETURN)	value or null if (no value or node does not have attr)
sub getattrval
{
	if (!$init_called) {
		init();
	}

	my $attr = shift(@_);
	my $node = (@_) ? shift(@_) : $hname;

	my ($xattr);

	if (defined($attrs{$node})) {
		foreach $xattr (@{$attrs{$node}}) {
			if ($xattr =~ /^$attr=(.*)/) {
				return $1;
			}
		}
	}
	return "";
}

# get list of attributes held by node
#	$node (IN)	[optional] hostname
#	@attrs (RETURN)	list of attributes
sub getattr
{
	if (!$init_called) {
		init();
	}

	my $node = (@_) ? shift(@_) : $hname;

	if (defined($attrs{$node})) {
		return @{$attrs{$node}};
	} else {
		return ();
	}
}

# get all attributes in genders file
#	@attrs (RETURN)	list of attributes
sub getallattr
{
	if (!$init_called) {
		init();
	}

	my ($attr, @attrs);

        foreach $attr (keys %nodes) {
		push (@attrs, $attr);
        }
	return @attrs;
}

# get list of nodes that have attribute
#	$attr (IN)	attribute
#	@nodes (RETURN)	list of nodes
sub getnode
{
	if (!$init_called) {
		init();
	}

	my $attr = shift(@_);
	my (@rv);

	if ($attr =~ /=/) {
		if (defined($xnodes{$attr})) {
			@rv = @{$xnodes{$attr}};
		} else {
			@rv = ();
		}
	} else {
		if (defined($nodes{$attr})) {
			@rv = @{$nodes{$attr}};
		} else {
			@rv = ();
		}
	}
	return @rv;
}

# Get a copy of hash of attributes -> node lists.  
# 	\%nodes (OUT)  	node hash
sub get_node_hash
{
	my ($nodes) = (@_);

	my ($attr);

	if (!$init_called) {
		init();
	}

	%{$nodes} = %nodes;
}

# initialize list of clusters
#	$path (IN)	[optional] path to clusters file
#	$rv (RETURN)	0 on failure opening attributes, 1 on success
sub init_clusters
{
        # clusters file now removed, just return 1
        return(1);
}

# get a copy of the list of clusters
#       $rv (RETURN)    "" on failure (from getattrval()), cluster name on success
sub get_clusters
{
        if (!@init_called) {
 	        init();
	}
	return (getattrval("cluster"));
}


# evaluate expression involving genders attributes
# (assume any non-numeric token is a genders attribute;  turn this into
# a boolean variable with value assigned by hasattr(), then evaluate
# the expression and return the result)
# 	$exp (IN)	expression to evaluate
#	$node (IN)	[optional] hostname, use local host if unspecified
#	$rv (RETURN)	result of expression evaluation
sub gendexp
{
	if (!$init_called) {
		init();
	}

	my $exp = shift(@_);
	my $node = (@_) ? shift(@_) : $hname; 

	my ($var, $pro, %vars);

	if (!$exp) {
		return $exp;
	}

	$pro = "";
	foreach $var (split(/[\!\+\-\*\/(\s\(\&\|)]+/, $exp)) {
		$var =~ s/\s+//g;
		next if (!$var || $var =~ /^[0-9]+$/);
		if (hasattr($var, $node)) {
			$pro .= "my \$$var = 1; ";
		} else {
			$pro .= "my \$$var = 0; ";
		}
		$vars{$var}++;
	}
	foreach $var (keys %vars) {
		$exp =~ s/$var/\$$var/g;
	}
	if ($debug) {
		printf("evaluating { %s } for host %s\n", $pro . $exp, $node);
	}
	no strict;
	return(eval($pro . $exp));
	use strict;
}

# Convert "genders names" to an alternate names.  On an SP, the genders
# name is assumed to be the SDR initial_hostname, and the alternate name
# is the reliable_hostname.  On other systems, the alternate hostname is
# stored in the genders for each node as the value of the "altname" attribute.
#	@inList (IN)	list of genders names
#	RETURN		list of alternate names
# NOTE: names in the input that cannot be converted are preserved in the output
sub to_altnames
{
	my (@inList) = @_;
	my (@outList, $altName, $name);

	foreach $name (@inList) {
		($name) = split(/\./, $name);	# shorten name
		if ($havePSSP) {
			$altName = Sdr::nn2ename(Sdr::sname2nn($name));
		} else {
			$altName = getattrval($altAttr,$name);
		}
		push(@outList, $altName ? $altName : $name);
	}	
	return(@outList);
}

# perform the inverse of to_altnames()
#	@inList (IN)	list of altnernate names
#	RETURN		list of genders names
# NOTE: names in the input that cannot be converted are preserved in the output
sub to_gendnames
{
	my (@inList) = @_;
	my (@outList, $altName, $name, $tmp);

	foreach $altName (@inList) {
		($altName) = split(/\./, $altName);	# shorten name
		if ($havePSSP) {
			$name = Sdr::nn2sname(Sdr::ename2nn($altName));
		} else {
			foreach $tmp (getnode($altAttr)) {
				if (getattrval($altAttr, $tmp) eq $altName) {
					$name = $tmp;
					last;
				}
			}
		}
		push(@outList, $name ? $name : $altName);
	}	
	return(@outList);
}

}	# $included
1;	# return a true value...
