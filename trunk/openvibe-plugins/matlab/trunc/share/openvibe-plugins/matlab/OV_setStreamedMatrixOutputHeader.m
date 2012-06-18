% OV_MATLAB toolkit
% OV_setStreamedMatrixOutputHeader.m
% -------------------------------
% Author : Laurent Bonnet (INRIA)
% Date   : 28/07/2011

% Set the header information on a signal output.
% MEANT TO BE USED IN USER SCRIPT

function box_out = OV_setStreamedMatrixOutputHeader(box_in, output_index, nb_dim, dim_sizes, dim_labels)
    
    box_in.outputs{output_index}.header.type = 'Streamed Matrix Stream';
    box_in.outputs{output_index}.header.nb_dimensions = nb_dim;
    box_in.outputs{output_index}.header.dimension_sizes = dim_sizes;
    box_in.outputs{output_index}.header.dimension_labels = dim_labels;
    
    box_in.outputs{output_index}.buffer = {};
    
    box_out = box_in;
end