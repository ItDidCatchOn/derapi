#ifndef _EXCHANGEOPTIONCONTRACTPAIRSET_
#define _EXCHANGEOPTIONCONTRACTPAIRSET_

#include <map>

#include "ExchangeOptionContractPair.h"

class ExchangeOptionContractPairSet
{
  public:
    typedef std::map<ExchangeOptionContract::t_strike_price, ExchangeOptionContractPair*>::const_iterator const_iterator;
    typedef std::map<ExchangeOptionContract::t_strike_price, ExchangeOptionContractPair*>::size_type size_type;
    ExchangeOptionContractPairSet(): fSet() {}
    ~ExchangeOptionContractPairSet(){Clear();}

    void Clear()
    {
      for(auto it=fSet.begin(); it!=fSet.end(); ++it) delete it->second;
      fSet.clear();
    }

    inline ExchangeOptionContractPair* GetExchangeOptionContractPair(const ExchangeOptionContract::t_strike_price& strike_price) const
    {
      const_iterator selstrike = fSet.find(strike_price);

      if(selstrike == fSet.end()) return NULL;
      return selstrike->second;
    }

    inline ExchangeOptionContractPair& AddExchangeOptionContractPair(const ExchangeOptionContract::t_strike_price& strike_price)
    {
      ExchangeOptionContractPair*& c = fSet[strike_price];

      if(!c) c = new ExchangeOptionContractPair;
      return *c;
    }

    inline void DeleteExchangeOptionContractPair(const_iterator cp)
    {
      if(cp != fSet.end()) {
	delete cp->second;
	fSet.erase(cp);
      }
    }

    inline void DeleteExchangeOptionContractPair(const ExchangeOptionContract::t_strike_price& strike_price){DeleteExchangeOptionContractPair(fSet.find(strike_price));}

    inline const_iterator begin() const{return fSet.begin();}
    inline const_iterator end() const{return fSet.end();}
    inline size_type size() const{return fSet.size();}

  protected:
    std::map<ExchangeOptionContract::t_strike_price, ExchangeOptionContractPair*> fSet;

  private:
    ExchangeOptionContractPairSet(const ExchangeOptionContractPairSet&){}
    const ExchangeOptionContractPairSet& operator=(const ExchangeOptionContractPairSet&){return *this;}
};

#endif
