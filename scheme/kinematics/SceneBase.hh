#ifndef INCLUDED_kinematics_SceneBase_HH
#define INCLUDED_kinematics_SceneBase_HH

#include "scheme/types.hh"
#include <vector>

namespace scheme {
namespace kinematics {

template<
	class _Position,
	class _Index = uint64_t
>
struct SceneBase {
	typedef _Index Index;
	typedef _Position Position;

	// members
		std::vector<Position> symframes_; // include identity at first position
		Index n_sym_bodies_, n_bodies_;
		std::vector<Position> positions_;

	SceneBase() : n_bodies_(0), n_sym_bodies_(0), symframes_(1,Position::Identity()) {}

	// void set_position( Index i, Position const & newp ){ positions_.at(i) = newp; }
	// Position position( Index i ) const { return positions_.at(i); }

	Position position(Index i) const {
		Index isym = this->sym_index_map(i);
		return this->symframes_.at(isym) * positions_.at(i);
	}
	void set_position(Index i, Position const & newp){
		positions_.at(i) = newp;
	}

	// symmetry stuff, doesn't need to be Conformation-sepcific
		void update_symmetry( Index nbodies ){
			n_bodies_ = nbodies;
			n_sym_bodies_ = n_bodies_*((Index)symframes_.size());
		}
		Index sym_index_map(Index & i) const {
			Index isym = i / n_bodies_;
			i = i % n_bodies_;
			return isym;
		}
		void set_symmetry(std::vector<Position> const & sym){ symframes_ = sym; update_symmetry(n_bodies_); }
		void add_symframe(Position const & symframe){ symframes_.push_back(symframe); update_symmetry(n_bodies_); }
		std::vector<Position> const & symframes() const { return symframes_; }
		Index nbodies() const { return n_sym_bodies_; }
		Index nbodies_asym() const { return n_bodies_; }
};


}
}

#endif