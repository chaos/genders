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

#print "&Libgenders::string_constant tests\n";
#print "---------------------------------------------------------------\n";
#$ret = &Libgenders::string_constant("DEFAULT_GENDERS_FILE");
#print $ret,"\n";
#print "\n\n";


$handle = &Libgenders::genders_handle_create();
if ($handle == 0) {
    print "Error, genders_handle_create()\n";
    exit(1);
}

$ret = &Libgenders::genders_load_data($handle);
if ($ret == -1) {
    print "Error, genders_load_data()\n";
    exit(1);
}

#$ret = &Libgenders::genders_handle_dump($handle);
#if ($ret == -1) {
#    print "Error, genders_handle_dump()\n";
#    exit(1);
#}

#$nodename = &Libgenders::genders_getnodename($handle);
#if ($nodename eq "") {
#    print "Error, genders_getnodename()\n";
#    exit(1);
#}
#print "Nodename is: ",$nodename,"\n\n"; 

#$nodes = &Libgenders::genders_getnodes($handle);
#if (@$nodes == 0 && &Libgenders::genders_errnum($handle) != 0) {
#    print "Error, genders_getnodes()\n";
#    exit(1);
#}
#foreach $_ (@$nodes) {
#    print "node: ",$_,"\n";
#}
#print "\n";

$temp = &Libgenders::genders_getattr($handle);
if (&Libgenders::genders_errnum($handle) != 0) {
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

#$allattr = &Libgenders::genders_getattr_all($handle);
#if (@$allattr == 0 && &Libgenders::genders_errnum($handle) != 0) {
#    print "Error, genders_getattr_all()\n";
#    exit(1);
#}
#foreach $_ (@$allattr) {
#    print "attr: ",$_,"\n";
#}

#$ret = &Libgenders::genders_getattrval($handle, "resmgr", "mdevi");
#print "getattrval resmgr - ",$ret,"\n";
#$ret = &Libgenders::genders_getattrval($handle, "mgmt", "mdevi");
#print "getattrval mgmt - ",$ret,"\n";
#$ret = &Libgenders::genders_getattrval($handle, "foo", "mdevi");
#print "getattrval foo - ",$ret,"\n";

$ret = &Libgenders::genders_testattr($handle, "resmgr");
print "resmgr - ",$ret,"\n";
$ret = &Libgenders::genders_testattr($handle, "mgmt");
print "mgmt - ",$ret,"\n";
$ret = &Libgenders::genders_testattr($handle, "foo");
print "foo - ",$ret,"\n";

#$ret = &Libgenders::genders_testattrval($handle, "resmgr");
#print "resmgr - ",$ret,"\n";
#$ret = &Libgenders::genders_testattrval($handle, "mgmt");
#print "mgmt - ",$ret,"\n";
#$ret = &Libgenders::genders_testattrval($handle, "foo");
#print "foo - ",$ret,"\n";
#$ret = &Libgenders::genders_testattrval($handle, "resmgr", "rms");
#print "resmgr=rms - ",$ret,"\n";
#$ret = &Libgenders::genders_testattrval($handle, "resmgr", "bar");
#print "resmgr=bar - ",$ret,"\n";
#$ret = &Libgenders::genders_testattrval($handle, "mgmt", "rms");
#print "mgmt=rms - ",$ret,"\n";
#$ret = &Libgenders::genders_testattrval($handle, "mgmt", "bar");
#print "mgmt=bar - ",$ret,"\n";
#$ret = &Libgenders::genders_testattrval($handle, "foo", "rms");
#print "foo=rms - ",$ret,"\n";
#$ret = &Libgenders::genders_testattrval($handle, "foo", "bar");
#print "foo=bar - ",$ret,"\n";
#print "\n";

#$ret = &Libgenders::genders_testnode($handle);
#print "local - ",$ret,"\n";
#$ret = &Libgenders::genders_testnode($handle, "mdevi");
#print "mdevi - ",$ret,"\n";
#$ret = &Libgenders::genders_testnode($handle, "foo");
#print "foo - ",$ret,"\n";

#$ret = &Libgenders::genders_parse($handle);
#print "errors - ", $ret, "\n"; 

$ret = &Libgenders::genders_handle_destroy($handle);
if ($ret == -1) {
    print "Error, genders_handle_destroy()\n";
    exit(1);
}
