#include "read_input.hpp"

#include <iostream>
#include <string>
#include <cctype>
#include <iomanip>

// Funkce pro porovnani stringu bez ohledu na velikost pismen prevzata z https://stackoverflow.com/questions/23943728/case-insensitive-standard-string-comparison-in-c
bool icompare_pred(unsigned char a, unsigned char b)
{
	return std::tolower(a) == std::tolower(b);
}

bool icompare(std::string const& a, std::string const& b)
{
	if (a.length() == b.length()) {
		return std::equal(b.begin(), b.end(),
			a.begin(), icompare_pred);
	}
	else {
		return false;
	}
}

void write_help(std::ostream & out) { // TODO: Zarovnat text doleva
	int odsazeni1 = 3, odsazeni2 = 6, odsazeni3 = 9;
	int odsazeni = 25;
	out << std::setw(odsazeni1) << "" << std::setw(odsazeni - odsazeni1) << std::left << "Mage name " << "\t- zacne vytvaret noveho maga se jmenem 'name'" << std::endl	// Mage name
		<< std::setw(odsazeni2) << "" << std::setw(odsazeni - odsazeni2) << std::left << "Library " << "\t- vypise seznam kouzel, ktere se mag muze naucit" << std::endl	// School
		<< std::setw(odsazeni3) << "" << std::setw(odsazeni - odsazeni3) << std::left << "Learn 'name' " << "\t- nauci maga kouzlo se jmenem 'name'" << std::endl	// Learn spell_name
		<< std::setw(odsazeni2) << "" << std::setw(odsazeni - odsazeni2) << std::left << "Shop " << "\t- vypise seznam veci, ktere si mag muze koupit" << std::endl	// Shop
		<< std::setw(odsazeni3) << "" << std::setw(odsazeni - odsazeni3) << std::left << "Buy weapon 'name' " << "\t- mag si koupi zbran se jmenem 'name'" << std::endl	// Buy item_name
		<< std::setw(odsazeni3) << "" << std::setw(odsazeni - odsazeni3) << std::left << "Buy robe 'name' " << "\t- mag si koupi robu se jmenem 'name'" << std::endl	// Buy item_name
		<< std::setw(odsazeni2) << "" << std::setw(odsazeni - odsazeni2) << std::left << "Show " << "\t- vypise aktualni vlastnost maga" << std::endl	// Show
		<< std::setw(odsazeni2) << "" << std::setw(odsazeni - odsazeni2) << std::left << "Team [12] " << "\t- ukonceni procesu vytvareni postavy a zarazeni maga do tymu [12]" << std::endl	// Team
		<< std::setw(odsazeni1) << "" << std::setw(odsazeni - odsazeni1) << std::left << "Fight " << "\t- zahaji souboj s vytvorenymi magy" << std::endl	// Fight
		<< std::setw(odsazeni1) << "" << std::setw(odsazeni - odsazeni1) << std::left << "Exit " << "\t- ukonci program" << std::endl	// Konec
		<< std::endl;
}

void write_items( const Shop & shop) {
	shop.show_weapons();
	shop.show_robes();
	// TODO: Pridat dalsi druhy vybaveni
}

void team(Arena & arena, Mage mage, int i) { // Tady predavam hodnotou, budu presouvat vytvoreneho maga
	switch (i)
	{
	case 1:
		arena.out << mage.get_name() << " byl zarazen do tymu 1" << std::endl;
		arena.team1_add(std::move(mage));
		break;
	case 2:
		arena.out << mage.get_name() << " byl zarazen do tymu 2" << std::endl;
		arena.team2_add(std::move(mage));
		break;
	default:
		break;
	}
}

void my_getline(std::istream & is, std::string & vstup) { // Potrebuju, aby na zacatku preskocil mezery a aby prvni pismeno bylo velke a ostatni male
	std::getline(is, vstup);
	while (vstup[0] == ' ')
		vstup.erase(0, 1);
	for (auto && ch : vstup)
		ch = tolower(ch);
	vstup[0] = toupper(vstup[0]);
}

void read_input( Arena & arena, Knihovna & knihovna, Shop & shop)
{	
	arena.out << "Vytvorit noveho maga je mozne napsanim prikazu Mage 'jmeno'. Pro dalsi napovedu napis help. Na velikosti pismen nezalezi. " << std::endl;
	std::string vstup;
	arena.in >> vstup; // TODO: Ukonci to cteni na konci radky?
	while (!icompare(vstup, "Exit")) {
		if		(icompare(vstup, "help")) write_help(arena.out);
		else if (icompare(vstup, "Library")) knihovna.show_spells();
		else if (icompare(vstup, "Shop")) write_items( shop);
		else if (icompare(vstup, "Mage")) {					// Vytvor maga
			arena.in >> vstup; 
			Mage mage{ vstup, arena };
			arena.out << mage.get_name() << " muze nyni navstivit knihovnu nebo obchod." << std::endl;
			arena.in >> vstup;
			while (!icompare(vstup, "Team")) {				// A dokud ho nezaradis do tymu, muzes ho upravovat
				if		(icompare(vstup, "help")) write_help(arena.out);
				else if (icompare(vstup, "Show")) mage.show_stats();
				else if (icompare(vstup, "Library")) knihovna.show_spells();
				else if (icompare(vstup, "Learn")) { my_getline(arena.in, vstup); mage.learn( knihovna.get_spell( vstup)); mage.show_stats(); }
				else if (icompare(vstup, "Shop")) write_items(shop);
				else if (icompare(vstup, "Buy")) { 
					arena.in >> vstup; 
					if		(icompare(vstup, "weapon")) { my_getline(std::cin, vstup); mage.buy_weapon( shop.get_weapon( vstup)); } // TODO: Vyjimky, pokud spatne napisu jmeno (kouzla + vsechny predmety)
					else if (icompare(vstup, "robe")) { 
						my_getline(std::cin, vstup); 
						mage.buy_robe( shop.get_robe(vstup));
					}
					else arena.out << "Neznamy typ predmetu: >>" << vstup << "<<" << std::endl;
					mage.show_stats();
				}
				else if (icompare(vstup, "Fight")) arena.out << "Pred pouzitim prikazu >>" << vstup << "<< musis nejprve zaradit maga do tymu" << std::endl;
				else if (icompare(vstup, "Exit")) break;
				else arena.out << "Neznamy prikaz: >>" << vstup << "<<" << std::endl;
				arena.in >> vstup;
			}
			arena.in >> vstup;
			while (vstup != "1" && vstup != "2") {
				arena.out << "Napis cislo tymu: ";
				arena.in >> vstup;
			}
			team(arena, std::move(mage), std::stoi(vstup));
		}
		else if (icompare(vstup, "Fight")) arena.souboj();
		else if (icompare(vstup, "Show") || icompare(vstup, "Learn") || icompare(vstup, "Buy")) arena.out << "Pro prikaz >>" << vstup << "<< je nutne nejprve vytvorit Maga." << std::endl;
		else arena.out << "Neznamy prikaz: >>" << vstup << "<<" << std::endl;
		arena.in >> vstup;
	}
}