% OV_MATLAB toolkit
% OV_setSpectrumOutputHeader.m
% -------------------------------
% Author : Laurent Bonnet (INRIA)
% Date   : 28/07/2011

% Set the header information on a Spectrum input.

function box_out = OV_setSpectrumOutputHeader(box_in, output_index, nb_channels, channel_names, nb_bands, band_names, bands)
    
    box_in.outputs{output_index}.header.type = 'Spectrum Stream';
    box_in.outputs{output_index}.header.nb_channels = nb_channels;
    box_in.outputs{output_index}.header.channel_names = channel_names;
    box_in.outputs{output_index}.header.nb_bands = nb_bands;
    box_in.outputs{output_index}.header.band_names = band_names;
    box_in.outputs{output_index}.header.bands = bands;
    
    box_in.outputs{output_index}.buffer = {};
    
    box_out = box_in;
end