% openvibe toolbox for Matlab
% OV_getStreamedMatrixOutputHeader.m
% -------------------------------
% Author : Laurent Bonnet (INRIA)
% Date   : 28/07/2011

% Set the header information on a signal output.
% MEANT TO BE USED IN USER SCRIPT

function [errno, nb_dimensions, dimension_sizes, dimension_labels] = OV_getStreamedMatrixOutputHeader(box_in, output_index)

    if(numel(box_in.outputs{output_index}.header) == 0)
        nb_dimensions = 0;
        dimension_sizes = 0;
        dimension_labels = 0;
        errno = 1;
    else
        nb_dimensions = box_in.outputs{output_index}.header.nb_dimensions;
        dimension_sizes = box_in.outputs{output_index}.header.dimension_sizes;
        % we produce a 1 dimension array (linear array of labels. it's easier
        % to process in the C++ code.
        dimension_labels = horzcat(box_in.outputs{output_index}.header.dimension_labels{1:end});
        errno = 0;
    end

end