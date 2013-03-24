function w = fitLine(X,Y)

% FITLINE least-squared line fitting.
%    W = FITLINE(X,Y) finds the best-fit line L, in the least-squares
%    sense, between the points (X,Y). The resulting line L is described
%    by the coefficient vector W, where W(1)*X + W(2)*Y = W(3), for
%    (X,Y) on the line L.
%
% Douglas Lanman and Gabriel Taubin 
% Brown University
% 18 May 2009

% Construct the linear least-squares matrix U.
meanX = mean(X(:));
meanY = mean(Y(:));
U = [X(:)-meanX,Y(:)-meanY];

% Determine the minimum eigenvector of U'*U.
[V,D] = eig(U'*U);
[minVal,minInd] = min(diag(D));
a = V(1,minInd);
b = V(2,minInd);

% Calculate the corresponding value of d.
d = a*meanX+b*meanY;

% return the parameter vector.
w = [a b d];