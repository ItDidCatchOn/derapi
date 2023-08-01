#ifndef _DERIBITBASE_
#define _DERIBITBASE_

#include <cstdint>
#include <sys/time.h>

#define DERIBIT_SUBDIR "/api/v2"
#define DERIBIT_SUBDIR_LEN 7

#define DERIBIT_TEST_BASEURI "https://test.deribit.com" DERIBIT_SUBDIR
#define DERIBIT_TEST_WS_BASEURI "wss://test.deribit.com/ws" DERIBIT_SUBDIR

#define DERIBIT_BASEURI "https://www.deribit.com" DERIBIT_SUBDIR
#define DERIBIT_WS_BASEURI "wss://www.deribit.com/ws" DERIBIT_SUBDIR

inline static uint64_t getmstime() {
  struct timeval tv;
  gettimeofday(&tv, NULL);

  return (uint64_t)(tv.tv_sec)*1000 + (uint64_t)(tv.tv_usec)/1000;
}

inline static const char* getctime(const uint64_t* timestamp) {
  time_t timep=(time_t)*timestamp/1000;
  return ctime(&timep);
}

struct dertype
{
  dertype(const uint8_t& id, const char* ep, const char* ws):id(id), ep(ep), ws(ws){}
  uint8_t id;
  std::string ep;
  std::string ws;
};

const std::string derempty="";

const dertype dernormal={0,DERIBIT_BASEURI,DERIBIT_WS_BASEURI}; 
const dertype dertest={1,DERIBIT_TEST_BASEURI,DERIBIT_TEST_WS_BASEURI}; 

inline static bool operator==(const dertype& lhs, const dertype& rhs){return (lhs.id==rhs.id);}

enum derepsign {derepsign_false=false, derepsign_true=true};

enum dereptype {bieneptype_get, bieneptype_post, bieneptype_put, bieneptype_delete};

struct derep
{
  std::string cmd;
  dereptype type;
};

const derep der_get_time={"/public/get_time?",bieneptype_get};
const derep der_test={"/public/test?",bieneptype_get};

const derep der_get_currencies={"/public/get_currencies?",bieneptype_get};
const derep der_get_index={"/public/get_index?",bieneptype_get};
const derep der_get_instrument={"/public/get_instrument?",bieneptype_get};
const derep der_get_instruments={"/public/get_instruments?",bieneptype_get};
const derep der_get_book_summary_by_currency={"/public/get_book_summary_by_currency?",bieneptype_get};

const derep der_get_deposits={"/private/get_deposits?",bieneptype_get};

const derep deraccount={"account?",bieneptype_get};
const derep derposition={"positionRisk?",bieneptype_get};
const derep dercommissionrate={"commissionRate?",bieneptype_get};
const derep derexchangeinfo={"exchangeInfo",bieneptype_get};
const derep derpositionrisk={"positionRisk?",bieneptype_get};
const derep deropenorders={"openOrders?",bieneptype_get};
const derep derorder={"order?",bieneptype_post};
const derep derfuturestransfer={"futures/transfer",bieneptype_post};
#endif
