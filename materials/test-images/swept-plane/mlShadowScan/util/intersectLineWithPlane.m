function p = intersectLineWithPlane(q,v,w)

% INTERSECTLINES Find intersection of a line with a plane.
%    P = INTERSECTLINEWITHPLANE(Q,V,W) finds the point of intersection
%    of a line in parametric form (i.e., containing a point Q and spanned
%    by the vector V) with a plane W defined in implicit form. Note,
%    this function does not handle "pathelogical" cases, since they
%    do not occur in practice with the "shadow scanning" configuration.
%
% Douglas Lanman and Gabriel Taubin 
% Brown University
% 18 May 2009

% Intersect a line with a plane (in 3D).
t = (w(4) - w(1:3)'*q)/(w(1:3)'*v);
p = q + t*v;
