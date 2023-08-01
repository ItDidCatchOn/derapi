#ifndef _NAMEDINDICES_
#define _NAMEDINDICES_

#include <unordered_map>
#include <string>

typedef uint16_t t_namedindex_id;

typedef struct s_namedindex_ {
  s_namedindex_(const char* symb, const t_namedindex_id& theid): symbol(symb), id(theid){}
  std::string symbol;
  t_namedindex_id id;
} s_namedindex;

class NamedIndices
{
  public:
    typedef std::unordered_map<std::string, s_namedindex*>::const_iterator const_iterator;
    typedef std::unordered_map<std::string, s_namedindex*>::size_type size_type;
    NamedIndices(): fNamedIndices() {}
    virtual ~NamedIndices(){Clear();}

    void Clear(){for(auto it = fNamedIndices.begin(); it != fNamedIndices.end(); ++it) delete(it->second);  fNamedIndices.clear();}

    inline s_namedindex const* GetNamedIndex(const char* symbol) const
    {
      const_iterator cur = fNamedIndices.find(symbol);

      if(cur == fNamedIndices.end()) return NULL;
      return cur->second;
    }

    inline s_namedindex const& operator[](const char* symbol)
    {
      s_namedindex*& c = fNamedIndices[symbol];

      if(!c)  c = new s_namedindex(symbol, fNamedIndices.size());
      return *c;
    }
    inline const_iterator begin() const{return fNamedIndices.begin();}
    inline const_iterator end() const{return fNamedIndices.end();}
    inline size_type size() const{return fNamedIndices.size();}

  protected:
    std::unordered_map<std::string, s_namedindex*> fNamedIndices;

  private:
    NamedIndices const& operator=(const NamedIndices&){return *this;}
};

#endif
