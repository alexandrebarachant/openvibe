% OV_MATLAB toolkit
% OV_hasPendingOutputChunk.m
% -------------------------------
% Author : Laurent Bonnet (INRIA)
% Date   : 13 Jan. 2012

% Tests if there is pending output chunk in the buffer
% -------------------------------
% MEANT TO BE USED IN USER SCRIPT

function nb_pending = OV_getNbPendingOutputChunk(box_in, output_index)
    
    if(output_index > size(box_in.outputs,2))
        disp('Error while calling OV_getNbPendingOutputChunk : output index out-of-range.')
        nb_pending = 0;
    else
        nb_pending = size(box_in.outputs{output_index}.buffer,2); 
    end
    
end
