% openvibe toolbox for Matlab
% OV_addOutputBuffer.m
% -------------------------------
% Author : Laurent Bonnet (INRIA)
% Date   : 13 Jan. 2012

% Appends a new chunk in an output buffer, 
% meant to be popped by the openvibe box.
% -------------------------------
% MEANT TO BE USED IN USER SCRIPT

function box_out = OV_addOutputBuffer(box_in, output_index, start_time, end_time, matrix_data)
	
    chunk.start_time = start_time;
    chunk.end_time = end_time;
    chunk.matrix_data = matrix_data;
    box_in.outputs{output_index}.buffer{size(box_in.outputs{output_index}.buffer,2)+1} = chunk;
   
    box_out = box_in;
end