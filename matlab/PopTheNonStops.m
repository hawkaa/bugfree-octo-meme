function [] = PopTheNonStops(I, filename, colors, SE, shape)

    file = fopen(filename, 'w+');

    [c,length] = size(colors);
    
    % Number of colors
    fprintf(file, '%i\n', length);
    
    % Print color definitions
    for i=1:length
        fprintf(file, '%i %i %i %i\n', i, colors{i}{1});
    end
    
    
    % Print the vertices
    
    for i=1:length
       BW = RGBThreshold(I, colors{i}{2});
       Vertices = GetGeom(BW, SE, true);
       for j=1:size(Vertices)
           fprintf(file, '%i %i %i %s %i\n', Vertices(j, 1), Vertices(j, 2), Vertices(j, 3), shape, i);
       end
    end
end

