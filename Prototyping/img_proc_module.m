clear all; close all; clc;
% Open Depth image
orig = imread('depth.png');
% Convert to 0-255
img = double(orig)./145;

% Open RGB image
rgb = imread('rgb.png');

% Choose random points that should lie on calibration plane
% X = [x y z]
X = double([
     300 40 img(40,300);
     100 350 img(350,100);
     300 450 img(450,300);
     500 400 img(400,500);
     580 250 img(250,580);
     50 450 img(450,50);
     50 200 img(200,50);
    ]);

% Y is all ones
Y = ones(size(X,1),1);
% Normal equation to solve for plane normal vector
B = pinv(X'*X)*(X'*Y);
% Grid for all pixels to generate plane
[Xg,Yg] = meshgrid(1:640,1:480);
% Genereate plane based off of equation
%  B1*x + B2*y + B3*z = 1
B_plane = double((-B(1)*Xg - B(2)*Yg + 1))./B(3);

% Visualize RGB Image
subplot(2,2,1);
image(rgb);
title('RGB Image');
% Visualize generated plane
subplot(2,2,2);
imagesc(img);
title('Depth Image with Sample Points');
% Draw sample points
for i=1:size(X,1)
    rectangle('Position',[X(i,1)-10,X(i,2)-10,20,20],'EdgeColor',[0 0 0],'LineWidth',2);
    drawnow
end

% Visualize Depth image
subplot(2,2,3);
imagesc(B_plane);
title('Estimated Surface Plane');
% Find depth pixels not within threshold of plane
threshold = 20;
% Generate plane mask
img_plane = (abs(img-B_plane) > threshold & img ~= 0);
% Overlay original depth pixels onto mask
masked_img = img_plane.*img;
% Visualize masked version of detph iamge
subplot(2,2,4);
title('Masked Depth Image');

% Scale image?
num = 1;
masked_img = round(num.*masked_img./max(max(masked_img)));
imagesc(masked_img);

% Find first pixels for outside of box
[~,top] = find(masked_img'~=0,1);
[~,bot] = find(masked_img'~=0,1,'last');
[~,left] = find(masked_img~=0,1);
[~,right] = find(masked_img~=0,1,'last');

% Draw bounding box
rectangle('Position',[left,top,right-left,bot-top],'EdgeColor','g','LineWidth',2);

distance = mean(mean(img(masked_img > 0)))
x = mean([left right])
y = mean([top bot])
width = right-left
height = bot-top