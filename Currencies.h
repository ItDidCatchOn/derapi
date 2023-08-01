#ifndef _CURRENCIES_
#define _CURRENCIES_

#include "NamedIndices.h"

typedef s_namedindex s_currency;
typedef t_namedindex_id t_currency_id;

class Currencies: public NamedIndices
{
  public:
    Currencies(): NamedIndices() {}
    ~Currencies(){}

  private:
    Currencies const& operator=(const Currencies&){return *this;}
};

extern Currencies sCurrencies;

#endif
