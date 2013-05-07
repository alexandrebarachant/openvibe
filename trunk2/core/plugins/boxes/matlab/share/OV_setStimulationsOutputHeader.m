% OV_MATLAB toolkit
% OV_setStimulationOutputHeader.m
% -------------------------------
% Author : Laurent Bonnet (INRIA)
% Date   : 28/07/2011

% Set the header information on a Stimulation input.

function box_out = OV_setStimulationsOutputHeader(box_in, output_index)
    
    box_in.outputs{output_index}.header.type = 'Stimulation Stream';
    
    box_in.outputs{output_index}.buffer = {};
    
    box_out = box_in;
end