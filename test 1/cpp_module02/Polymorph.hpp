#ifndef POLYMORPH_HPP
# define POLYMORPH_HPP

# include <iostream>
# include "ASpell.hpp"

class ASpell;

class Polymorph : public ASpell
{
    public:
        Polymorph(void);
        ~Polymorph(void);
        ASpell* clone(void) const;
};

#endif
