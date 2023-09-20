#ifndef FIREBALL_HPP
# define FIREBALL_HPP

# include <iostream>
# include "ASpell.hpp"

class ASpell;

class Fireball : public ASpell
{
    public:
        Fireball(void);
        ~Fireball(void);
        ASpell* clone(void) const;
};

#endif
