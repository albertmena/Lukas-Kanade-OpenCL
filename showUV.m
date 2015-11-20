nx = 150;
ny = 150;
% nx = 64;
% ny = 64;

% Read U
file = 'u.raw';
fid = fopen(file);
[val, count] = fread (fid, nx*ny, 'float');
im2D = reshape(val,ny,nx);
im2D = im2D';
fclose(fid);
U = im2D;

% Read V
file = 'v.raw';
fid = fopen(file);
[val, count] = fread (fid, nx*ny, 'float');
im2D = reshape(val,ny,nx);
im2D = im2D';
fclose(fid);
V = im2D;

for i=1:10:ny-10,
	for j=1:10:nx-10,
		ux_red((i+9)/10,(j+9)/10) = sum(sum(U(i:i+10,j:j+10)))/100;
		uy_red((i+9)/10,(j+9)/10) = sum(sum(V(i:i+10,j:j+10)))/100;
	end
end
[X, Y] = meshgrid(1:10:ny-10,-1:-10:-nx+10);
figure(7);
quiver(X, Y, ux_red, uy_red);
title('lk-openCL');

