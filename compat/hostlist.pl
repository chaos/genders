#############################################################################
#  $Id: hostlist.pl,v 1.9 2010-02-02 00:04:34 chu11 Exp $
#############################################################################
#  Copyright (C) 2007-2019 Lawrence Livermore National Security, LLC.
#  Copyright (C) 2001-2007 The Regents of the University of California.
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
#  with Genders.  If not, see <http://www.gnu.org/licenses/>.
#############################################################################

#
# Routines for reading, normalizing, and displaying lists of hosts.
#
package Hostlist;
 
use strict;
use Carp;

use Genders;

if (!$Hostlist::included) {
$Hostlist::included = 1;

# compress will generate a quadrics-style range if this is > 0
$Hostlist::quadrics_ranges = 0;

# Construct node list from hostlist file
#   $fileName (IN)	hostlist filename
#   RETURN		list of nodes
sub mk_file
{
	my ($fileName) = @_;
	my (@targetNodes);

	if (open(FILE, "< $fileName")) {
		while (<FILE>) {
			chomp;
			s/\*.*$//;              # strip comments (*)
			s/\!.*$//;              # strip comments (!)
			s/^\s+.*//;		# strip leading spaces
			s/.*\s+$//;		# strip trailing spaces
			next if (/^\s*$/);      # skip blank lines
			push(@targetNodes, $_);
		}
		close(FILE);
	}
	return @targetNodes;
}

# Construct node list from genders attribute name
#   $attrName (IN)	attribute name
#   RETURN		list of nodes
sub mk_gend
{
	my ($attrName) = @_;
        my $obj;

        $obj = Genders->new();
	return $obj->getnodes($attrName);
}

# Construct node list from command line
#   $cmdLine (IN)	comma-separated list of nodes 
#   RETURN		list of nodes
sub mk_cmdline
{
	my ($cmdLine) = @_;
	my (@targetNodes);

	@targetNodes = split(/,/, $cmdLine);
	return @targetNodes;
}

# Convert list of hostnames from reliable_hostname to initial_hostname.  
# OK if already initial_hostname.
#   @inList (IN)	list of reliable_hostnames
#   RETURN      	list of initial_hostnames
sub to_initial
{
	my (@inList) = @_;
	my (@outList, $node, $iname);

	foreach $node (@inList) {
		($node) = split(/\./, $node); 			# shorten name
		#$iname = Sdr::nn2sname(Sdr::ename2nn($node));	# convert
		push(@outList, $iname ? $iname : $node);
	}
	
	return @outList;
}

# Convert list of hostnames from initial_hostname to reliable_hostname.  
# OK if already reliable_hostname.
#   @inList (IN)	list of initial_hostnames
#   RETURN 		list of reliable_hostnames
sub to_reliable
{
	my (@inList) = @_;
	my (@outList, $node, $rname);

	foreach $node (@inList) {
		($node) = split(/\./, $node); 			# shorten name
		#$rname = Sdr::nn2ename(Sdr::sname2nn($node));	# convert
		push(@outList, $rname ? $rname : $node);
	}
	
	return @outList;
}

# Detect shell metacharacters in a hostlist entry.
#   $line (IN)		hostlist line
#   RETURN		true if metachars found, false otherwise
sub detect_metachar
{
	my ($line) = @_;

	return ($line =~ /(\;|\||\&)/);
}

# expand()
# turn a hostname range into a list of hostnames. Try to autodetect whether
# a quadrics-style range or a normal hostname range was passed in.
#
sub expand
{
	my ($list) = @_;

        # matching "[" "]" pair with stuff inside will be considered a quadrics
        # range:
        if ($list =~ /[^[]*\[.+\]/) {
		# quadrics ranges are separated by whitespace in RMS -
		# try to support that here
		$list =~ s/\s+/,/g;

		# 
		# Replace ',' chars internal to "[]" with ':"
		#
		while ($list =~ s/(\[[^\]]*),([^\[]*\])/$1:$2/) {}

		return map { expand_quadrics_range($_) } split /,/, $list;

	} else {
		return map { 
                            s/(\w*?)(\d+)-(\w*?)(\d+)/"$2".."$4"/ 
			                       || 
				          s/(.+)/""/; 
                            map {"$1$_"} eval; 
                           } split /,/, $list;
	}
}
			

# expand_quadrics_range
#
# expand nodelist in quadrics form 
#
sub expand_quadrics_range 
{
        my ($list) = @_;
        my ($pfx, $ranges) = split(/[\[\]]/, $list);

        return $list if (!defined $ranges);

        return map {"$pfx$_"} 
	           map { s/(\d+)-(\d+)/"$1".."$2"/; eval } 
		       split(/,|:/, $ranges);
}

# compress_to_quadrics
#
# compress a list of nodes to into a quadrics-style list of ranges
#
sub compress_to_quadrics 
{
	my (@list) = @_;
	local $Hostlist::quadrics_ranges = 1;
        return compress(@list) if @list;
}

# Turn long lists of nodes with numeric suffixes into ranges where possible
# optionally return a Quadrics-style range if $quadrics_ranges is nonzero.
#
#   @nodes (IN)         flat list of nodes
#   RETURN              list of nodes possibly containing ranges
#                       
sub compress {
    	my %rng = comp2(@_);
	my @list = ();

	local $"=",";

    	if (!$Hostlist::quadrics_ranges) {
       		foreach my $k (keys %rng) {
            		@{$rng{$k}} = map { "$k$_" } @{$rng{$k}};
		}
		@list = map { @{$rng{$_}} } sort keys %rng;

    	} else {
		@list = map {  $_ . 
	                      (@{$rng{$_}}>1 || ${$rng{$_}}[0] =~ /-/ ? 
		                       "[@{$rng{$_}}]" :
		                        "@{$rng{$_}}"                     
		              ) 
 		            } sort keys %rng;
    	}

	return wantarray ? @list : "@list"; 
}


# comp2():
# 
# takes a list of names and returns a hash of arrays, indexed by name prefix,
# each containing a list of numerical ranges describing the initial list.
#
# e.g.: %hash = comp2(lx01,lx02,lx03,lx05,dev0,dev1,dev21) 
#       will return:
#       $hash{"lx"}  = ["01-03", "05"]
#       $hash{"dev"} = ["0-1", "21"]
#
sub comp2 
{
    	my (%i) = ();
	my (%s) = ();

    	# turn off warnings here to avoid perl complaints about 
    	# uninitialized values for members of %i and %s
    	local ($^W) = 0;
        push(@{
               $s{$$_[0]}[ (
                            $s{ $$_[0] }[ $i{$$_[0]} ]
                              [$#{$s{$$_[0]}[$i{$$_[0]}]}] == ($$_[1]-1)
                           ) ? $i{$$_[0]} : ++$i{$$_[0]}
                         ]
              }, ($$_[1])
        ) for map { [/(.*?)(\d*)$/] } sortn(@_);

    	for my $key (keys %s) {
        	@{$s{$key}} = 
		    map { $#$_>0 ? "$$_[0]-$$_[$#$_]" : @{$_} }  @{$s{$key}};
    	} 
	
	
	return %s;
}

# uniq: remove duplicates from a hostlist
#
sub uniq
{
	my %seen   = ();
	grep { !$seen{$_}++ } @_;
}

# intersect(\@a, \@b): return the intersection of two lists, 
#                      i.e. those hosts in both @a and @b.
# IN : two array refs \@a , \@b
# OUT: flat list of hosts in =both= @a and @b
sub intersect
{
	my ($a, $b) = @_;
	(ref $a && ref $b) or 
	    croak "Error: arguments to intersect must be references";
	my @result = ();

	for my $hn (@$a) {
		push (@result, grep { $_ eq $hn } @$b);
	}

	return @result;
}

# union(\@a, \@b): return the union of two lists
#                  i.e. list of hosts from both @a and @b
# IN : two array refs \@a, \@b
# OUT: flat list of hosts from @a and @b
sub union
{
	my ($a, $b) = @_;
	(ref $a && ref $b) or 
	    croak "Error: arguments to union must be references";
	return uniq(@$a, @$b);
}

# diff(\@a, \@b): return the list of hosts in @a that are not in @b 
#		  i.e. hosts in @a - hosts in @b
# IN : two array refs \@a, \@b
# OUT: flat list of hosts in @a that are not in @b
sub diff
{
	my ($a, $b) = @_;
	(ref $a && ref $b) or 
	    croak "Error: arguments to diff must be references";
	my @result = ();

	for my $hn (@$a) {
		push(@result, $hn) if (!grep { $_ eq $hn } @$b); 
	}

	return @result;
}

# xor(\@a, \@b): exclusive OR hosts in @a and @b
#		 i.e. those hosts in @a and @b but not in both
# IN : two array refs \@a, \@b
# OUT: flat list of hosts in @a and @b but not in both
sub xor
{
	my ($a, $b) = @_;
	(ref $a && ref $b) or 
	    croak "Error: arguments to xor must be references";
	return (diff($a, $b), diff($b, $a));
}

# within(\@a, \@b): true if all hosts in @a are in @b
#		    i.e. is @a a subset of @b?
# IN : two array refs \@a, \@b
# OUT: true if @a is a subset of @b, false otherwise.
sub within
{
	my ($a, $b) = @_;
	(ref $a && ref $b) or 
	    croak "Error: arguments to within must be references";
	return (intersect($a, $b) == @$a)
}

# same(\@a, \@b) : true if @a and @b contain the exact same hosts
#		   i.e. are @a and @b the same list?
# IN : two array refs \@a, \@b
# OUT: true if @a is same as @b, false otherwise
sub same
{
	my ($a, $b) = @_;
	(ref $a && ref $b) or 
	    croak "arguments to same must be references";
	return (within($a, $b) && within($b, $a));
}

# sortn:
#
# sort a group of alphanumeric strings by the last group of digits on
# those strings, if such exists (good for numerically suffixed host lists)
#
sub sortn
{
	map {$$_[0]} sort {($$a[1]||0)<=>($$b[1]||0)} map {[$_,/(\d*)$/]} @_;
}


}	# Hostlist::included
1; 	# return a true value...
