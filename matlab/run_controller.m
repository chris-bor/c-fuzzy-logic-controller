% E:\dyplom\matlab\
% Krzysztof I. Borowiec

close all;
clear all;
format short; 
domain_scope = round( ( 2^12-1 ) / 6 );
fuzzy_sets(1) = struct(...
    'id', 1, ...
    'domain', [0,100], ...
    'mf', zeros(1,256) );
create_fuzzy_sets();