#ifndef ATARGET_HPP
# define ATARGET_HPP

# include <iostream>
# include "ASpell.hpp"

class ASpell;

class ATarget
{
    protected:
        std::string _type;
    public:
        ATarget(const std::string type);
        virtual ~ATarget(void);
        ATarget(void);
        ATarget(const ATarget& other);
        ATarget& operator=(const ATarget& other);
        
        std::string getType(void) const;
        virtual ATarget* clone(void) const = 0;
        void getHitBySpell(const ASpell& spell) const;
};

#endif
