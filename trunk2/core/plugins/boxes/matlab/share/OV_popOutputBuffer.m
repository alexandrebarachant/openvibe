% OV_MATLAB toolkit
% OV_popOutputBuffer.m
% -------------------------------
% Author : Laurent Bonnet (INRIA)
% Date   : 28/07/2011

% pops the last chunk in the output buffer
% MEANT TO BE USED IN OPENVIBE

function [box_out, start_time, end_time, linear_matrix_size, linear_data] = OV_popOutputBuffer(box_in, output_index)
    
    if size(box_in.outputs{output_index}.buffer,1) == 0
        start_time = -1;
        end_time = -1;
        linear_data = [];
        linear_matrix_size = 0;
    else
        start_time = box_in.outputs{output_index}.buffer{1}.start_time;
        end_time = box_in.outputs{output_index}.buffer{1}.end_time;
        nb_elements = size(box_in.outputs{output_index}.buffer{1}.matrix_data(1:end),2);
        linear_data = box_in.outputs{output_index}.buffer{1}.matrix_data;
        linear_matrix_size = nb_elements;
    end
   
    box_in.outputs{output_index}.buffer = box_in.outputs{output_index}.buffer(2:end);
        
    box_out = box_in;
    
end