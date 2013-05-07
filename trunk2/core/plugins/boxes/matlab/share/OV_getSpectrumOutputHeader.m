% openvibe toolbox for Matlab
% OV_getSpectrumOutputHeader.m
% -------------------------------
% Author : Laurent Bonnet (INRIA)
% Date   : 28/07/2011

% Set the header information on a signal output.
% MEANT TO BE USED IN USER SCRIPT

function [errno, nb_channels, channel_names, nb_bands, band_names, bands_linear] = OV_getSpectrumOutputHeader(box_in, output_index)
    
    if(numel(box_in.outputs{output_index}.header) == 0)
        nb_channels = 0;
        channel_names = 0;
        nb_bands = 0;
        band_names = 0;
        bands_linear = 0;
        errno = 1;
    else
        nb_channels = box_in.outputs{output_index}.header.nb_channels;
        channel_names = box_in.outputs{output_index}.header.channel_names;
        nb_bands = box_in.outputs{output_index}.header.nb_bands;
        band_names = box_in.outputs{output_index}.header.band_names;
        bands_linear = reshape(box_in.outputs{output_index}.header.bands,1,numel(box_in.outputs{output_index}.header.bands));
        errno = 0;
    end    
end