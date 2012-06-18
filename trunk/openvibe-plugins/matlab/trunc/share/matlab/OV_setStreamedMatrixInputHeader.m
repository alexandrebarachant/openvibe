% OV_MATLAB toolkit
% OV_setStreamedMatrixInputHeader.m
% -------------------------------
% Author : Laurent Bonnet (INRIA)
% Date   : 28/07/2011

% Set the header information on a streamed Matrix input.

function box_out = OV_setStreamedMatrixInputHeader(box_in, input_index, nb_dim, dim_sizes, dim_labels)
    
    box_in.inputs{input_index}.header.type = 'Streamed Matrix Stream';
    box_in.inputs{input_index}.header.nb_dimensions = nb_dim;
    box_in.inputs{input_index}.header.dimension_sizes = dim_sizes;
    box_in.inputs{input_index}.header.dimension_labels = dim_labels;
    
    box_in.inputs{input_index}.buffer = {};
    
    box_out = box_in;
end