#ifndef _EXCHANGEINSTRUMENTSETTREE_
#define _EXCHANGEINSTRUMENTSETTREE_

#include <vector>

#include "Currencies.h"
#include "ExchangeInstrumentSetExpiryMap.h"

class ExchangeInstrumentSetTree
{
  public:
    typedef std::vector<ExchangeInstrumentSetExpiryMap*>::const_iterator const_iterator;
    typedef std::unordered_map<std::string, ExchangeFutureContractBase*>::const_iterator future_contract_const_iterator;
    typedef std::unordered_map<std::string, ExchangeOptionContract*>::const_iterator option_contract_const_iterator;
    typedef std::vector<ExchangeInstrumentSetExpiryMap*>::size_type size_type;
    typedef std::unordered_map<std::string, ExchangeFutureContractBase*>::size_type future_contract_size_type;
    typedef std::unordered_map<std::string, ExchangeOptionContract*>::size_type option_contract_size_type;
    ExchangeInstrumentSetTree(): fTree(), fFutureContracts(), fOptionContracts() {}
    ~ExchangeInstrumentSetTree(){Clear();}
    void Clear(){for(int i=fTree.size()-1; i>=0; --i) delete fTree[i]; fTree.clear(); fFutureContracts.clear(); fOptionContracts.clear();}

    inline ExchangeInstrumentSetExpiryMap& AddInstrumentCurrency(const s_exchange_underlying_asset& underlying_asset)
    {
      if(fTree.size() <= underlying_asset.cur.id) {
	t_currency_id cursize = fTree.size();
	fTree.resize(underlying_asset.cur.id+1);

	for(int i=fTree.size()-1; i>=cursize; --i) fTree[i]=new ExchangeInstrumentSetExpiryMap(underlying_asset);
      }
      return *(fTree[underlying_asset.cur.id]);
    }

    inline ExchangeInstrumentSetExpiryMap* GetExchangeInstrumentSetExpiryMap(const t_currency_id& cid) const
    {
      if(fTree.size() <= cid) return NULL;
      return fTree[cid];
    }

    inline ExchangeInstrumentSetExpiryMap* GetExchangeInstrumentSetExpiryMap(const char* currency) const{return GetExchangeInstrumentSetExpiryMap(sCurrencies[currency].id);}

    inline ExchangeFutureContractBase*& GetFutureContractPtrRef(const char* instrument_name){return fFutureContracts[instrument_name];}
    inline ExchangeOptionContract*& GetOptionContractPtrRef(const char* instrument_name){return fOptionContracts[instrument_name];}

    inline const_iterator begin() const{return fTree.begin();}
    inline const_iterator end() const{return fTree.end();}
    inline size_type size() const{return fTree.size();}

    inline future_contract_const_iterator future_contract_begin() const{return fFutureContracts.begin();}
    inline future_contract_const_iterator future_contract_end() const{return fFutureContracts.end();}
    inline future_contract_size_type future_contract_size() const{return fFutureContracts.size();}

    inline option_contract_const_iterator option_contract_begin() const{return fOptionContracts.begin();}
    inline option_contract_const_iterator option_contract_end() const{return fOptionContracts.end();}
    inline option_contract_size_type option_contract_size() const{return fOptionContracts.size();}

  protected:
    std::vector<ExchangeInstrumentSetExpiryMap*> fTree;
    std::unordered_map<std::string, ExchangeFutureContractBase*> fFutureContracts;
    std::unordered_map<std::string, ExchangeOptionContract*> fOptionContracts;

  private:
    ExchangeInstrumentSetTree(const ExchangeInstrumentSetTree&){}
    const ExchangeInstrumentSetTree& operator=(const ExchangeInstrumentSetTree&){return *this;}
};

#endif
