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

print "\n";

$handle = Libgenders::genders_handle_create();
if (!defined $handle) {
    print "Error, genders_handle_create()\n";
    exit(1);
}

#$ret = Libgenders::genders_load_data($handle);
#if ($ret == -1) {
#    print "Error, genders_load_data()\n";
#    exit(1);
#}

$ret = Libgenders::genders_load_data($handle, undef);
if ($ret == -1) {
    print "Error, genders_load_data()\n";
    exit(1);
}

#$ret = Libgenders::genders_handle_dump($handle);
#if ($ret == -1) {
#    print "Error, genders_handle_dump()\n";
#    exit(1);
#}

$nodename = Libgenders::genders_getnodename($handle);
if (!defined $nodename) {
    print "Error, genders_getnodename()\n";
    exit(1);
}
print "Nodename is: ",$nodename,"\n\n"; 

$nodes = Libgenders::genders_getnodes($handle);
if (!defined $nodes) {
    print "Error, genders_getnodes()\n";
    exit(1);
}
foreach $_ (@$nodes) {
    print "node: ",$_,"\n";
}
print "\n";

$temp = Libgenders::genders_getattr($handle);
if (!defined $temp) {
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

$allattr = Libgenders::genders_getattr_all($handle);
if (!defined $allattr) {
    print "Error, genders_getattr_all()\n";
    exit(1);
}
foreach $_ (@$allattr) {
    print "attr: ",$_,"\n";
}

$ret = Libgenders::genders_getattrval($handle, "resmgr", "mdevi");
print "getattrval resmgr - ",$ret,"\n";
$ret = Libgenders::genders_getattrval($handle, "mgmt", "mdevi");
print "getattrval mgmt - ",$ret,"\n";
$ret = Libgenders::genders_getattrval($handle, "foo", "mdevi");
print "getattrval foo - ",$ret,"\n";

$ret = Libgenders::genders_testattr($handle, "resmgr");
print "resmgr - ",$ret,"\n";
$ret = Libgenders::genders_testattr($handle, "mgmt");
print "mgmt - ",$ret,"\n";
$ret = Libgenders::genders_testattr($handle, "foo");
print "foo - ",$ret,"\n";
$ret = Libgenders::genders_testattr($handle, "resmgr", undef);
print "resmgr2 - ",$ret,"\n";
$ret = Libgenders::genders_testattr($handle, "mgmt", undef);
print "mgmt2 - ",$ret,"\n";
$ret = Libgenders::genders_testattr($handle, "foo", undef);
print "foo2 - ",$ret,"\n";
$ret = Libgenders::genders_testattrval($handle, "resmgr", "both");
print "resmgr=both - ",$ret,"\n";
$ret = Libgenders::genders_testattrval($handle, "resmgr", "bar");
print "resmgr=bar - ",$ret,"\n";
$ret = Libgenders::genders_testattrval($handle, "mgmt", "both");
print "mgmt=both - ",$ret,"\n";
$ret = Libgenders::genders_testattrval($handle, "mgmt", "bar");
print "mgmt=bar - ",$ret,"\n";
$ret = Libgenders::genders_testattrval($handle, "foo", "both");
print "foo=both - ",$ret,"\n";
$ret = Libgenders::genders_testattrval($handle, "foo", "bar");
print "foo=bar - ",$ret,"\n";
print "\n";

$ret = Libgenders::genders_isnode($handle);
print "local - ",$ret,"\n";
$ret = Libgenders::genders_isnode($handle, undef);
print "local2 - ",$ret,"\n";
$ret = Libgenders::genders_isnode($handle, "mdevi");
print "mdevi - ",$ret,"\n";
$ret = Libgenders::genders_isnode($handle, "foo");
print "foo - ",$ret,"\n";

$ret = Libgenders::genders_isattr($handle, "qla");
print "qla - ",$ret,"\n";
$ret = Libgenders::genders_isattr($handle, "foo");
print "foo - ",$ret,"\n";

$ret = Libgenders::genders_isattrval($handle, "resmgr", "both");
print "resmgr=both - ",$ret,"\n";
$ret = Libgenders::genders_isattrval($handle, "resmgr", "foo");
print "resmgr=foo - ",$ret,"\n";
$ret = Libgenders::genders_isattrval($handle, "foo", "bar");
print "foo=bar - ",$ret,"\n";

$ret = Libgenders::genders_parse($handle);
print "errors - ", $ret, "\n"; 

$ret = Libgenders::genders_handle_destroy($handle);
if ($ret == -1) {
    print "Error, genders_handle_destroy()\n";
    exit(1);
}
