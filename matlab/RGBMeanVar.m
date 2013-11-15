function [ Colors ] = RGBMeanVar( ColorSamples )
    
    Colors = cell(size(ColorSamples), 2);

    for i=1:size(ColorSamples);
        Colors{i,1} = mean(ColorSamples{i}, 1);
        Colors{i,2} = std(ColorSamples{i}, 1);
    end
end

