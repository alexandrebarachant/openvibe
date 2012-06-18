% openvibe toolbox for Matlab
% OV_addInputBuffer.m
% -------------------------------
% Author : Laurent Bonnet (INRIA)
% Date   : 13 Jan. 2012

% Appends a new signal chunk to a signal input buffer in a matlab box instance.
% -------------------------------
% MEANT TO BE USED BY OPENVIBE

function box_out = OV_addInputBuffer(box_in, input_index, start_time, end_time, matrix_data)
	 
    chunk.start_time = start_time;
    chunk.end_time = end_time;
    chunk.matrix_data = matrix_data;    
    box_in.inputs{input_index}.buffer{size(box_in.inputs{input_index}.buffer,2)+1} = chunk;
    
    box_out = box_in;
end