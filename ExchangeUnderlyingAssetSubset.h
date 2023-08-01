#ifndef _EXCHANGEUNDERLYINGASSETSUBSET_
#define _EXCHANGEUNDERLYINGASSETSUBSET_

#include <cinttypes>

#include <map>
#include <unordered_map>

#include "ExchangeCurrencySet.h"

typedef struct _s_exchange_underlying_asset{
  inline _s_exchange_underlying_asset(s_currency const& cur): cur(cur){} 
  inline _s_exchange_underlying_asset(const _s_exchange_underlying_asset& rhs): cur(rhs.cur), value(rhs.value), request_time(rhs.request_time){} 
  s_currency const& cur;
  double value;
  uint64_t request_time;
} s_exchange_underlying_asset;

class ExchangeUnderlyingAssetSubset
{
  public:
    typedef std::unordered_map<std::string, s_exchange_underlying_asset*>::const_iterator const_iterator;
    typedef std::unordered_map<std::string, s_exchange_underlying_asset*>::size_type size_type;
    ExchangeUnderlyingAssetSubset(const ExchangeCurrencySet& cur_set): fCurSet(cur_set), fCurrencies(){AddDefaultCurrency();}
    ~ExchangeUnderlyingAssetSubset(){Clear();}

    inline s_exchange_underlying_asset* GetCurrency(const t_currency_id& id)
    {
      std::map<t_currency_id, s_exchange_underlying_asset*>::const_iterator cur = fCurrenciesFromIDs.find(id);

      if(cur == fCurrenciesFromIDs.end()) return NULL;
      return cur->second;
    }

    inline s_exchange_underlying_asset* GetCurrency(const char* symbol)
    {
      const_iterator cur = fCurrencies.find(symbol);

      if(cur == fCurrencies.end()) return NULL;
      return cur->second;
    }

    inline s_exchange_underlying_asset const& operator[](const t_currency_id& id)
    {
      std::map<t_currency_id, s_exchange_underlying_asset*>::const_iterator cur = fCurrenciesFromIDs.find(id);

      if(cur == fCurrenciesFromIDs.end()) {
	fprintf(stderr,"%s: Error: Currency ID '%" PRIu16 "' is not part of the exchange currency set!\n",__func__,id);
	throw 1;
      }
      return *cur->second;
    }

    inline s_exchange_underlying_asset const& operator[](const char* symbol)
    {
      const_iterator cur = fCurrencies.find(symbol);

      if(cur == fCurrencies.end()) {
	fprintf(stderr,"%s: Error: Currency '%s' is not part of the exchange currency set!\n",__func__,symbol);
	throw 1;
      }
      return *cur->second;
    }

    inline const s_exchange_underlying_asset& Add(const char* symbol)
    {
      s_currency const* const& cur=fCurSet.GetCurrency(symbol);

      if(!cur) {
	fprintf(stderr,"%s: Error: Currency '%s' is not part of the exchange currency set!\n",__func__,symbol);
	throw 1;
      }

      s_exchange_underlying_asset*& asset = fCurrencies[symbol];

      if(asset) return *asset;

      asset = new s_exchange_underlying_asset(*cur);
      fCurrenciesFromIDs[cur->id] = asset;

      return *asset;
    }

    inline void AddAll()
    {
      Clear();

      for(auto cur_it=fCurSet.begin(); cur_it!=fCurSet.end(); ++cur_it) {
	s_exchange_underlying_asset*& asset = fCurrencies[cur_it->first];

	if(!asset) {
	  asset = new s_exchange_underlying_asset(*cur_it->second);
          fCurrenciesFromIDs[cur_it->second->id] = asset;
	}
      }
    }

    inline void Remove(const char* symbol)
    {
      std::unordered_map<std::string, s_exchange_underlying_asset*>::const_iterator cur_it=fCurrencies.find(symbol);

      if(cur_it != fCurrencies.end()) {
	fCurrencies.erase(cur_it);
	fCurrenciesFromIDs.erase(cur_it->second->cur.id);
	delete cur_it->second;
      }
    }

    inline void RemoveAll()
    {
      Clear();
      AddDefaultCurrency();
    }

    inline const s_exchange_underlying_asset& GetDefaultCurrency() const {return *fDefaultCurrency;}

    inline const_iterator begin() const{return fCurrencies.begin();}
    inline const_iterator end() const{return fCurrencies.end();}
    inline size_type size() const{return fCurrencies.size();}

  protected:
    inline void Clear()
    {
      for(const_iterator cur_it=begin(); cur_it!=end(); ++cur_it) delete cur_it->second;
      fCurrencies.clear();
      fCurrenciesFromIDs.clear();
    }

    inline void AddDefaultCurrency()
    {
      s_currency const* const cur = &fCurSet.GetDefaultCurrency();

      fDefaultCurrency = new s_exchange_underlying_asset(*cur);
      fCurrencies[cur->symbol] = fDefaultCurrency;
      fCurrenciesFromIDs[cur->id] = fDefaultCurrency;
      fDefaultCurrency->request_time = UINT64_MAX;
      fDefaultCurrency->value = 1;
    }

    const ExchangeCurrencySet& fCurSet;
    std::unordered_map<std::string, s_exchange_underlying_asset*> fCurrencies;
    std::map<t_currency_id, s_exchange_underlying_asset*> fCurrenciesFromIDs;
    s_exchange_underlying_asset* fDefaultCurrency;

  private:
    ExchangeUnderlyingAssetSubset(const ExchangeUnderlyingAssetSubset& rhs): fCurSet(rhs.fCurSet), fCurrencies(rhs.fCurrencies), fCurrenciesFromIDs(rhs.fCurrenciesFromIDs) {}
    const ExchangeUnderlyingAssetSubset& operator=(const ExchangeUnderlyingAssetSubset& rhs) {return *this;}

};

#endif
