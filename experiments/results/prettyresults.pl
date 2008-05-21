@ARGV = qw(.) unless @ARGV;
use File::Find;
find sub { s/searchterm/replaceterm/ $File::Find::name, -d && '/', "\n" }, @ARGV;
