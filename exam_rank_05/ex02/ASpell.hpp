#ifndef ASPELL_HPP
# define ASPELL_HPP

# include <iostream>
# include "ATarget.hpp"

class ATarget;

class ASpell
{
	public:
		ASpell(void);
		ASpell(const ASpell& other);
		ASpell(const std::string& name, const std::string& effects);
		virtual ~ASpell(void);

		ASpell& operator=(const ASpell& other);

		std::string getName(void) const;
		std::string getEffects(void) const;
		virtual ASpell* clone(void) const = 0;
		void launch(const ATarget& target);
	private:
		std::string _name;
		std::string _effects;
};

#endif
