function writeVRML(filename,vertices,colors,normals)

% WRITEVRML exports a VRML point clound.
%    WRITEVRML(F,V,C,N) exports the 3D point cloud to a VRML file
%    for use with j3DPGP. Also handles per-vertex colors and normals
%    if they are provided.
%
% Douglas Lanman and Gabriel Taubin 
% Brown University
% 18 May 2009


% Write VRML file.
fid = fopen(filename,'w');
fprintf(fid,'%s\r\n','#VRML V2.0 utf8');
fprintf(fid,'%s\r\n','Shape {');
fprintf(fid,'%s\r\n',' geometry IndexedFaceSet {');
fprintf(fid,'%s\r\n','  coord Coordinate {');
fprintf(fid,'%s\r\n','   point [');
nPoints = size(vertices,1);
for i = 1:nPoints
   fprintf(fid,'   %0.4f %0.4f %0.4f\r\n',vertices(i,:));
end
fprintf(fid,'%s\r\n','   ]');
fprintf(fid,'%s\r\n','  }');
if exist('normals','var') && ~isempty(normals)
   fprintf(fid,'%s\r\n','  normalPerVertex TRUE');
   fprintf(fid,'%s\r\n','  normal Normal {');
   fprintf(fid,'%s\r\n','   vector [');
   for i = 1:nPoints
      fprintf(fid,'   %0.4f %0.4f %0.4f\r\n',-normals(i,:));
   end
   fprintf(fid,'%s\r\n','   ]');
   fprintf(fid,'%s\r\n','  }');
end
if exist('colors','var') && ~isempty(colors)
   fprintf(fid,'%s\r\n','  colorPerVertex TRUE');
   fprintf(fid,'%s\r\n','  color Color {');
   fprintf(fid,'%s\r\n','   color [');
   for i = 1:nPoints
      fprintf(fid,'   %0.4f %0.4f %0.4f\r\n',colors(i,:));
   end
   fprintf(fid,'%s\r\n','   ]');
   fprintf(fid,'%s\r\n','  }');
end
fprintf(fid,'%s\r\n',' }');
fprintf(fid,'%s\r\n','}');
fclose(fid);