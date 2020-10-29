%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% octave_serial_callback.m
%
% If running for the first time, it may be necessary to uncomment 
% the line which installs the instrument-control package which is
% needed to control com ports.
%
% DON'T FORGET TO UPDATE port VARIABLE!!
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% pkg install -forge instrument-control

clc; clear all; close all

pkg load instrument-control

port = "COM5";

s1 = serial( port, 9600 );
srl_flush( s1 );

while 1
  i = input( "Input: ", "s" );
  
  srl_write( s1, i );
  
  data = srl_read( s1, 1 );
  
  char( data )
endwhile
