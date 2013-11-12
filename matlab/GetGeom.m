function [ out ] = GetGeom(BW, SE, reverse)

    [rS, cS] = size(BW);

    Eroded = imerode(BW, SE);
    
    Dilated = imdilate(Eroded, SE);
    
    
    [Labeled, n] = bwlabel(Dilated, 8);
    %imshow(Labeled, [0 n]);
    
    out = zeros(0,3);
    
    for i=1:n
        [r, c] = find(Labeled==i);
        radius = sqrt(size(r,1)/pi);
        if reverse
            out(end+1,1) = cS - mean(c);
            out(end,2) = rS - mean(r);
            out(end, 3) = radius;
        else
            out(end+1,1) = mean(c);
            out(end,2) = mean(r);
            out(end, 3) = radius;
        end
        
    end
    out = round(out);


end

