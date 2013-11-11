function [ out ] = GetGeom(BW, SE)


    Eroded = imerode(BW, SE);
    
    Dilated = imdilate(Eroded, SE);
    
    
    [Labeled, n] = bwlabel(Dilated, 8);
    %imshow(Labeled, [0 n]);
    
    out = zeros(0,3);
    
    for i=1:n
        [r, c] = find(Labeled==i);
        radius = sqrt(size(r,1)/pi);
        out(end+1,1) = mean(c);
        out(end,2) = mean(r);
        out(end, 3) = radius;
    end
    out = round(out);


end
