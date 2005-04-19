use ExtUtils::MakeMaker;

WriteMakefile(
    'NAME'		=> 'Libgenders',
    'VERSION_FROM'	=> 'Libgenders.pm',
    'PREREQ_PM'		=> {},
    ($] >= 5.005 ?
      (ABSTRACT_FROM => 'Libgenders.pm',
       AUTHOR     => 'Albert Chu <chu11@llnl.gov>') : ()),
);
