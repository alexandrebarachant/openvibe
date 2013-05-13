% openvibe toolbox for Matlab
% OV_getSignalOutputHeader.m
% -------------------------------
% Author : Laurent Bonnet (INRIA)
% Date   : 28/07/2011

% Set the header information on a signal output.
% MEANT TO BE USED IN USER SCRIPT

function [errno, nb_features, labels] = OV_getFeatureVectorOutputHeader(box_in, output_index)
    
    if(numel(box_in.outputs{output_index}.header) == 0)
        nb_features = 0;
        labels = 0;
        errno = 1;
    else
        nb_features = box_in.outputs{output_index}.header.nb_features;
        labels = box_in.outputs{output_index}.header.labels;
        errno = 0;
    end
    
end