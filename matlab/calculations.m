% E:\dyplom\matlab\
close all;
clear all;
format long;
nr_figure = 1;
show_sets = 0;
show_resul = 1;

% Oblicza funkcje przynaleznosci dla punktow Domain ale tylko linia
Domain = [100, 200];
Scalar = [ Domain(1) : 4 : Domain(2) ];
LoDomain = Domain(1);
Range = Domain( 2 ) - Domain( 1 );
VECMAX = 256;
TVcell = ( Scalar - LoDomain ) / Range * VECMAX;
UNIV_OF_DISC( 1 ) = 0;
UNIV_OF_DISC( 2 ) = 2^12-1;
% TVcell = ( Scalar - LoDomain ) / Range

term = UNIV_OF_DISC( 2 )/6;
for i = 1 : 7
    terms( i ) = term*( i - 1 );
end

if( show_sets ) figure( nr_figure ); nr_figure=nr_figure+1; 
    plot_sets; 
end
SHIFTED_UNIV_OF_DISC( 1 ) = UNIV_OF_DISC( 1 ) - 2^12/2;
SHIFTED_UNIV_OF_DISC( 2 ) = UNIV_OF_DISC( 2 ) - 2^12/2;

input = -900;
domain0 = -2048;
scope = 1365;
MF_LENG = 32;
SHIFT = 2048;
% posi1 = ( ( -604 - (-1365) )/1365*( 32-1 ) )
% posi2 = ( ( -604+2048 - (-1365+2048) )/1365*( 32-1 ) )
posit1 = ( ( input - domain0 )/scope*( MF_LENG-1 ) );
posit2 = ( ( (input + SHIFT) - (domain0 + SHIFT) )/scope*( MF_LENG-1 ) );
posit1-posit2

RESUL_LENGT = 3*MF_LENG;
resul_count = ( ( domain0 + SHIFT ) / scope * RESUL_LENGT );

if( show_resul ) figure( nr_figure ); nr_figure=nr_figure+1; 
    plot_resul;
end