#ifndef ASPELL_HPP
# define ASPELL_HPP

# include <iostream>
# include "ATarget.hpp"

class ATarget;

class ASpell
{
    protected:
        std::string _name;
        std::string _effects;
    public:
        ASpell(const std::string name, const std::string effects);
        virtual ~ASpell(void);
        ASpell(void);
        ASpell(const ASpell& other);
        ASpell& operator=(const ASpell& other);
    
        std::string getName(void) const;
        std::string getEffects(void) const;
        virtual ASpell* clone(void) const = 0;
        void launch(const ATarget& target) const;
};

#endif
