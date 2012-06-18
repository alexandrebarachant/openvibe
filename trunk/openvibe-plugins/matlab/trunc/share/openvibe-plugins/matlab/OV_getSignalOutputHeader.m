% openvibe toolbox for Matlab
% OV_getSignalOutputHeader.m
% -------------------------------
% Author : Laurent Bonnet (INRIA)
% Date   : 28/07/2011

% Set the header information on a signal output.
% MEANT TO BE USED IN USER SCRIPT

function [errno, nb_channels, nb_samples_per_buffer, channel_names, sampling_rate] = OV_getSignalOutputHeader(box_in, output_index)
    
    if(numel(box_in.outputs{output_index}.header) == 0)
        nb_channels = 0;
        nb_samples_per_buffer = 0;
        channel_names = 0;
        sampling_rate = 0;
        errno = 1;
    else
        nb_channels = box_in.outputs{output_index}.header.nb_channels;
        nb_samples_per_buffer = box_in.outputs{output_index}.header.nb_samples_per_buffer;
        channel_names = box_in.outputs{output_index}.header.channel_names;
        sampling_rate = box_in.outputs{output_index}.header.sampling_rate;
        errno = 0;
    end
    
end