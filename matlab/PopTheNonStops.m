function [] = PopTheNonStops(I, filename, colors, SE, shape, t)
    file = fopen(filename, 'w+');
    length = size(colors);
    length = length(1);
    
    % Number of colors
    fprintf(file, '%i\n', length);
    
    % Print color definitions
    for i=1:length
        fprintf(file, '%i %i %i %i\n', i, round(colors{i,1}));
    end
 
    % Print the vertices   
    for i=1:length
       BW = RGBDistanceThreshold(I, colors{i,1}, colors{i,2}, t);
       figure;imshow(BW);
       BW_Filled = imfill(BW);
       BW_1 = imerode(BW_Filled, SE);
       BW_2 = imdilate(BW_1, SE);
       BW_3 = imdilate(BW_2, SE);
       BW_4 = imerode(BW_3, SE);
       Vertices = GetGeom(BW_4, true);
        for j=1:size(Vertices)
            fprintf(file, '%i %i %i %s %i\n', Vertices(j, 1), Vertices(j, 2), Vertices(j, 3), shape, i);
        end
    end
end

