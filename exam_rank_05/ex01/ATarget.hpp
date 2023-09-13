#ifndef ATARGET_HPP
# define ATARGET_HPP

# include "ASpell.hpp"

class ASpell;

class ATarget
{
	public:
		ATarget(void);
		ATarget(const ATarget& other);
		ATarget(const std::string& type);
		virtual ~ATarget(void);

		ATarget& operator=(const ATarget& other);

		const std::string& getType(void) const;
		void getHitBySpell(const ASpell& spell) const;
	private:
		std::string _type;
};

#endif
