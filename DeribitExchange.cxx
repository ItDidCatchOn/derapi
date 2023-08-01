#include "DeribitExchange.h"

int DeribitExchange::UpdateCurrencies()
{
  if(!fEP->Request(der_get_currencies, derempty)) {
    json_object* jobj;

    if((jobj=fEP->GetResult())) {

      if(json_object_get_type(jobj)!=json_type_array) {
	fprintf(stderr,"%s: Error: Returned result does not contain an array of currencies!\n",__func__);
	return -2;
      }
      int nentries=json_object_array_length(jobj);
      json_object *val, *val2;

      for(int i=0; i<nentries; ++i) {
	val=json_object_array_get_idx(jobj,i);

	if(json_object_object_get_ex(val, "currency", &val2)) {
	  fCurrencies.Add(json_object_get_string(val2));

	} else {
	  fprintf(stderr,"%s: Error: Returned currency array element does not have a 'currency' entry!\n",__func__);
	  return -3;
	}
      }
      return 0;
    }
  }
  return -1;
}

int DeribitExchange::LoadNewFutureContractData(ExchangeInstrumentSetExpiryMap* em, const char* instr_name, json_object* jobj, ExchangeFutureContract* fc)
{
  json_object *val;

  if(!json_object_object_get_ex(jobj, "expiration_timestamp", &val)) {
    fprintf(stderr,"%s: Error: Returned future contract does not have an 'expiration_timestamp' entry!\n",__func__);
    return -1;
  }
  fc->SetExpirationTimestamp(json_object_get_int64(val));

  ExchangeInstrumentSet& eis = em->AddInstrumentSet(fc->GetExpirationTimestamp());

  eis.SetFutureContract(fc);

  if(!json_object_object_get_ex(jobj, "creation_timestamp", &val)) {
    fprintf(stderr,"%s: Error: Returned future contract does not have a 'creation_timestamp' entry!\n",__func__);
    return -2;
  }
  fc->creation_timestamp=json_object_get_int64(val);

  if(!json_object_object_get_ex(jobj, "contract_size", &val)) {
    fprintf(stderr,"%s: Error: Returned future contract does not have a 'contract_size' entry!\n",__func__);
    return -3;
  }
  fc->contract_size=json_object_get_int(val);

  if(!json_object_object_get_ex(jobj, "is_active", &val)) {
    fprintf(stderr,"%s: Error: Returned future contract does not have an 'is_active' entry!\n",__func__);
    return -4;
  }
  fc->is_active=json_object_get_int(val);

  if(!json_object_object_get_ex(jobj, "max_leverage", &val)) {
    fprintf(stderr,"%s: Error: Returned future contract does not have a 'max_leverage' entry!\n",__func__);
    return -5;
  }
  fc->max_leverage=json_object_get_int(val);

  if(!json_object_object_get_ex(jobj, "tick_size", &val)) {
    fprintf(stderr,"%s: Error: Returned future contract does not have a 'tick_size' entry!\n",__func__);
    return -6;
  }
  fc->tick_size=json_object_get_double(val);

  if(!json_object_object_get_ex(jobj, "maker_commission", &val)) {
    fprintf(stderr,"%s: Error: Returned future contract does not have a 'maker_commission' entry!\n",__func__);
    return -7;
  }
  fc->maker_commission=json_object_get_double(val);

  if(!json_object_object_get_ex(jobj, "taker_commission", &val)) {
    fprintf(stderr,"%s: Error: Returned future contract does not have a 'taker_commission' entry!\n",__func__);
    return -8;
  }
  fc->taker_commission=json_object_get_double(val);

  if(!json_object_object_get_ex(jobj, "block_trade_commission", &val)) {
    fprintf(stderr,"%s: Error: Returned future contract does not have a 'block_trade_commission' entry!\n",__func__);
    return -9;
  }
  fc->block_trade_commission=json_object_get_double(val);

  if(!json_object_object_get_ex(jobj, "min_trade_amount", &val)) {
    fprintf(stderr,"%s: Error: Returned future contract does not have a 'min_trade_amount' entry!\n",__func__);
    return -10;
  }
  fc->min_trade_amount=json_object_get_double(val);
  return 0;
}

int DeribitExchange::LoadNewOptionContractData(ExchangeInstrumentSetExpiryMap* em, const char* instr_name, json_object* jobj, ExchangeOptionContract* oc)
{
  json_object *val;

  if(!json_object_object_get_ex(jobj, "expiration_timestamp", &val)) {
    fprintf(stderr,"%s: Error: Returned option contract does not have an 'expiration_timestamp' entry!\n",__func__);
    return -1;
  }
  oc->SetExpirationTimestamp(json_object_get_int64(val));

  ExchangeInstrumentSet& eis = em->AddInstrumentSet(oc->GetExpirationTimestamp());

  ExchangeOptionContractPairSet& ocps = eis.AddOptionContractPairSet();

  if(!json_object_object_get_ex(jobj, "strike", &val)) {
    fprintf(stderr,"%s: Error: Returned option contract does not have a 'strike' entry!\n",__func__);
    return -2;
  }
  oc->strike=json_object_get_double(val);

  ExchangeOptionContractPair& ocp = ocps.AddExchangeOptionContractPair(oc->strike);

  if(!json_object_object_get_ex(jobj, "option_type", &val)) {
    fprintf(stderr,"%s: Error: Returned option contract does not have an 'option_type' entry!\n",__func__);
    return -3;
  }
  const char* type=json_object_get_string(val);

  if(!strdiffer(type,"call")) {
    oc->call_option=true;
    ocp.SetCall(&eis, oc);

  } else if(!strdiffer(type,"put")) {
    oc->call_option=false;
    ocp.SetPut(&eis, oc);

  } else {
    fprintf(stderr,"%s: Error: Returned option contract has an invalid '%s' type!\n",__func__,type);
    return -4;
  }

  if(!json_object_object_get_ex(jobj, "creation_timestamp", &val)) {
    fprintf(stderr,"%s: Error: Returned option contract does not have a 'creation_timestamp' entry!\n",__func__);
    return -5;
  }
  oc->creation_timestamp=json_object_get_int64(val);

  if(!json_object_object_get_ex(jobj, "contract_size", &val)) {
    fprintf(stderr,"%s: Error: Returned option contract does not have a 'contract_size' entry!\n",__func__);
    return -6;
  }
  oc->contract_size=json_object_get_int(val);

  if(!json_object_object_get_ex(jobj, "is_active", &val)) {
    fprintf(stderr,"%s: Error: Returned option contract does not have an 'is_active' entry!\n",__func__);
    return -7;
  }
  oc->is_active=json_object_get_int(val);

  if(!json_object_object_get_ex(jobj, "tick_size", &val)) {
    fprintf(stderr,"%s: Error: Returned option contract does not have a 'tick_size' entry!\n",__func__);
    return -8;
  }
  oc->tick_size=json_object_get_double(val);

  if(!json_object_object_get_ex(jobj, "maker_commission", &val)) {
    fprintf(stderr,"%s: Error: Returned option contract does not have a 'maker_commission' entry!\n",__func__);
    return -9;
  }
  oc->maker_commission=json_object_get_double(val);

  if(!json_object_object_get_ex(jobj, "taker_commission", &val)) {
    fprintf(stderr,"%s: Error: Returned option contract does not have a 'taker_commission' entry!\n",__func__);
    return -10;
  }
  oc->taker_commission=json_object_get_double(val);

  if(!json_object_object_get_ex(jobj, "block_trade_commission", &val)) {
    fprintf(stderr,"%s: Error: Returned option contract does not have a 'block_trade_commission' entry!\n",__func__);
    return -11;
  }
  oc->block_trade_commission=json_object_get_double(val);

  if(!json_object_object_get_ex(jobj, "min_trade_amount", &val)) {
    fprintf(stderr,"%s: Error: Returned option contract does not have a 'min_trade_amount' entry!\n",__func__);
    return -12;
  }
  oc->min_trade_amount=json_object_get_double(val);
  return 0;
}

int DeribitExchange::UpdateInstruments()
{
  char curstr[64];
  char cmdopts[128];
  json_object *jobj, *val, *val2;
  int nentries;
  int i, j, k;
  int nnfcs, nnocs;
  const char* instr_name;
  const char *base_cur, *quote_cur;
  const char* kind;

  ExchangeInstrumentSetExpiryMap::instrument_set_const_iterator exp_it;
  ExchangeOptionContractPairSet* op_set;
  ExchangeOptionContractPairSet::const_iterator strike_it;

  for(ExchangeUnderlyingAssetSubset::const_iterator it=fUnderlyingAssetSubset->begin(); it!=fUnderlyingAssetSubset->end(); ++it) {
    
    if(it->second == &fUnderlyingAssetSubset->GetDefaultCurrency()) continue;
    printf("Currency '%s'\n",it->first.c_str());
    sprintf(curstr,"currency=%s",it->first.c_str());
    ExchangeInstrumentSetExpiryMap& expiry_map=fInstruments.AddInstrumentCurrency(*it->second);
    std::unordered_set<std::string> retinstruments;
    std::unordered_map<std::string, ExchangeFutureContract*> newfuturecontracts;
    std::unordered_map<std::string, ExchangeOptionContract*> newoptioncontracts;
    uint64_t reqtime;

    if(!fEP->Request(der_get_index, curstr)) {

      if((jobj=fEP->GetResult(&reqtime))) {

	  if(json_object_object_get_ex(jobj, it->first.c_str(), &val)) {
	    it->second->request_time = reqtime;
	    it->second->value = json_object_get_double(val);

	  } else {
	    fprintf(stderr,"%s: Error: Returned index price does not contain a price!\n",__func__);
	    return -3;
	  }

      } else return -2;

    } else return -1;

    if(!fEP->Request(der_get_book_summary_by_currency, curstr)) {

      if((jobj=fEP->GetResult(&expiry_map.InstrumentSetRequestTime()))) {

	if(json_object_get_type(jobj)!=json_type_array) {
	  fprintf(stderr,"%s: Error: Returned book summary does not contain an array of instruments!\n",__func__);
	  return -2;
	}
	nentries=json_object_array_length(jobj);

	//Loop over the returned instruments
	for(i=0; i<nentries; ++i) {
	  val=json_object_array_get_idx(jobj,i);

	  if(json_object_object_get_ex(val, "instrument_name", &val2)) {
	    instr_name=json_object_get_string(val2);
	    retinstruments.insert(instr_name);

	    //If the instruments is an option contract
	    if(json_object_object_get_ex(val, "interest_rate", &val2)) {
	      ExchangeOptionContract*& option_contract = fInstruments.GetOptionContractPtrRef(instr_name);

	      if(!option_contract) {
		option_contract = new ExchangeOptionContract(instr_name);
		newoptioncontracts[instr_name] = option_contract;
	      }

	      //option_contract->interest_rate=json_object_get_double(val2);

	      if(json_object_object_get_ex(val, "volume", &val2)) {
		option_contract->volume=json_object_get_double(val2);

	      } else {
		fprintf(stderr,"%s: Error: Returned option contract book summary array element does not have a 'volume' entry!\n",__func__);
		return -4;
	      }

	      if(json_object_object_get_ex(val, "underlying_price", &val2)) {
		option_contract->underlying_price=json_object_get_double(val2);

	      } else {
		fprintf(stderr,"%s: Error: Returned option contract book summary array element does not have an 'underlying_price' entry!\n",__func__);
		return -5;
	      }

	      if(json_object_object_get_ex(val, "underlying_index", &val2)) {
		option_contract->underlying_index=json_object_get_string(val2);

	      } else {
		fprintf(stderr,"%s: Error: Returned option contract book summary array element does not have an 'underlying_index' entry!\n",__func__);
		return -6;
	      }

	      if(json_object_object_get_ex(val, "quote_currency", &val2)) {
		quote_cur=json_object_get_string(val2);

	      } else {
		fprintf(stderr,"%s: Error: Returned option contract book summary array element does not have a 'quote_currency' entry!\n",__func__);
		return -7;
	      }

	      option_contract->quote_currency=&(*fUnderlyingAssetSubset)[quote_cur];

	      if(json_object_object_get_ex(val, "base_currency", &val2)) {
		base_cur=json_object_get_string(val2);

	      } else {
		fprintf(stderr,"%s: Error: Returned option contract book summary array element does not have a 'base_currency' entry!\n",__func__);
		return -8;
	      }

	      if(strdiffer(base_cur, it->first.c_str())) {
		fprintf(stderr,"%s: Error: Returned option contract book summary array element 'base_currency' entry is '%s' instrad of '%s'!\n",__func__,base_cur, it->first.c_str());
		return -8;
	      }

	      option_contract->base_currency=&(*fUnderlyingAssetSubset)[base_cur];

	      if(json_object_object_get_ex(val, "price_change", &val2)) {
		option_contract->percent_price_change_24h=json_object_get_double(val2);

	      } else {
		fprintf(stderr,"%s: Error: Returned option contract book summary array element does not have a 'price_change' entry!\n",__func__);
		return -9;
	      }

	      if(json_object_object_get_ex(val, "mid_price", &val2)) {
		option_contract->mid_price = json_object_get_double(val2);

	      } else {
		fprintf(stderr,"%s: Error: Returned option contract book summary array element does not have a 'mid_price' entry!\n",__func__);
		return -10;
	      }

	      if(json_object_object_get_ex(val, "mark_price", &val2)) {
		option_contract->mark_price = json_object_get_double(val2);

	      } else {
		fprintf(stderr,"%s: Error: Returned option contract book summary array element does not have a 'mark_price' entry!\n",__func__);
		return -11;
	      }

	      if(json_object_object_get_ex(val, "low", &val2)) {
		option_contract->low_price_24h = json_object_get_double(val2);

	      } else {
		fprintf(stderr,"%s: Error: Returned option contract book summary array element does not have a 'low' entry!\n",__func__);
		return -12;
	      }

	      if(json_object_object_get_ex(val, "last", &val2)) {
		option_contract->last_price = json_object_get_double(val2);

	      } else {
		fprintf(stderr,"%s: Error: Returned option contract book summary array element does not have a 'last' entry!\n",__func__);
		return -13;
	      }

	      if(json_object_object_get_ex(val, "high", &val2)) {
		option_contract->high_price_24h = json_object_get_double(val2);

	      } else {
		fprintf(stderr,"%s: Error: Returned option contract book summary array element does not have a 'high' entry!\n",__func__);
		return -15;
	      }

	      if(json_object_object_get_ex(val, "estimated_delivery_price", &val2)) {
		option_contract->estimated_delivery_price = json_object_get_double(val2);

	      } else {
		fprintf(stderr,"%s: Error: Returned option contract book summary array element does not have a 'estimated_delivery_price' entry!\n",__func__);
		return -16;
	      }

	      if(json_object_object_get_ex(val, "bid_price", &val2)) {
		option_contract->bid_price = json_object_get_double(val2);

	      } else {
		fprintf(stderr,"%s: Error: Returned option contract book summary array element does not have a 'bid_price' entry!\n",__func__);
		return -17;
	      }

	      if(json_object_object_get_ex(val, "ask_price", &val2)) {
		option_contract->ask_price = json_object_get_double(val2);

	      } else {
		fprintf(stderr,"%s: Error: Returned option contract book summary array element does not have an 'ask_price' entry!\n",__func__);
		return -18;
	      }

	    } else { //Else the instrument is a future contract
	      ExchangeFutureContractBase*& future_contract_base = fInstruments.GetFutureContractPtrRef(instr_name);
	      ExchangeFutureContract* future_contract = dynamic_cast<ExchangeFutureContract*>(future_contract_base);

	      if(future_contract_base && !future_contract) {
		fprintf(stderr,"%s: Error: New returned future contract book summary array element is trying to overwrite a synthetic future contract!\n",__func__);
		return -4;
	      }

	      if(!future_contract) {
		future_contract_base = future_contract = new ExchangeFutureContract(instr_name);
		newfuturecontracts[instr_name] = future_contract;
	      }

	      if(json_object_object_get_ex(val, "volume", &val2)) {
		future_contract->volume=json_object_get_double(val2);

	      } else {
		fprintf(stderr,"%s: Error: Returned future contract book summary array element does not have a 'volume' entry!\n",__func__);
		return -4;
	      }

	      if(json_object_object_get_ex(val, "quote_currency", &val2)) {
		quote_cur=json_object_get_string(val2);

	      } else {
		fprintf(stderr,"%s: Error: Returned future contract book summary array element does not have a 'quote_currency' entry!\n",__func__);
		return -7;
	      }

	      future_contract->quote_currency=&(*fUnderlyingAssetSubset)[quote_cur];

	      if(json_object_object_get_ex(val, "base_currency", &val2)) {
		base_cur=json_object_get_string(val2);

	      } else {
		fprintf(stderr,"%s: Error: Returned future contract book summary array element does not have a 'base_currency' entry!\n",__func__);
		return -8;
	      }

	      if(strdiffer(base_cur, it->first.c_str())) {
		fprintf(stderr,"%s: Error: Returned future contract book summary array element 'base_currency' entry is '%s' instrad of '%s'!\n",__func__,base_cur, it->first.c_str());
		return -8;
	      }

	      future_contract->base_currency=&(*fUnderlyingAssetSubset)[base_cur];

	      if(json_object_object_get_ex(val, "price_change", &val2)) {
		future_contract->percent_price_change_24h=json_object_get_double(val2);

	      } else {
		fprintf(stderr,"%s: Error: Returned future contract book summary array element does not have a 'price_change' entry!\n",__func__);
		return -9;
	      }

	      if(json_object_object_get_ex(val, "mid_price", &val2)) {
		future_contract->mid_price = json_object_get_double(val2);

	      } else {
		fprintf(stderr,"%s: Error: Returned future contract book summary array element does not have a 'mid_price' entry!\n",__func__);
		return -10;
	      }

	      if(json_object_object_get_ex(val, "mark_price", &val2)) {
		future_contract->mark_price = json_object_get_double(val2);

	      } else {
		fprintf(stderr,"%s: Error: Returned future contract book summary array element does not have a 'mark_price' entry!\n",__func__);
		return -11;
	      }

	      if(json_object_object_get_ex(val, "low", &val2)) {
		future_contract->low_price_24h = json_object_get_double(val2);

	      } else {
		fprintf(stderr,"%s: Error: Returned future contract book summary array element does not have a 'low' entry!\n",__func__);
		return -12;
	      }

	      if(json_object_object_get_ex(val, "last", &val2)) {
		future_contract->last_price = json_object_get_double(val2);

	      } else {
		fprintf(stderr,"%s: Error: Returned future contract book summary array element does not have a 'last' entry!\n",__func__);
		return -13;
	      }

	      if(json_object_object_get_ex(val, "high", &val2)) {
		future_contract->high_price_24h = json_object_get_double(val2);

	      } else {
		fprintf(stderr,"%s: Error: Returned future contract book summary array element does not have a 'high' entry!\n",__func__);
		return -15;
	      }

	      if(json_object_object_get_ex(val, "estimated_delivery_price", &val2)) {
		future_contract->estimated_delivery_price = json_object_get_double(val2);

	      } else {
		fprintf(stderr,"%s: Error: Returned future contract book summary array element does not have a 'estimated_delivery_price' entry!\n",__func__);
		return -16;
	      }

	      if(json_object_object_get_ex(val, "bid_price", &val2)) {
		future_contract->bid_price = json_object_get_double(val2);

	      } else {
		fprintf(stderr,"%s: Error: Returned future contract book summary array element does not have a 'bid_price' entry!\n",__func__);
		return -17;
	      }

	      if(json_object_object_get_ex(val, "ask_price", &val2)) {
		future_contract->ask_price = json_object_get_double(val2);

	      } else {
		fprintf(stderr,"%s: Error: Returned future contract book summary array element does not have an 'ask_price' entry!\n",__func__);
		return -18;
	      }
	    }

	  } else {
	    fprintf(stderr,"%s: Error: Returned book summary array element does not have an 'instrument_name' entry!\n",__func__);
	    return -3;
	  }
	}

      } else return -2;

    } else return -1;

    nnfcs = newfuturecontracts.size();
    nnocs = newoptioncontracts.size();

    //Loop over the previously known instruments and delete unused ones
    for(exp_it = expiry_map.instrument_set_begin(); exp_it != expiry_map.instrument_set_end();) {
      j=0;

      //Here we just delete real future contracts
      if(dynamic_cast<ExchangeFutureContract*>(exp_it->second->GetFutureContract())) {

	if(retinstruments.find(exp_it->second->GetFutureContract()->instrument_name) == retinstruments.end()) exp_it->second->DeleteFutureContract();
	else ++j;
      }

      if((op_set=exp_it->second->GetOptionContractPairSet())) {

	//Loop over previously known strike prices
	for(strike_it = op_set->begin(); strike_it != op_set->end();) {
	  k=0;

	  if(strike_it->second->GetCall()) {

	    if(retinstruments.find(strike_it->second->GetCall()->instrument_name) == retinstruments.end()) strike_it->second->DeleteCall();
	    else ++k;
	  }

	  if(strike_it->second->GetPut()) {

	    if(retinstruments.find(strike_it->second->GetPut()->instrument_name) == retinstruments.end()) strike_it->second->DeletePut();
	    else ++k;
	  }

	  if(k) ++strike_it;

	  else op_set->DeleteExchangeOptionContractPair(strike_it++);
	}

	//If there is no option contract strike price remaining
	if(!op_set->size()) {
	  exp_it->second->DeleteOptionContractPairSet();

	  //If the future contract was synthetic, delete it
	  if(!dynamic_cast<ExchangeFutureContract*>(exp_it->second->GetFutureContract())) exp_it->second->DeleteFutureContract();

	} else ++j;
      }

      if(j) ++exp_it;

      else expiry_map.DeleteInstrumentSet(exp_it++);
    }

    //If there is missing information for more than one future/option contract,
    //refresh data for all instruments
    if(nnfcs + nnocs > 1) {

      bool tried_expired=false;

      if(!nnfcs) sprintf(cmdopts, "%s&kind=option", curstr);

      else if(!nnocs) sprintf(cmdopts, "%s&kind=future", curstr);

      else sprintf(cmdopts, "%s", curstr);
get_instruments:

      if(!fEP->Request(der_get_instruments, cmdopts)) {

	if((jobj=fEP->GetResult())) {

	  if(json_object_get_type(jobj)!=json_type_array) {
	    fprintf(stderr,"%s: Error: Returned instruments does not contain an array of instruments!\n",__func__);
	    return -20;
	  }
	  nentries=json_object_array_length(jobj);

	  //Loop over the returned instruments
	  for(i=0; i<nentries; ++i) {
	    val=json_object_array_get_idx(jobj,i);

	    if(json_object_object_get_ex(val, "instrument_name", &val2)) {
	      instr_name = json_object_get_string(val2);

	    } else {
	      fprintf(stderr,"%s: Error: Returned instruments array element does not have an 'instrument_name' entry!\n",__func__);
	      return -23;
	    }

	    if(json_object_object_get_ex(val, "kind", &val2)) {
	      kind = json_object_get_string(val2);

	      if(!strdiffer(kind, "future")) {
		std::unordered_map<std::string, ExchangeFutureContract*>::const_iterator fc_it = newfuturecontracts.find(instr_name);

		//If the returned future instrument was amongst the list of new
		//future contracts returned by the book summary endpoint
		if(fc_it != newfuturecontracts.end()) {
		  printf("Loading future contract '%s'\n",instr_name);

		  if(!LoadNewFutureContractData(&expiry_map, instr_name, val, fc_it->second)) --nnfcs;

		  else {
		    fprintf(stderr,"%s: Error: Could not successfully load new future contract data for instrument '%s'!\n",__func__,instr_name);
		    return -24;
		  }
		}

	      } else if(!strdiffer(kind, "option")) {
		std::unordered_map<std::string, ExchangeOptionContract*>::const_iterator oc_it = newoptioncontracts.find(instr_name);

		//If the returned option instrument was amongst the list of new
		//future contracts returned by the book summary endpoint
		if(oc_it != newoptioncontracts.end()) {
		  printf("Loading option contract '%s'\n",instr_name);

		  if(!LoadNewOptionContractData(&expiry_map, instr_name, val, oc_it->second)) --nnocs;

		  else {
		    fprintf(stderr,"%s: Error: Could not successfully load new option contract data for instrument '%s'!\n",__func__,instr_name);
		    return -25;
		  }
		}
	      }

	    } else {
	      fprintf(stderr,"%s: Error: Returned instruments array element does not have a 'kind entry!\n",__func__);
	      return -22;
	    }
	  }

	} else return -21;

      } else return -20;

      if(nnfcs || nnocs) {

	if(!tried_expired) {
	  tried_expired=true;

	  if(!nnfcs) sprintf(cmdopts, "%s&kind=option&expired=true", curstr);

	  else if(!nnocs) sprintf(cmdopts, "%s&kind=future&expired=true", curstr);

	  else sprintf(cmdopts, "%s&expired=true", curstr);

	  goto get_instruments;

	} else {
	  fprintf(stderr,"%s: Error: Could not successfully find all new future and option contracts through returned instruments endpoint!\n",__func__);
	  return -26;
	}
      }

    } else if(nnfcs == 1) {
      std::unordered_map<std::string, ExchangeFutureContract*>::const_iterator fc_it = newfuturecontracts.begin();

      sprintf(cmdopts, "instrument_name=%s", fc_it->first.c_str());

      if(!fEP->Request(der_get_instrument, cmdopts)) {

	if((jobj=fEP->GetResult())) {

	  if(json_object_object_get_ex(jobj, "instrument_name", &val2)) {
	    instr_name = json_object_get_string(val2);

	  } else {
	    fprintf(stderr,"%s: Error: Returned instrument does not have an 'instrument_name' entry!\n",__func__);
	    return -29;
	  }


	  if(!strdiffer(instr_name, fc_it->first.c_str())) {

	    if(!LoadNewFutureContractData(&expiry_map, instr_name, jobj, fc_it->second)) --nnfcs;

	    else {
	      fprintf(stderr,"%s: Error: Could not successfully load new future contract data for instrument '%s'!\n",__func__,instr_name);
	      return -31;
	    }

	  } else {
	    fprintf(stderr,"%s: Error: Returned instrument '%s' does not have the expected instrument name ('%s')!\n",__func__,instr_name,fc_it->first.c_str());
	    return -30;
	  }

	} else return -28;

      } else return -27;

    } else if(nnocs == 1) {
      std::unordered_map<std::string, ExchangeOptionContract*>::const_iterator oc_it = newoptioncontracts.begin();

      sprintf(cmdopts, "instrument_name=%s", oc_it->first.c_str());

      if(!fEP->Request(der_get_instrument, cmdopts)) {

	if((jobj=fEP->GetResult())) {

	  if(json_object_object_get_ex(jobj, "instrument_name", &val2)) {
	    instr_name = json_object_get_string(val2);

	  } else {
	    fprintf(stderr,"%s: Error: Returned instrument does not have an 'instrument_name' entry!\n",__func__);
	    return -34;
	  }


	  if(!strdiffer(instr_name, oc_it->first.c_str())) {

	    if(!LoadNewOptionContractData(&expiry_map, instr_name, jobj, oc_it->second)) --nnocs;

	    else {
	      fprintf(stderr,"%s: Error: Could not successfully load new option contract data for instrument '%s'!\n",__func__,instr_name);
	      return -36;
	    }

	  } else {
	    fprintf(stderr,"%s: Error: Returned instrument '%s' does not have the expected instrument name ('%s')!\n",__func__,instr_name,oc_it->first.c_str());
	    return -35;
	  }

	} else return -33;

      } else return -32;
    }

    if(expiry_map.UpdateFutureContractYields()) {
      fprintf(stderr,"%s: Error: With currency '%s'\n",__func__,it->first.c_str());
      return -37;
    }
  }
  return 0;
}
