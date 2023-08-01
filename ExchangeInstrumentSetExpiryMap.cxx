#include "ExchangeInstrumentSetExpiryMap.h"

#include <time.h>

#include "ExchangeFutureContract.h"
#include "ExchangeFutureContractSynthetic.h"
#include "ExchangeFutureContractFirstSynthetic.h"
#include "ExchangeFutureContractLastSynthetic.h"

int ExchangeInstrumentSetExpiryMap::UpdateFutureContractYields()
{
  instrument_set_const_iterator is_it, is_it_l, is_it_r, is_it_limit;
  ExchangeOptionContractPairSet const* ops;
  ExchangeOptionContractPair const* op;
  ExchangeFutureContractBase* fcb;
  ExchangeFutureContract *fc_l=NULL, *fc_r=NULL;

  //Start with the first expiration date and loop until there is one with a future contract
  for(is_it=instrument_set_begin(); is_it!=instrument_set_end(); ++is_it) {

    if(dynamic_cast<ExchangeFutureContract*>(is_it->second->GetFutureContract())) {
      is_it->second->GetFutureContract()->UpdateRate();
      is_it_r=is_it;
      break;
    }
    ops = is_it->second->GetOptionContractPairSet();

    if(ops) {
      op = ops->begin()->second;

      is_it_r = is_it;
      ++is_it_r;

      while(is_it_r!=instrument_set_end() && !(fc_r=dynamic_cast<ExchangeFutureContract*>(is_it_r->second->GetFutureContract()))) ++is_it_r;

      if(is_it_r!=instrument_set_end()) {
        is_it_r->second->GetFutureContract()->UpdateRate();

	if(op->GetCall()) fcb = new ExchangeFutureContractFirstSynthetic(op->GetCall()->underlying_index.c_str(), *fc_r);

	else fcb = new ExchangeFutureContractFirstSynthetic(op->GetPut()->underlying_index.c_str(), *fc_r);

	fcb->SetExpirationTimestamp(is_it->first);
	is_it->second->SetFutureContract(fcb);
	//printf("First synthetic contract '%s'\n",fcb->instrument_name.c_str());
	is_it->second->GetFutureContract()->UpdateRate();

	for(++is_it; is_it!=is_it_r; ++is_it) {
	  ops = is_it->second->GetOptionContractPairSet();

	  if(ops) {
	    op = ops->begin()->second;

	    if(op->GetCall()) fcb = new ExchangeFutureContractFirstSynthetic(op->GetCall()->underlying_index.c_str(), *fc_r);

	    else fcb = new ExchangeFutureContractFirstSynthetic(op->GetPut()->underlying_index.c_str(), *fc_r);

	    fcb->SetExpirationTimestamp(is_it->first);
	    is_it->second->SetFutureContract(fcb);
	    //printf("First synthetic contract '%s'\n",fcb->instrument_name.c_str());
	    is_it->second->GetFutureContract()->UpdateRate();
	  }
	}
	break;

      } else {
	time_t expiry=is_it->first/1000;
	fprintf(stderr, "%s: Error: Could neither find a future contract nor create a synthetic one for expiration time %s",__func__,ctime(&expiry));
	return -1;
      }
    }
  }

  if(is_it==instrument_set_end()) return 0;

  //Assertion: is_it==is_it_r and point to the first expiration date with a
  //future contract.

  //Start with the last expiration date and loop until there is one with a future contract
  is_it=instrument_set_end();

  for(--is_it; is_it!=is_it_r; --is_it) {

    if(dynamic_cast<ExchangeFutureContract*>(is_it->second->GetFutureContract())) {
      is_it->second->GetFutureContract()->UpdateRate();
      is_it_l=is_it;
      break;
    }
    ops = is_it->second->GetOptionContractPairSet();

    if(ops) {
      op = ops->begin()->second;

      is_it_l = is_it;
      --is_it_l;

      while(is_it_l!=instrument_set_begin() && !(fc_l=dynamic_cast<ExchangeFutureContract*>(is_it_l->second->GetFutureContract()))) --is_it_l;

      //is_it_l==instrument_set_begin() was excluded
      if(is_it_l!=instrument_set_begin() || (fc_l=dynamic_cast<ExchangeFutureContract*>(is_it_l->second->GetFutureContract()))) {
        is_it_l->second->GetFutureContract()->UpdateRate();

	if(op->GetCall()) fcb = new ExchangeFutureContractLastSynthetic(op->GetCall()->underlying_index.c_str(), *fc_l);

	else fcb = new ExchangeFutureContractLastSynthetic(op->GetPut()->underlying_index.c_str(), *fc_l);

	fcb->SetExpirationTimestamp(is_it->first);
	is_it->second->SetFutureContract(fcb);
	//printf("Last synthetic contract '%s'\n",fcb->instrument_name.c_str());
	is_it->second->GetFutureContract()->UpdateRate();

	for(--is_it; is_it!=is_it_l; --is_it) {
	  ops = is_it->second->GetOptionContractPairSet();

	  if(ops) {
	    op = ops->begin()->second;

	    if(op->GetCall()) fcb = new ExchangeFutureContractLastSynthetic(op->GetCall()->underlying_index.c_str(), *fc_l);

	    else fcb = new ExchangeFutureContractLastSynthetic(op->GetPut()->underlying_index.c_str(), *fc_l);

	    fcb->SetExpirationTimestamp(is_it->first);
	    is_it->second->SetFutureContract(fcb);
	    //printf("Last synthetic contract '%s'\n",fcb->instrument_name.c_str());
	    is_it->second->GetFutureContract()->UpdateRate();
	  }
	}
	break;

      } else {
	time_t expiry=is_it->first/1000;
	fprintf(stderr, "%s: Error: Could neither find a future contract nor create a synthetic one for expiration time %s",__func__,ctime(&expiry));
	return -1;
      }
    }
  }

  if(is_it==is_it_r) return 0;

  //Assertion: is_it==is_it_l and point to the last expiration date with a
  //future contract.

  is_it_limit = is_it_r;
  fc_r=dynamic_cast<ExchangeFutureContract*>(is_it_r->second->GetFutureContract());
  --is_it_l;

  while(!(fc_l=dynamic_cast<ExchangeFutureContract*>(is_it_l->second->GetFutureContract()))) --is_it_l;
  is_it_l->second->GetFutureContract()->UpdateRate();

  //Loop over these expiry dates
  for(--is_it; is_it!=is_it_limit; --is_it) {

    if(dynamic_cast<ExchangeFutureContract*>(is_it->second->GetFutureContract())) {
      fc_r=(ExchangeFutureContract*)is_it->second->GetFutureContract();
      is_it_r = is_it;
      --is_it_l;

      while(!(fc_l=dynamic_cast<ExchangeFutureContract*>(is_it_l->second->GetFutureContract()))) --is_it_l;
      is_it_l->second->GetFutureContract()->UpdateRate();
      continue;
    }

    ops = is_it->second->GetOptionContractPairSet();

    //If there are option contracts but there is no future contract
    if(ops) {
      op = ops->begin()->second;

      if(op->GetCall()) fcb = new ExchangeFutureContractSynthetic(op->GetCall()->underlying_index.c_str(), *fc_l, *fc_r);

      else fcb = new ExchangeFutureContractSynthetic(op->GetPut()->underlying_index.c_str(), *fc_l, *fc_r);

      fcb->SetExpirationTimestamp(is_it->first);
      is_it->second->SetFutureContract(fcb);
      //printf("Synthetic contract '%s'\n",fcb->instrument_name.c_str());
      is_it->second->GetFutureContract()->UpdateRate();
    }
  }

  return 0;
}
