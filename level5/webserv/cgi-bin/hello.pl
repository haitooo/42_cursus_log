# Driven by /usr/bin/perl. Perl takes the script as its own program argument,
# which is why @ARGV is empty here while $0 is the script name.
print "Content-Type: text/plain\r\n\r\n";
print "hello from perl\n";
print "interpreter=/usr/bin/perl script=$0\n";

my $method = defined $ENV{'REQUEST_METHOD'} ? $ENV{'REQUEST_METHOD'} : '';
my $query  = defined $ENV{'QUERY_STRING'}   ? $ENV{'QUERY_STRING'}   : '';
my $info   = defined $ENV{'PATH_INFO'}      ? $ENV{'PATH_INFO'}      : '';
print "method=$method query=$query path_info=$info\n";

# The server chdir()s into the script's directory, so this relative open works.
my $neighbour = 'MISSING';
if (open(my $fh, '<', 'data.txt')) {
	local $/;
	$neighbour = <$fh>;
	close($fh);
	$neighbour =~ s/\s+\z//;
}
use Cwd ();
print 'cwd=' . Cwd::getcwd() . " neighbour=$neighbour\n";

if ($method eq 'POST') {
	local $/;
	my $body = <STDIN>;
	$body = '' unless defined $body;
	print "body=$body\n";
}
exit 0;
