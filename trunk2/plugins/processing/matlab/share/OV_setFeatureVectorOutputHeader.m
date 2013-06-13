% OV_MATLAB toolkit
% OV_setFeatureVectorInputHeader.m
% -------------------------------
% Author : Laurent Bonnet (INRIA)
% Date   : 28/07/2011

% Set the header information on a Feature Vector input.

function box_out = OV_setFeatureVectorOutputHeader(box_in, output_index, nb_features, labels)
    
    box_in.outputs{output_index}.header.type = 'Feature Vector Stream';
    box_in.outputs{output_index}.header.nb_features = nb_features;
    box_in.outputs{output_index}.header.labels = labels;
    
    box_in.outputs{output_index}.buffer = {};
    
    box_out = box_in;
end