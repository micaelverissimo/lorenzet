__all__ = [ 'lorenzetti_cells_grid_builder' ]

import os
import json
import logging

import numpy as np
import math

logger = logging.getLogger('__name__')
logger.setLevel('INFO')

class lorenzetti_cells_grid_builder(object):
    '''
    A class to create the Lorenzetti cell grid in term os (phi, eta) using any granularity.
    Parameters:
    layer_dict: a dictionary contains the following keys:
        * distance_to_ip : the maximum distante to the interaction point (0,0,0) of the detector.
        * nominal_size   : half the size of the detector.
        * delta_eta      : the cell size in term of eta. The eta center is delta_eta divided by 2.
        * delta_eta      : same of delta_eta but using phi.
    '''
    def __init__(self, layer_dict, logger_level='INFO'):
        logger.setLevel(logger_level)
        logging.info('init lorenzetti_cells_grid_builder... ')
        self.layer_dict = layer_dict

    def get_layer_info(self, layer):#, distance_to_ip, detector_size, delta_eta, delta_phi):
        self.detector_size  = self.layer_dict['nominal_size']
        self.eta_step_size  = self.layer_dict[layer]['delta_eta']/2.
        self.phi_step_size  = self.layer_dict[layer]['delta_phi']/2.
        # Because is a circle this is the radius in the transverse plane
        self.r_xy           = math.sqrt((self.layer_dict[layer]['max_dist_to_ip']**2 + 
                                       self.layer_dict[layer]['max_dist_to_ip']**2))
        self.eta_max        = self.calculate_eta_max(self.detector_size)
        # Because eta is symmetric
        self.eta_min        = -1*self.eta_max
    
    def get_theta(self):
        return self.theta

    def get_eta_bounds(self):
        return [self.eta_min, self.eta_max]
    
    def get_phi_bounds(self):
        return [-1.*np.pi, np.pi]

    def get_r_xy(self):
        return self.r_xy

    def calculate_eta_max(self, r_z):
        '''
        This method will calculate the maximum of eta for the layer.
        Parameters:
        r_z: the size of the detector.
        '''
        self.theta   = math.atan(self.r_xy/r_z)
        eta_max      = -1*math.log(math.tan(self.theta/2.))
        return eta_max
    
    def calculate_cell_centers(self, is_eta):
        '''
        This method will calculate the cell centers based on the delta_eta or delta_phi.
        Parameters:
        is_eta: this parameter is used to calculate the center in eta. 
                if False the center will calculate in terms of phi.
        '''
        if is_eta:
            return np.arange(self.eta_min+self.eta_step_size, self.eta_max,
                             step=self.eta_step_size).tolist()
        else:
            return np.arange(-1.*np.pi+self.phi_step_size, np.pi,
                             step=self.phi_step_size).tolist()

    def get_all_cell_centers(self):
        for layer in self.layer_dict.keys():
            if layer == 'nominal_size':
                continue
            temp_dict = self.layer_dict[layer]
            logging.info(('Creating the cell center for %s using:' %(layer) + 
                   'max_radius: %1.3f \n delta_eta: %1.4f \n and delta_phi: %1.4f')
                   %(temp_dict['max_dist_to_ip'],
                     temp_dict['delta_eta'],
                     temp_dict['delta_phi']))
            
            self.get_layer_info(layer)

            # Get the cell centers and the boundaries
            self.layer_dict[layer]['eta_centers'] = self.calculate_cell_centers(is_eta=True)
            self.layer_dict[layer]['eta_bounds']  = self.get_eta_bounds()
            self.layer_dict[layer]['phi_centers'] = self.calculate_cell_centers(is_eta=False)
            self.layer_dict[layer]['eta_bounds']  = self.get_phi_bounds()
            logging.info(60*'-')
    
    def dump_dict(self, path, name):
        logging.info('Save layer dict with name: %s' %(name))
        json.dump(self.layer_dict, open(os.path.join(path, name+'.json'), 'w'))