A01 = imread('sweetsA01.png');
A02 = imread('sweetsA02.png');
A03 = imread('sweetsA03.png');

%colors{1} = {[255 255 0], [50 255;80 255;0 50]}; % Yellow
%colors{2} = {[0 255 0], [0 50;50 255;0 50]}; % Green
%colors{3} = {[0 0 255], [100 200;140 255;100 200]}; % Blue
%colors{4} = {[255 0 0], [50 255;0 50;0 50]}; % Red

colors{1} = {[255 255 0], [50 255;80 255;0 50]}; % Yellow
colors{2} = {[0 255 0], [0 50;0 255;0 50]}; % Green
colors{3} = {[0 0 255], [70 110;90 130; 115 155]}; % Blue
colors{4} = {[255 0 0], [70 160;0 30;0 30]}; % Red
colors{5} = {[255 128 128], [130 200;30 90; 65 125]}; % Pink
colors{6} = {[255 128 0], [155 200;40 90; 0 55]}; % Orange

SE = strel('disk', 4);

PopTheNonStops(A03, 'test.txt', colors, SE, 'c');
