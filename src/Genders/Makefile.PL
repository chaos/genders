use ExtUtils::MakeMaker;

WriteMakefile(
    'NAME'		=> 'Genders',
    'VERSION_FROM'	=> 'Genders.pm',
    'PREREQ_PM'		=> {},
    ($] >= 5.005 ?
      (ABSTRACT_FROM => 'Genders.pm',
       AUTHOR     => 'Albert Chu <chu11@llnl.gov>') : ()),
);
