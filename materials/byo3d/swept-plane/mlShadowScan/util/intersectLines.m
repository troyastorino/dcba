function p = intersectLines(w1,w2)

% INTERSECTLINES Intersect two lines in parameteric form.
%    P = INTERSECTLINES(W1,W2) intersects two lines in 2D, with
%    coefficient vectors W1 and W2, respectively.
%
% Douglas Lanman and Gabriel Taubin 
% Brown University
% 18 May 2009

% Determine point of interesction of two co-planar lines.
w1 = w1(:)';
w2 = w2(:)';
p  = inv([w1(1:2); w2(1:2)])*[w1(3); w2(3)];