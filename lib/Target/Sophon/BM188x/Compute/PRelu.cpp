//===- PRelu.cpp ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "PRelu.h"
#include "../BM188xVisitor.h"
#include <onnc/Support/Casting.h>

#define DEBUG_TYPE "tg_prelu"

using namespace onnc;
using namespace onnc::BM188X;

char BM188X::PRelu::ID = 0;

//===----------------------------------------------------------------------===//
// PRelu
//===----------------------------------------------------------------------===//
BM188X::PRelu::PRelu()
    : onnc::PRelu(), m_GTRShiftWidth(0), m_LERShiftWidth(0), m_GTScale(0)
{
  setID(ID);
}

BM188X::PRelu::~PRelu()
{
}

void BM188X::PRelu::print(std::ostream &pOS) const
{
  // TODO
}

void BM188X::PRelu::accept(ComputeVisitor& pV)
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

void BM188X::PRelu::accept(ComputeVisitor& pV) const
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

bool BM188X::PRelu::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
