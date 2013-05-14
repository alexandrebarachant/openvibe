% OV_MATLAB toolkit
% OV_setSignalOutputHeader.m
% -------------------------------
% Author : Laurent Bonnet (INRIA)
% Date   : 28/07/2011

% Set the header information on a signal output.
% MEANT TO BE USED IN USER SCRIPT

function box_out = OV_setSignalOutputHeader(box_in, output_index, nb_channel, nb_samples_per_buffer, channel_names, sampling_rate)
    
    box_in.outputs{output_index}.header.type = 'Signal Stream';
    box_in.outputs{output_index}.header.nb_channels = nb_channel;
    box_in.outputs{output_index}.header.nb_samples_per_buffer = nb_samples_per_buffer;
    box_in.outputs{output_index}.header.channel_names = channel_names;
    box_in.outputs{output_index}.header.sampling_rate = sampling_rate;
    
    box_in.outputs{output_index}.buffer = {};
    
    box_out = box_in;
end