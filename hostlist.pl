#
# $Id: hostlist.pl,v 1.4 2001-07-14 05:36:52 garlick Exp $
# $Source: /g/g0/achu/temp/genders-cvsbackup-full/genders/hostlist.pl,v $
#
# Copyright (C) 2000-2001 Regents of the University of California
# See the DISCLAIMER file distributed with this package.
# 
# Author: Jim Garlick
#
# Adapted from IBM SP version for linux 4/00.
# Routines for reading, normalizing, and displaying lists of hosts.
#

package Hostlist;
 
use strict;

if (!$Hostlist::included) {
$Hostlist::included = 1;

require "/usr/lib/genders/gendlib.pl";

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

	return Genders::getnode($attrName);
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

# Turn long lists of nodes with numeric suffixes into ranges where possible
#   @nodes (IN)   	flat list of nodes
#   RETURN        	list of nodes possibly containing ranges
sub compress
{
        my @nodes = @_;
        my @out = ();
        my (%nn, $node, $pfx, @stack, @n, $i);

        # build %nn (lists of numeric hostname suffixes by prefix)
        foreach $node (@nodes) {
                if ($node =~ /(\D+)(\d+)/) {
                        push(@{$nn{$1}}, $2);
                } else {
                        push(@out, $node);
                }       
        }       

        foreach $pfx (keys %nn) {
                @n = sort {$a <=> $b} @{$nn{$pfx}};
                for ($i = 0; $i <= $#n; $i++) {
                        if ($i == 0) {
                                push(@stack, $n[$i]);
                        } else {
                                if ($stack[$#stack] == $n[$i] - 1) {
                                        push(@stack, $n[$i]);
                                } else {
                                        push(@out, $pfx . get_range(@stack));
                                        @stack = ($n[$i]);
                                }
                        }
                }
                if (@stack) {
                        push(@out, $pfx . get_range(@stack));
                }
        }
        return @out;
}

# determine if a range is on the stack (helper for compress)
#   @stack (IN)		stack
#   RETURN		range or single value
sub get_range
{
        my @stack = @_;

        if ($#stack == 0) {  # one element
                return ($stack[0]);
        } else {
                return ($stack[0] . "-" . $stack[$#stack]);
        }
}

}	# Hostlist::included
1; 	# return a true value...
