use ExtUtils::MakeMaker;
use Config;

# On AIX, set the compiled-in library path for dynamic libraries to the path
# where the C library is installed plus the default library path.  Otherwise,
# the library path contains an entry for the genders library in the source
# directory.
my @new_argv;
foreach (@ARGV) {
    if (my ($libdir) = /^GENDERS_LIBDIR=(.*)$/) {
        if ($Config{osname} eq "aix") {
            next if ($libdir eq "");
            my $lddlflags = $Config{lddlflags};
            if ($lddlflags !~ s/(-blibpath:)/$1$libdir:/g) {
                $lddlflags .= " -blibpath:$libdir:/usr/lib:/lib";
            }
            push @new_argv, "LDDLFLAGS=$lddlflags";
        }
    }
    else {
        push @new_argv, $_;
    }
}
@ARGV = @new_argv;

WriteMakefile(
    'NAME'		=> 'Libgenders',
    'VERSION_FROM'	=> 'Libgenders.pm',
    'PREREQ_PM'		=> {},
    ($] >= 5.005 ?
      (ABSTRACT_FROM => 'Libgenders.pm',
       AUTHOR     => 'Albert Chu <chu11@llnl.gov>') : ()),
);
