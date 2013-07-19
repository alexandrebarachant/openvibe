% OV_MATLAB toolkit
% OV_setChannelLocalizationInputHeader.m
% -------------------------------
% Author : Laurent Bonnet (INRIA)
% Date   : 28/07/2011

% Set the header information on a Stimulation input.

function box_out = OV_setStimulationsInputHeader(box_in, input_index)
    
    box_in.inputs{input_index}.header.type = 'Stimulation Stream';
    
    box_in.inputs{input_index}.buffer = {};
    
    box_out = box_in;
end