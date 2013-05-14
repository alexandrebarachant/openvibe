% OV_MATLAB toolkit
% OV_popInputBuffer.m
% -------------------------------
% Author : Laurent Bonnet (INRIA)
% Date   : 13 Jan. 2012

% Pops the first signal chunk on an input
% -------------------------------
% output = matrix data (matlab format) 
% -------------------------------
% MEANT TO BE USED IN USER SCRIPT

function [box_out, start_time,end_time, matrix_data] = OV_popInputBuffer(box_in, input_index)
  
    if size(box_in.inputs{input_index}.buffer,2) == 0
        start_time = -1;
        end_time = -1;
        matrix_data = [];
    else
        start_time = box_in.inputs{input_index}.buffer{1}.start_time;
        end_time = box_in.inputs{input_index}.buffer{1}.end_time;
        matrix_data = box_in.inputs{input_index}.buffer{1}.matrix_data;
    end
   
    box_in.inputs{input_index}.buffer = box_in.inputs{input_index}.buffer(2:end);
        
    box_out = box_in;
    
end