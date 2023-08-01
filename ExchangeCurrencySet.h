#ifndef _EXCHANGECURRENCYSET_
#define _EXCHANGECURRENCYSET_

#include <map>
#include <unordered_map>

#include "Currencies.h"

class ExchangeCurrencySet
{
  public:
    typedef std::unordered_map<std::string, s_currency const*>::const_iterator const_iterator;
    typedef std::unordered_map<std::string, s_currency const*>::size_type size_type;
    ExchangeCurrencySet(): fCurrencies(){}
    ExchangeCurrencySet(const ExchangeCurrencySet& rhs): fCurrencies(rhs.fCurrencies), fCurrenciesFromIDs(rhs.fCurrenciesFromIDs), fDefaultCurrency(NULL) {}
    ~ExchangeCurrencySet(){Clear();}

    inline void Clear()
    {
      fCurrencies.clear();
      fCurrenciesFromIDs.clear();
    }

    const ExchangeCurrencySet& operator=(const ExchangeCurrencySet& rhs)
    {
      Clear();
      for(const_iterator cur_it=begin(); cur_it!=end(); ++cur_it) {
	fCurrencies[cur_it->first] = cur_it->second;
	fCurrenciesFromIDs[cur_it->second->id] = cur_it->second;
      }
      return *this;
    }

    inline s_currency const* GetCurrency(const t_currency_id& id) const
    {
      std::map<t_currency_id, s_currency const*>::const_iterator cur = fCurrenciesFromIDs.find(id);

      if(cur == fCurrenciesFromIDs.end()) return NULL;
      return cur->second;
    }

    inline s_currency const* GetCurrency(const char* symbol) const
    {
      const_iterator cur = fCurrencies.find(symbol);

      if(cur == fCurrencies.end()) return NULL;
      return cur->second;
    }

    inline const s_currency& Add(const char* symbol)
    {
      s_currency const*& cur=fCurrencies[symbol];

      if(!cur) {
	cur =  &sCurrencies[symbol];
	fCurrenciesFromIDs[cur->id] = cur;
      }

      return *cur;
    }

    inline const s_currency& GetDefaultCurrency() const {return *fDefaultCurrency;}

    inline void Remove(const char* symbol)
    {
      std::unordered_map<std::string, s_currency const*>::const_iterator cur_it=fCurrencies.find(symbol);

      if(cur_it != fCurrencies.end()) {
	fCurrencies.erase(cur_it);
	fCurrenciesFromIDs.erase(cur_it->second->id);
      }
    }

    inline const s_currency& SetDefaultCurrency(const char* symbol)
    {
      fDefaultCurrency=&Add(symbol);

      return *fDefaultCurrency;
    }

    inline const_iterator begin() const{return fCurrencies.begin();}
    inline const_iterator end() const{return fCurrencies.end();}
    inline size_type size() const{return fCurrencies.size();}

  protected:
    std::unordered_map<std::string, s_currency const*> fCurrencies;
    std::map<t_currency_id, s_currency const*> fCurrenciesFromIDs;
    s_currency const* fDefaultCurrency;

  private:
};

#endif
