#include "DeribitEndpoint.h"
#include "conv_utils.h"

extern "C" {
#include "json_print.h"
};

DeribitEndpoint::DeribitEndpoint(const dertype& dtype, const char* configfile): fDerType(dtype), fCHandle(curl_easy_init()), fHeaders(NULL), fJSTok(json_tokener_new()), fJObj(NULL), fKey(NULL), fCode(NULL), fKeyLength(0), fCodeLength(0), fSigBuf(), fNeedCleanup(false)
{
  int fid=open(configfile,0);

  if(fid<0) {
    perror(__func__);
    throw 0;
  }

  long flength=lseek(fid, 0,  SEEK_END);
  lseek(fid, 0,  SEEK_SET);

  char* fbuf=(char*)malloc(flength);

  if(read(fid, fbuf, flength)<0) {
    perror(__func__);
    throw 0;
  }
  close(fid);

  json_tokener_reset(fJSTok);
  json_object* jobj=json_tokener_parse_ex(fJSTok, fbuf, flength);
  free(fbuf);
  enum json_tokener_error jerr=json_tokener_get_error(fJSTok);

  if(!jobj || jerr!=json_tokener_success) {

    if(jerr==json_tokener_continue) fprintf(stderr,"%s: Incomplete JSON reply\n",__func__);

    else fprintf(stderr,"%s: Error parsing JSON reply: %s\n",__func__,json_tokener_error_desc(jerr));
    
    if(jobj) json_object_put(jobj);
    json_tokener_reset(fJSTok);
    throw 0;

  } else {
    json_object *val;

    if(json_object_object_get_ex(jobj, "apiKey", &val)) {
      fKey=(unsigned char*)strdup(json_object_get_string(val));
      fKeyLength=strlen((char*)fKey);

    } else {
      fprintf(stderr,"%s: Error: Could not read apiKey\n",__func__);
      json_object_put(jobj);
      json_tokener_reset(fJSTok);
      throw 0;
    }

    if(json_object_object_get_ex(jobj, "secretKey", &val)) {
      fCode=(unsigned char*)strdup(json_object_get_string(val));
      fCodeLength=strlen((char*)fCode);

    } else {
      fprintf(stderr,"%s: Error: Could not read secretKey\n",__func__);
      json_object_put(jobj);
      json_tokener_reset(fJSTok);
      throw 0;
    }
    json_object_put(jobj);
    json_tokener_reset(fJSTok);
  }

  curl_easy_setopt(fCHandle,CURLOPT_NOSIGNAL,1);
  curl_easy_setopt(fCHandle, CURLOPT_WRITEFUNCTION, CurlCB);
  curl_easy_setopt(fCHandle, CURLOPT_WRITEDATA, this);
  //curl_easy_setopt(fCHandle, CURLOPT_VERBOSE, 1L);
  //curl_easy_setopt(fCHandle, CURLOPT_DEBUGFUNCTION, debug_callback);
}

int DeribitEndpoint::debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr)
{
  printf("Data: %.*s\n",(int)size,data);
  return size;
}

size_t DeribitEndpoint::CurlCB(char *ptr, size_t size, size_t nmemb, void *instance)
{
  const size_t nbytes=size*nmemb;
  DeribitEndpoint& bep=*(DeribitEndpoint*)instance;

  //printf("%.*s\n",(int)nbytes,ptr);
  bep.fJObj=json_tokener_parse_ex(bep.fJSTok, ptr, nbytes);
  enum json_tokener_error jerr=json_tokener_get_error(bep.fJSTok);

  if(!bep.fJObj && jerr!=json_tokener_continue) {
    fprintf(stderr,"%s: Error parsing JSON reply: %s\n",__func__,json_tokener_error_desc(jerr));
    json_tokener_reset(bep.fJSTok);
    return 0;

  }
  return nbytes;
}

uint64_t DeribitEndpoint::GetServerTime()
{
  uint64_t ret=0;

  if(!Request(der_get_time, derempty, false)) {
    json_object *val;

    if(json_object_object_get_ex(fJObj, "result", &val)) ret=(uint64_t)json_object_get_int64(val);
  }
  return ret;
}

int DeribitEndpoint::PingServer()
{
  uint64_t mstime=getmstime();
  if(!Request(der_test, derempty, false)) {
    return getmstime()-mstime;
  }
  return -1;
}

int DeribitEndpoint::Request(const derep& ep, const std::string& args, const int& sign)
{
  if(fNeedCleanup) {
    json_object_put(fJObj);
    json_tokener_reset(fJSTok);
    fNeedCleanup=false;
  }
  char* url=(char*)malloc(fDerType.ep.size()+ep.cmd.size()+args.size()+1);
  memcpy(url, fDerType.ep.c_str(), fDerType.ep.size());
  size_t urllength=fDerType.ep.size();
  memcpy(url+urllength, ep.cmd.c_str(), ep.cmd.size());
  urllength+=ep.cmd.size();
  char* buf;

  if(sign==derepsign_true && fCodeLength && fKeyLength) {
    uint64_t mstime=getmstime();
    int len;

    buf=(char*)malloc(40+DERIBIT_SUBDIR_LEN+ep.cmd.size()+args.size());
    const char* method;

    switch(ep.type) {

      case bieneptype_get:
	method="GET";
	break;

      case bieneptype_post:
	method="POST";
	break;

      case bieneptype_put:
	method="PUT";
	break;

      case bieneptype_delete:
	method="DELETE";
    }

    uint64_t lrbuf;
    RAND_bytes((unsigned char*)&lrbuf, 6);
    char nounce[8]={_cu_base64[lrbuf&63], _cu_base64[(lrbuf>>6)&63], _cu_base64[(lrbuf>>12)&63], _cu_base64[(lrbuf>>18)&63], _cu_base64[(lrbuf>>24)&63], _cu_base64[(lrbuf>>30)&63], _cu_base64[(lrbuf>>36)&63], _cu_base64[(lrbuf>>42)&63]};
    len=sprintf(buf,"%" PRIu64 "\n%.8s\n%s\n" DERIBIT_SUBDIR "%.*s%.*s\n\n", mstime, nounce, method, (int)ep.cmd.size(), ep.cmd.c_str(), (int)args.size(), args.c_str());

    printf("String to be signed: '%.*s'\n",len,buf);
    unsigned int len2=EVP_MAX_MD_SIZE;
    mx_hmac_sha256(fCode,fCodeLength,buf,len,fSigBuf,&len2);

    buf=(char*)realloc(buf, 81+fKeyLength+2*EVP_MAX_MD_SIZE);
    len=sprintf(buf,"Authorization: deri-hmac-sha256 id=%.*s,ts=%" PRIu64 ",nonce=%.8s,sig=", fKeyLength, fKey, mstime, nounce);

    for(unsigned int i=0; i<len2; ++i) {
      //printf("%02x",fSigBuf[i]);
      buf[len+2*i]=uint4toasciihex(fSigBuf[i]>>4);
      buf[len+2*i+1]=uint4toasciihex(fSigBuf[i] & 0xF);
    }
    buf[len+2*len2]=0;
    //printf("\n");
    printf("Full is '%s'\n",buf);
    fHeaders=curl_slist_append(NULL,buf);
    free(buf);
    curl_easy_setopt(fCHandle, CURLOPT_HTTPHEADER, fHeaders);

  } else curl_easy_setopt(fCHandle, CURLOPT_HTTPHEADER, NULL);

  switch(ep.type) {

    case bieneptype_get:

      if(args.size()) {
	memcpy(url+urllength,args.c_str(),args.size());
	url[urllength+args.size()]=0;

      } else url[urllength]=0;
      printf("URL is '%s'\n",url);
      curl_easy_setopt(fCHandle, CURLOPT_URL, url); 
      free(url);

      if(curl_easy_perform(fCHandle)) {
	fprintf(stderr,"curl_easy_perform: An error was returned!\n");
	return -1;
      }
      break;

    case bieneptype_post:
      url[urllength]=0;

      if(args.size()) {
	curl_easy_setopt(fCHandle, CURLOPT_POSTFIELDS, args.c_str());
	curl_easy_setopt(fCHandle, CURLOPT_POSTFIELDSIZE, args.size());

      } else {
	curl_easy_setopt(fCHandle, CURLOPT_POST,1L);
	curl_easy_setopt(fCHandle, CURLOPT_POSTFIELDSIZE, 0);
      }
      printf("URL is '%s'\n",url);
      curl_easy_setopt(fCHandle, CURLOPT_URL, url); 
      free(url);

      if(curl_easy_perform(fCHandle)) {
	fprintf(stderr,"curl_easy_perform: An error was returned!\n");
	curl_easy_setopt(fCHandle, CURLOPT_POST,0);
	return -1;
      }
      curl_easy_setopt(fCHandle, CURLOPT_POST,0);
      break;

    case bieneptype_put:

      if(args.size()) {
	memcpy(url+urllength,args.c_str(),args.size());
	url[urllength+args.size()]=0;

      } else url[urllength]=0;
      curl_easy_setopt(fCHandle, CURLOPT_UPLOAD, 1L);
      curl_easy_setopt(fCHandle, CURLOPT_INFILESIZE, 0);
      printf("URL is '%s'\n",url);
      curl_easy_setopt(fCHandle, CURLOPT_URL, url); 
      free(url);

      if(curl_easy_perform(fCHandle)) {
	fprintf(stderr,"curl_easy_perform: An error was returned!\n");
        curl_easy_setopt(fCHandle, CURLOPT_UPLOAD, 0L);
	return -1;
      }
      curl_easy_setopt(fCHandle, CURLOPT_UPLOAD, 0L);

      break;

    case bieneptype_delete:

      if(args.size()) {
	memcpy(url+urllength,args.c_str(),args.size());
	url[urllength+args.size()]=0;

      } else url[urllength]=0;
      curl_easy_setopt(fCHandle, CURLOPT_CUSTOMREQUEST, "DELETE");
      printf("URL is '%s'\n",url);
      curl_easy_setopt(fCHandle, CURLOPT_URL, url); 
      free(url);

      if(curl_easy_perform(fCHandle)) {
	fprintf(stderr,"curl_easy_perform: An error was returned!\n");
        curl_easy_setopt(fCHandle, CURLOPT_CUSTOMREQUEST, NULL);
	return -1;
      }
      curl_easy_setopt(fCHandle, CURLOPT_CUSTOMREQUEST, NULL);
  }
  fNeedCleanup=true;
  json_print(fJObj);
  return 0;
}

json_object* DeribitEndpoint::GetResult(uint64_t* usIn)
{
  json_object* val;

  if(usIn) {
    if(json_object_object_get_ex(fJObj, "usIn", &val)) *usIn = json_object_get_int64(val)/1000;

    else {
      fprintf(stderr, "%s: Error: Could not retried usIn parameter!\n",__func__);
      *usIn = 0;
    }
  }

  if(json_object_object_get_ex(fJObj, "result", &val)) {
    return val;

  } else if(json_object_object_get_ex(fJObj, "error", &val)) {
    json_object *val2;
    int code=-1;
    const char* message="";

    if(json_object_object_get_ex(val, "code", &val2)) code=json_object_get_int(val2);

    if(json_object_object_get_ex(val, "message", &val2)) message=json_object_get_string(val2);

    fprintf(stderr, "%s: Error %i: %s\n", __func__, code, message);
  }

  return NULL;
}
