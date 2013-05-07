% OV_MATLAB toolkit
% OV_setSignalInputHeader.m
% -------------------------------
% Author : Laurent Bonnet (INRIA)
% Date   : 28/07/2011

% Set the header information on a signal input.

function box_out = OV_setSignalInputHeader(box_in,  input_index, nb_channel, nb_samples_per_buffer, channel_names, sampling_rate)
    
    box_in.inputs{input_index}.header.type = 'Signal Stream';
    box_in.inputs{input_index}.header.nb_channels = nb_channel;
    box_in.inputs{input_index}.header.nb_samples_per_buffer = nb_samples_per_buffer;
    box_in.inputs{input_index}.header.channel_names = channel_names;
    box_in.inputs{input_index}.header.sampling_rate = sampling_rate;
    
    box_in.inputs{input_index}.buffer = {};
    
    box_out = box_in;
end