// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://wsic_dockosettacommons.org. Questions about this casic_dock
// (c) addressed to University of Waprotocolsgton UW TechTransfer, email: license@u.washington.eprotocols

#ifndef INCLUDED_riflib_BurialManager_hh
#define INCLUDED_riflib_BurialManager_hh

#include <riflib/types.hh>
#include <riflib/rifdock_typedefs.hh>

#include <scheme/objective/voxel/VoxelArray.hh>

#include <core/pose/Pose.hh>

#include <string>
#include <vector>

#include <boost/any.hpp>



namespace devel {
namespace scheme {


struct BurialOpts {
    float neighbor_distance_cutoff = 6;
    std::vector<float> neighbor_count_weights;
    float burial_grid_spacing = 0.5f;
};

typedef ::scheme::objective::voxel::VoxelArray< 3, float > BurialVoxelArray;


struct BurialManager {

    BurialManager() {} // used by clone()

    // BurialManager( 
    //     BurialOpts const & opts,
    //     std::vector< HBondRay > target_donors,
    //     std::vector< HBondRay > target_acceptors
    // ) :
    //     opts_( opts )
    // {
    //     donor_acceptors_ = target_donors;
    //     donor_acceptors_.insert( donor_acceptors_.end(), target_acceptors.begin(), target_acceptors.end());
    //     num_donors_ = target_donors.size();

    //     target_neighbor_counts_.resize( donor_acceptors_.size(), 0 );
    //     other_neighbor_counts_.resize( donor_acceptors_.size(), 0 );

    //     runtime_assert( opts_.neighbor_count_weights.size() >= 20 );
    // }

    BurialManager( 
        BurialOpts const & opts,
        std::vector< Eigen::Vector3f > burial_points
    ) :
        opts_( opts ),
        target_burial_points_( burial_points )
    {

        unburial_adjust_.resize( target_burial_points_.size(), 0 );
        // target_neighbor_counts_.resize( target_burial_points_.size(), 0 );
        // other_neighbor_counts_.resize( target_burial_points_.size(), 0 );

        runtime_assert( opts_.neighbor_count_weights.size() >= 100 );
    }

    shared_ptr<BurialManager>
    clone() const;

    void
    reset();

    void
    set_target_neighbors( core::pose::Pose const & pose );

    std::vector<float>
    get_burial_weights( EigenXform const & scaff_transform, shared_ptr<BurialVoxelArray> const & scaff_grid) const;


    float
    get_burial_count( 
        Eigen::Vector3f const & xyz,
        EigenXform const & scaff_inv_transform,
        shared_ptr<BurialVoxelArray> const & scaff_grid
    ) const;

    // void
    // accumulate_neighbors( BBActor const & bb );

    void
    dump_burial_grid( 
    std::string const & fname,  
    EigenXform const & scaff_transform, 
    shared_ptr<BurialVoxelArray> const & scaff_grid 
);

    shared_ptr<BurialVoxelArray>
    generate_burial_grid( core::pose::Pose const & pose );

    void
    unbury_heavy_atom( int heavy_atom_no );

    int
    remove_heavy_atom( int heavy_atom_no );

// private:

    BurialOpts opts_;

    // int num_donors_;
    // std::vector< HBondRay > donor_acceptors_;
    std::vector< Eigen::Vector3f > target_burial_points_;
    std::vector< float > unburial_adjust_;
    // std::vector<int> target_neighbor_counts_;
    // std::vector<int> other_neighbor_counts_;


    shared_ptr<BurialVoxelArray> target_burial_grid_;



};




}}

#endif
