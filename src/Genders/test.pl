#!/usr/bin/perl -w

use Genders;

$obj = Genders->new();

$nodename = $obj->getnodename();
print "Nodename is: ",$nodename,"\n\n"; 

$nodes = $obj->getnodes();
foreach $_ (@$nodes) {
    print "node: ",$_,"\n";
}
print "\n";

@attrs = $obj->getattr("mdevi");

print "Attributes are:\n";
foreach $_ (@attrs) {
    print "attr: ",$_,"\n";
}
print "\n";

print "All Attrs are\n";
@attrs = $obj->getattr_all();
foreach $_ (@attrs) {
    print "attr: ",$_,"\n";
}
print "\n";

$ret = $obj->getattrval("resmgr", "mdevi");
print "getattrval resmgr - ",$ret,"\n";
$ret = $obj->getattrval("mgmt", "mdevi");
print "getattrval mgmt - ",$ret,"\n";
$ret = $obj->getattrval("foo", "mdevi");
print "getattrval foo - ",$ret,"\n";

$ret = $obj->testattr("resmgr");
print "resmgr - ",$ret,"\n";
$ret = $obj->testattr("compute");
print "mgmt - ",$ret,"\n";
$ret = $obj->testattr("foo");
print "foo - ",$ret,"\n";

$ret = $obj->testattrval("resmgr", "rms");
print "resmgr=rms - ",$ret,"\n";
$ret = $obj->testattrval("resmgr", "bar");
print "resmgr=bar - ",$ret,"\n";
$ret = $obj->testattrval("compute", "rms");
print "mgmt=rms - ",$ret,"\n";
$ret = $obj->testattrval("mgmt", "bar");
print "mgmt=bar - ",$ret,"\n";
$ret = $obj->testattrval("foo", "rms");
print "foo=rms - ",$ret,"\n";
$ret = $obj->testattrval("altname", "emdev21");
print "foo=bar - ",$ret,"\n";
print "\n";

$ret = $obj->isnode();
print "local - ",$ret,"\n";
$ret = $obj->isnode("mdevi");
print "mdevi - ",$ret,"\n";
$ret = $obj->isnode("foo");
print "foo - ",$ret,"\n";

