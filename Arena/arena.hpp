#ifndef arena_hpp
#define arena_hpp

#include "mage.hpp"
#include "using_deklarace.hpp"

#include <map>
#include <memory>

class Arena {
public:
	void team1_add(Mage m) { team1_.insert(std::make_pair(m.get_health(), std::make_unique<Mage>(std::move(m)))); }
	void team2_add(Mage m) { team2_.insert(std::make_pair(m.get_health(), std::make_unique<Mage>(std::move(m)))); }
	void souboj();
private:
	cislo time_;
	team_container team1_;
	team_container team2_;
};

#endif // ! arena_hpp
