# Before `make install' is performed this script should be runnable with
# `make test'. After `make install' it should work as `perl test.pl'

#########################

# change 'tests => 1' to 'tests => last_test_to_print';

use Test;
BEGIN { plan tests => 1 };
use Libgenders;

ok(1); # If we made it this far, we're ok.

#########################

# Insert your test code below, the Test module is use()ed here so read
# its man page ( perldoc Test ) for help writing this test script.

$handle = Libgenders->genders_handle_create();
if (!defined($handle)) {
    print "Error, genders_handle_create()\n";
    exit(1);
}

$ret = $handle->genders_load_data();
if ($ret == -1) {
    print "Error, genders_load_data()\n";
    print $handle->genders_errormsg();
    exit(1);
}

#$ret = $handle->genders_handle_dump();
#if ($ret == -1) {
#    print "Error, genders_handle_dump()\n";
#    exit(1);
#}

$nodename = $handle->genders_getnodename();
if (!defined($nodename)) {
    print "Error, genders_getnodename()\n";
    print $handle->genders_errormsg();
    exit(1);
}
print "Nodename is: ",$nodename,"\n\n"; 

$nodes = $handle->genders_getnodes();
if (!defined($nodes)) {
    print "Error, genders_getnodes()\n";
    exit(1);
}
foreach $_ (@$nodes) {
    print "node: ",$_,"\n";
}
print "\n";

$temp = $handle->genders_getattr();
if (!defined($temp)) {
    print "Error, genders_getattr()\n";
    exit(1);
}
($attrs, $vals) = @$temp;

print "Attributes and Values are:\n";
$i = 0;
$len = @$attrs;
while ($i < $len) {
    print @$attrs[$i];
    if (@$vals[$i] ne "") {
        print "=",@$vals[$i];
    }
    print "\n";
    $i++;
} 
print "\n";

$allattr = $handle->genders_getattr_all();
if (!defined($allattr)) {
    print "Error, genders_getattr_all()\n";
    exit(1);
}
foreach $_ (@$allattr) {
    print "attr: ",$_,"\n";
}

$ret = $handle->genders_getattrval("resmgr", "mdevi");
print "getattrval resmgr - ",$ret,"\n";
$ret = $handle->genders_getattrval("mgmt", "mdevi");
print "getattrval mgmt - ",$ret,"\n";
$ret = $handle->genders_getattrval("foo", "mdevi");
print "getattrval foo - ",$ret,"\n";

$ret = $handle->genders_testattr("resmgr");
print "resmgr - ",$ret,"\n";
$ret = $handle->genders_testattr("mgmt");
print "mgmt - ",$ret,"\n";
$ret = $handle->genders_testattr("foo");
print "foo - ",$ret,"\n";
$ret = $handle->genders_testattr("resmgr", undef);
print "resmgr2 - ",$ret,"\n";
$ret = $handle->genders_testattr("mgmt", undef);
print "mgmt2 - ",$ret,"\n";
$ret = $handle->genders_testattr("foo", undef);
print "foo2 - ",$ret,"\n";
$ret = $handle->genders_testattrval("resmgr", "both");
print "resmgr=both - ",$ret,"\n";
$ret = $handle->genders_testattrval("resmgr", "bar");
print "resmgr=bar - ",$ret,"\n";
$ret = $handle->genders_testattrval("mgmt", "both");
print "mgmt=both - ",$ret,"\n";
$ret = $handle->genders_testattrval("mgmt", "bar");
print "mgmt=bar - ",$ret,"\n";
$ret = $handle->genders_testattrval("foo", "both");
print "foo=both - ",$ret,"\n";
$ret = $handle->genders_testattrval("foo", "bar");
print "foo=bar - ",$ret,"\n";
print "\n";

$ret = $handle->genders_isnode();
print "local - ",$ret,"\n";
$ret = $handle->genders_isnode(undef);
print "local2 - ",$ret,"\n";
$ret = $handle->genders_isnode("mdevi");
print "mdevi - ",$ret,"\n";
$ret = $handle->genders_isnode("foo");
print "foo - ",$ret,"\n";

$ret = $handle->genders_isattr("qla");
print "qla - ",$ret,"\n";
$ret = $handle->genders_isattr("foo");
print "foo - ",$ret,"\n";

$ret = $handle->genders_isattrval("resmgr", "both");
print "resmgr=both - ",$ret,"\n";
$ret = $handle->genders_isattrval("resmgr", "foo");
print "resmgr=foo - ",$ret,"\n";
$ret = $handle->genders_isattrval("foo", "bar");
print "foo=bar - ",$ret,"\n";

$ret = $handle->genders_parse();
print "errors - ", $ret, "\n"; 
