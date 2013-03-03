% 
% %%%%%%%%%%%%%%%%%%%% SHOW EXTRINSIC RESULTS %%%%%%%%%%%%%%%%%%%%%%%%
% 
if ~exist('n_ima')|~exist('fc_proj')|~exist('fc_cam'),
   fprintf(1,'No system calibration data available.\n');
   return;
end;

if ~exist('show_camera'),
    show_camera = 1;
end;

check_active_images;

if ~exist(['omc_' num2str(ind_active(1))]),
    fprintf(1,'No calibration data available.\n');
    return;
end;

%if ~exist('no_grid'),
no_grid = 0;
%end;

% generate the grids
for i=ind_active
    % seting the number of squares for each pattern. Both X and Y
    eval(['n_sq_x_' num2str(i) '= ' num2str(n_sq_x) '; n_sq_y_' num2str(i) '=' num2str(n_sq_y) ';']);
end

if ~exist(['n_sq_x_' num2str(ind_active(1))]),
    no_grid = 1;
end;

if ~exist('alpha_c'),
    alpha_c = 0;
end;


% Color code for each image:

colors = 'brgkcm';


%%% Show the extrinsic parameters

if ~exist('dX'),
    eval(['dX = norm(Tc_' num2str(ind_active(1)) ')/10;']);
    dY = dX;
end;

IP = 2*dX*[1 -alpha_c 0;0 1 0;0 0 1]*[1/fc(1) 0 0;0 1/fc(2) 0;0 0 1]*[1 0 -cc(1);0 1 -cc(2);0 0 1]*[0 nx-1 nx-1 0 0 ; 0 0 ny-1 ny-1 0;1 1 1 1 1];
BASE = 2*(.9)*dX*([0 1 0 0 0 0;0 0 0 1 0 0;0 0 0 0 0 1]);
IP = reshape([IP;BASE(:,1)*ones(1,5);IP],3,15);
POS = [[6*dX;0;0] [0;6*dX;0] [-dX;0;5*dX] [-dX;-dX;-dX] [0;0;-dX]];

IP_cam = 2*dX*[1 -alpha_c_cam 0;0 1 0;0 0 1]*[1/fc_cam(1) 0 0;0 1/fc_cam(2) 0;0 0 1]*[1 0 -cc_cam(1);0 1 -cc_cam(2);0 0 1]*[0 nx_cam-1 nx_cam-1 0 0 ; 0 0 ny_cam-1 ny_cam-1 0;1 1 1 1 1];
BASE_cam = 2*(.9)*dX*([0 1 0 0 0 0;0 0 0 1 0 0;0 0 0 0 0 1]);
IP_cam = reshape([IP_cam;BASE_cam(:,1)*ones(1,5);IP_cam],3,15);
POS_cam = [[6*dX;0;0] [0;6*dX;0] [-dX;0;5*dX] [-dX;-dX;-dX] [0;0;-dX]];

if ishandle(4),
    figure(4);
    [a,b] = view;
else
    figure(4);
    a = 50;
    b = 20;
end;


figure(4);
clf;
hold on;

% Compute Projector Coordinate frame (the same for all)
eval(['omc_kk = omc_' num2str(ind_active(1)) ';']);
eval(['Tc_kk = Tc_' num2str(ind_active(1)) ';']);
R_kk = rodrigues(omc_kk);

BASEk = R_kk'*(BASE - Tc_kk * ones(1,6));
IPk = R_kk'*(IP - Tc_kk * ones(1,15));
POSk = R_kk'*(POS - Tc_kk * ones(1,5));

% Plot the projector
plot3(BASEk(1,:),BASEk(2,:),BASEk(3,:),'b-','linewidth',2');
plot3(IPk(1,:),IPk(2,:),IPk(3,:),'g-','linewidth',2);
text(POSk(1,5),POSk(2,5),POSk(3,5),'Projector','fontsize',10,'color','k','FontWeight','bold');
  
% Plot the camera
plot3(BASE_cam(1,:),BASE_cam(2,:),BASE_cam(3,:),'b-','linewidth',2');
plot3(IP_cam(1,:),IP_cam(2,:),IP_cam(3,:),'r-','linewidth',2);
text(POS_cam(1,5),POS_cam(2,5),POS_cam(3,5),'Camera','fontsize',10,'color','k','FontWeight','bold');


% Plot the planes
for kk = find(active_images)

    if exist(['X_' num2str(kk)]) & exist(['omc_' num2str(kk)]),
    % if the image has been used to calibrate
    
        eval(['XX_kk = X_' num2str(kk) ';']);
           if ~isnan(XX_kk(1,1))
                
                eval(['omc_kk = omc_' num2str(kk) ';']);
                eval(['Tc_kk = Tc_' num2str(kk) ';']);
                N_kk = size(XX_kk,2);
                
                if ~exist(['n_sq_x_' num2str(kk)]),
                    no_grid = 1;
                else
                    eval(['n_sq_x = n_sq_x_' num2str(kk) ';']);
                    if isnan(n_sq_x(1)),
                        no_grid = 1;
                    end;  
                end;
                
                
                if ~no_grid,
                    eval(['n_sq_x = n_sq_x_' num2str(kk) ';']);
                    eval(['n_sq_y = n_sq_y_' num2str(kk) ';']);
                    if (N_kk ~= ((n_sq_x+1)*(n_sq_y+1))),
                        no_grid = 1;
                    end;
                end;
                
                if ~isnan(omc_kk(1,1)),
                    

                    
                    YY_kk = XX_kk;
                    
                    if ~no_grid,
                        
                        YYx = zeros(n_sq_x+1,n_sq_y+1);
                        YYy = zeros(n_sq_x+1,n_sq_y+1);
                        YYz = zeros(n_sq_x+1,n_sq_y+1);
                        
                        YYx(:) = YY_kk(1,:);
                        YYy(:) = YY_kk(2,:);
                        YYz(:) = YY_kk(3,:);
                        
                        %plot the mesh and label it
                        hhh= mesh(YYx,YYy,YYz);
                        set(hhh,'edgecolor',colors(rem(kk-1,6)+1),'linewidth',1); %,'facecolor','none');
%                        text(POSk(1,5),POSk(2,5),POSk(3,5),num2str(kk),'fontsize',10,'color','k','FontWeight','bold');
                        text(YYx(end-n_sq_x)-dX/2,YYy(end-n_sq_x)-dY/2,YYz(end-n_sq_x),num2str(kk),'fontsize',14,'color',colors(rem(kk-1,6)+1),'HorizontalAlignment','center');
                    end
                end
           end
    end
end


% Draw the camera at the origin
normT = 2*dX;
text(dX,0,0,'X_c');
text(0,dX,0,'Y_c');
text(0,0,dX,'Z_c');
text(dX/4,dX/4,0,'O_c');



grid on

figure(4);rotate3d on;
axis('equal');
title('Extrinsic parameters (camera-centered)');

xlabel('X_{world}')
ylabel('Y_{world}')
zlabel('Z_{world}')
view(a,b);

axis vis3d;
axis tight;
hold off;
set(4,'color',[1 1 1]);

set(4,'Name','3D','NumberTitle','off');
