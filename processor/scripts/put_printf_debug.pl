#!/usr/bin/perl -w
#
# autor:Manuel Hevia
# description: 
#
#-------------------------------------------
use strict;

sub p1
{
  open(f_in,"< parser_descent_rules.cpp")||die("error open < file");
  my @raw_data=<f_in>;
  open(f_out,"> parser_descent_rules.cpp_debug")||die("error open > file");

  my $method_detected = 0;
  my $method_id = "";
  foreach my $l (@raw_data)
  {
        chomp($l);
        $_= $l;

		#int c_parser_descent::translation_unit(void)
		#{
		if( $_ =~ /int c_parser_descent::([_a-zA-Z]+)([_a-zA-Z\(\) ]+)/ )
		{
			$method_detected = 1;
			$method_id = $1;
		}

		if( 1 == $method_detected)
		{
			if( $_ =~ /{/ )
			{
#				$_ = "{\n  printf(\"## $method_id\\n\");";
                 $_ = "{\n  trace(tab, \"## $method_id\");";
				$method_detected = 0;
			    $method_id = "";
			}
		}

		print f_out "$_\n";
  }

  close(f_in);
  close(f_out);
}

p1;
