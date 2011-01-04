from distutils.core import setup, Extension

libgendersmodule = Extension('libgenders',
                             include_dirs = ['../../../../config', '../../../libgenders'],
                             library_dirs = ['../../../libgenders/.libs'],
                             libraries = ['genders'],
                             sources = ['libgendersmodule.c'])

setup (name = 'libgenders',
       version = '1.0',
       description = 'This is the libgenders package',
       ext_modules = [libgendersmodule])
