#ifndef PATTERNMATCH_H
#define PATTERNMATCH_H

#include <onnx/common/ir.h>
#include <string>
#include <type_traits>

namespace onnc {
namespace PatternMatch {

template <typename Val, typename Pattern> bool match(Val *pV, const Pattern &pP)
{
  if (pV == nullptr)
    return false;
  return const_cast<Pattern &>(pP).match(pV);
}

template <typename Val, typename Pattern, typename... Patterns>
bool match(Val *pV, const Pattern &pP, const Patterns &... pPs)
{
  return match(pV, pP) and match(pV, pPs...);
}

struct matchSymbol {
  onnx::Symbol m_Symbol;
  matchSymbol(const std::string &pSym) : m_Symbol(pSym) {}
  bool match(onnx::Node *pN) { return pN->kind() == m_Symbol; }
};
matchSymbol mSymbol(const std::string &pSym) { return pSym; }

onnx::Node *next(onnx::Node *pN)
{
  if (pN->outputs().size() != 1)
    return nullptr;
  if (pN->output()->uses().size() != 1)
    return nullptr;
  return pN->output()->uses()[0].user;
}

/// Matching combinators
template <typename LTy, typename RTy> struct match_combine_or {
  LTy m_L;
  RTy m_R;

  match_combine_or(const LTy &pLeft, const RTy &pRight)
      : m_L(pLeft), m_R(pRight)
  {
  }

  template <typename ITy> bool match(ITy *pV)
  {
    return m_L.match(pV) or m_R.match(pV);
  }
};

template <typename LTy, typename RTy> struct match_combine_and {
  LTy m_L;
  RTy m_R;

  match_combine_and(const LTy &pLeft, const RTy &pRight)
      : m_L(pLeft), m_R(pRight)
  {
  }

  template <typename ITy> bool match(ITy *pV)
  {
    return m_L.match(pV) and m_R.match(pV);
  }
};

/// Combine two pattern matchers matching L || R
template <typename LTy, typename RTy>
inline match_combine_or<LTy, RTy> m_CombineOr(const LTy &pL, const RTy &pR)
{
  return match_combine_or<LTy, RTy>(pL, pR);
}

/// Combine two pattern matchers matching L && R
template <typename LTy, typename RTy>
inline match_combine_and<LTy, RTy> m_CombineAnd(const LTy &pL, const RTy &pR)
{
  return match_combine_and<LTy, RTy>(pL, pR);
}

// Attribute
template <typename T> struct matchAttr {
  static_assert(sizeof(T) == -1, "error: unsupported type");
};
#define CREATE_ACCESSOR_BASE(Kind, method, Kind_T)                             \
  template <> struct matchAttr<Kind_T> {                                       \
    std::string m_Name;                                                        \
    onnx::Kind##Attr::ValueType m_Value;                                       \
    matchAttr(const std::string &pName, Kind_T pV)                             \
        : m_Name(pName), m_Value(pV)                                           \
    {                                                                          \
    }                                                                          \
    bool match(onnx::Node *pN) const                                           \
    {                                                                          \
      auto s = onnx::Symbol(m_Name);                                           \
      if (not pN->hasAttribute(s))                                             \
        return false;                                                          \
      if (pN->method(s) != m_Value)                                            \
        return false;                                                          \
      return true;                                                             \
    }                                                                          \
  };
#define CREATE_ACCESSOR(Kind, method)                                          \
  CREATE_ACCESSOR_BASE(Kind, method, onnx::Kind##Attr::ValueType)
CREATE_ACCESSOR(Float, f)
CREATE_ACCESSOR(Floats, fs)
CREATE_ACCESSOR(String, s)
CREATE_ACCESSOR(Strings, ss)
CREATE_ACCESSOR(Int, i)
CREATE_ACCESSOR(Ints, is)
CREATE_ACCESSOR(Graph, g)
CREATE_ACCESSOR(Graphs, gs)

CREATE_ACCESSOR_BASE(Float, f, float)
CREATE_ACCESSOR_BASE(String, s, const char *)
CREATE_ACCESSOR_BASE(Int, i, int32_t)

#undef CREATE_ACCESSOR_BASE
#undef CREATE_ACCESSOR
template <typename T> matchAttr<T> mAttr(const std::string &pName, T pV)
{
  return matchAttr<T>(pName, pV);
}
struct matchNoAttr {
  std::string m_Name;
  matchNoAttr(const std::string &pName) : m_Name(pName) {}
  bool match(onnx::Node *pN) const
  {
    return not pN->hasAttribute(onnx::Symbol(m_Name));
  }
};
matchNoAttr mNoAttr(const std::string &pName) { return matchNoAttr(pName); }

match_combine_or<matchNoAttr, matchAttr<onnx::IntAttr::ValueType> >
mFalseAttr(const std::string &pName)
{
  return m_CombineOr(mNoAttr(pName), mAttr<onnx::IntAttr::ValueType>(pName, 0));
}
matchAttr<onnx::IntAttr::ValueType> mTrueAttr(const std::string &pName)
{
  return mAttr<onnx::IntAttr::ValueType>(pName, 1);
}

} // namespace PatternMatch
} // namespace onnc

#endif /* PATTERNMATCH_H */
