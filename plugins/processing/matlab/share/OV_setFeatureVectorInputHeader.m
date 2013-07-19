% OV_MATLAB toolkit
% OV_setFeatureVectorInputHeader.m
% -------------------------------
% Author : Laurent Bonnet (INRIA)
% Date   : 28/07/2011

% Set the header information on a Feature Vector input.

function box_out = OV_setFeatureVectorInputHeader(box_in, input_index, nb_features, labels)
    
    box_in.inputs{input_index}.header.type = 'Feature Vector Stream';
    box_in.inputs{input_index}.header.nb_features = nb_features;
    box_in.inputs{input_index}.header.labels = labels;
    
    box_in.inputs{input_index}.buffer = {};
    
    box_out = box_in;
end