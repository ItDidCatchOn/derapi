#ifndef _EXCHANGEINSTRUMENTSETEXPIRYMAP_
#define _EXCHANGEINSTRUMENTSETEXPIRYMAP_

#include <cstdint>

#include <map>

#include "ExchangeInstrument.h"
#include "ExchangeInstrumentSet.h"

class ExchangeInstrumentSetExpiryMap
{
  public:
    typedef std::map<ExchangeInstrument::t_timestamp, ExchangeInstrumentSet*>::const_iterator instrument_set_const_iterator;
    typedef std::map<ExchangeInstrument::t_timestamp, ExchangeInstrumentSet*>::size_type instrument_set_size_type;
    ExchangeInstrumentSetExpiryMap(const s_exchange_underlying_asset& underlying_asset): fMap(), fInstrumentSetRequestTime(0), fUnderlyingAsset(underlying_asset){}
    ~ExchangeInstrumentSetExpiryMap(){Clear();}

    void Clear(){

      for(auto it=fMap.begin(); it!=fMap.end(); ++it) delete it->second;
      fMap.clear();
    }

    inline ExchangeInstrumentSet& AddInstrumentSet(const ExchangeInstrument::t_timestamp& expiry)
    {
      ExchangeInstrumentSet*& s = fMap[expiry];

      if(!s) s = new ExchangeInstrumentSet;
      return *s;
    }

    inline void DeleteInstrumentSet(instrument_set_const_iterator eis)
    {
      if(eis != fMap.end()) {
	delete eis->second;
	fMap.erase(eis);
      }
    }

    inline void DeleteInstrumentSet(const ExchangeInstrument::t_timestamp& expiry){DeleteInstrumentSet(fMap.find(expiry));}

    inline instrument_set_const_iterator FindInstrumentSet(const ExchangeInstrument::t_timestamp& expiry){return fMap.find(expiry);} 

    inline ExchangeInstrumentSet* GetInstrumentSet(const ExchangeInstrument::t_timestamp& expiry)
    {
      instrument_set_const_iterator selexp = fMap.find(expiry);

      if(selexp == fMap.end()) return NULL;
      return selexp->second;    
    }

    inline ExchangeInstrument::t_timestamp& InstrumentSetRequestTime(){return fInstrumentSetRequestTime;}

    int UpdateFutureContractYields();

    inline instrument_set_const_iterator instrument_set_begin() const{return fMap.begin();}
    inline instrument_set_const_iterator instrument_set_end() const{return fMap.end();}
    inline instrument_set_size_type instrument_set_size() const{return fMap.size();}

  protected:
    std::map<ExchangeInstrument::t_timestamp, ExchangeInstrumentSet*> fMap;
    ExchangeInstrument::t_timestamp fInstrumentSetRequestTime;
    const s_exchange_underlying_asset& fUnderlyingAsset;

  private:
    const ExchangeInstrumentSetExpiryMap& operator=(const ExchangeInstrumentSetExpiryMap&){return *this;}
};

#endif
