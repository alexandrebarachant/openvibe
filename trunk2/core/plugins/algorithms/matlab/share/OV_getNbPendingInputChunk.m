% OV_MATLAB toolkit
% OV_hasPendingInputChunk.m
% -------------------------------
% Author : Laurent Bonnet (INRIA)
% Date   : 13 Jan. 2012

% Tests if there is pending Input chunk in the buffer
% -------------------------------
% MEANT TO BE USED IN USER SCRIPT

function nb_pending = OV_getNbPendingInputChunk(box_in, input_index)

    if(input_index > size(box_in.inputs,2))
        disp('Error while calling OV_getNbPendingOutputChunk : output index out-of-range.')
        nb_pending = 0;
    else
        nb_pending = size(box_in.inputs{input_index}.buffer,2); 
    end
    
end